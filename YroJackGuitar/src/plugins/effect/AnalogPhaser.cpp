/*
 AnalogPhaser.C  - Approximate digital model of an analog JFET phaser.
 Analog modeling implemented by Ryan Billing aka Transmogrifox.
 November, 2009

 Credit to:

 ZynAddSubFX - a software synthesizer
 
 Phaser.C - Phaser effect
 Copyright (C) 2002-2005 Nasca Octavian Paul
 Author: Nasca Octavian Paul

 Modified for rakarrack by Josep Andreu
 DSP analog modeling theory & practice largely influenced by various CCRMA publications, particularly works by Julius O. Smith.
 Modified by Yannick Roffin

 This program is free software; you can redistribute it and/or modify
 it under the terms of version 2 of the GNU General Public License
 as published by the Free Software Foundation.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License (version 2) for more details.

 You should have received a copy of the GNU General Public License (version 2)
 along with this program; if not, write to the Free Software Foundation,
 Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
 */

#include <plugins/effect/AnalogPhaser.h>
using namespace std;

#define PHASER_LFO_SHAPE 2
#define ONE_  0.99999f        // To prevent LFO ever reaching 1.0 for filter stability purposes
#define ZERO_ 0.00001f        // Same idea as above.
AnalogPhaser::AnalogPhaser() :
		YroEffectPlugin("AnalogPhaser",
				        "Phaser1: 64, 20, 14, 0, 1, 64, 110, 40, 4, 10, 0, 64, 1;"
						"Phaser2: 64, 20, 14, 5, 1, 64, 110, 40, 6, 10, 0, 70, 1;"
						"Phaser3: 64, 20, 9, 0, 0, 64, 40, 40, 8, 10, 0, 60, 0;"
						"Phaser4: 64, 20, 14, 10, 0, 64, 110, 80, 7, 10, 1, 45, 1;"
						"Phaser5: 25, 20, 240, 10, 0, 64, 25, 16, 8, 100, 0, 25, 0;"
						"Phaser6: 64, 20, 1, 10, 1, 64, 110, 40, 12, 10, 0, 70, 1;") {

	lxn1 = (float *) malloc(sizeof(float) * MAX_PHASER_STAGES);
	lyn1 = (float *) malloc(sizeof(float) * MAX_PHASER_STAGES);
	rxn1 = (float *) malloc(sizeof(float) * MAX_PHASER_STAGES);
	ryn1 = (float *) malloc(sizeof(float) * MAX_PHASER_STAGES);
	offset = (float *) malloc(sizeof(float) * MAX_PHASER_STAGES); //model mismatch between JFET devices

	offset[0] = -0.2509303f;
	offset[1] = 0.9408924f;
	offset[2] = 0.998f;
	offset[3] = -0.3486182f;
	offset[4] = -0.2762545f;
	offset[5] = -0.5215785f;
	offset[6] = 0.2509303f;
	offset[7] = -0.9408924f;
	offset[8] = -0.998f;
	offset[9] = 0.3486182f;
	offset[10] = 0.2762545f;
	offset[11] = 0.5215785f;

	barber = 0; //Deactivate barber pole phasing by default

	mis = 1.0f;
	Rmin = 625.0f; // 2N5457 typical on resistance at Vgs = 0
	Rmax = 22000.0f; // Resistor parallel to FET
	Rmx = Rmin / Rmax;
	Rconst = 1.0f + Rmx; // Handle parallel resistor relationship
	C = 0.00000005f; // 50 nF
	CFs = 2.0f * fSAMPLE_RATE * C;
	invperiod = 1.0f / fPERIOD;

	setPreset(0);
	cleanup();
}

AnalogPhaser::~AnalogPhaser() {
}

/*
 * Effect output
 */
void AnalogPhaser::render(jack_nframes_t nframes, float * smpsl,
		float * smpsr) {
	int i, j;
	float lfol, lfor, lgain, rgain, bl, br, gl, gr, rmod, lmod, d, hpfr, hpfl;
	lgain = 0.0;
	rgain = 0.0;

	//initialize hpf
	hpfl = 0.0;
	hpfr = 0.0;

	lfo.render(nframes, &lfol, &lfor);
	lmod = lfol * width + depth;
	rmod = lfor * width + depth;

	if (lmod > ONE_)
		lmod = ONE_;
	else if (lmod < ZERO_)
		lmod = ZERO_;
	if (rmod > ONE_)
		rmod = ONE_;
	else if (rmod < ZERO_)
		rmod = ZERO_;

	if (Phyper != 0) {
		lmod *= lmod; //Triangle wave squared is approximately sin on bottom, tri on top
		rmod *= rmod; //Result is exponential sweep more akin to filter in synth with exponential generator circuitry.
	};

	lmod = sqrtf(1.0f - lmod); //gl,gr is Vp - Vgs. Typical FET drain-source resistance follows constant/[1-sqrt(Vp - Vgs)]
	rmod = sqrtf(1.0f - rmod);

	rdiff = (rmod - oldrgain) * invperiod;
	ldiff = (lmod - oldlgain) * invperiod;

	gl = oldlgain;
	gr = oldrgain;

	oldlgain = lmod;
	oldrgain = rmod;

	for (i = 0; i < iPERIOD; i++) {

		gl += ldiff; // Linear interpolation between LFO samples
		gr += rdiff;

		float lxn = smpsl[i];
		float rxn = smpsr[i];

		if (barber) {
			gl = fmodf((gl + 0.25f), ONE_);
			gr = fmodf((gr + 0.25f), ONE_);
		};

		//Left channel
		for (j = 0; j < Pstages; j++) { //Phasing routine
			mis = 1.0f + offsetpct * offset[j];
			d = (1.0f + 2.0f * (0.25f + gl) * hpfl * hpfl * distortion) * mis; //This is symmetrical. FET is not, so this deviates slightly, however sym dist. is better sounding than a real FET.
			Rconst = 1.0f + mis * Rmx;
			bl = (Rconst - gl) / (d * Rmin); // This is 1/R. R is being modulated to control filter fc.
			lgain = (CFs - bl) / (CFs + bl);

			lyn1[j] = lgain * (lxn + lyn1[j]) - lxn1[j];
			lyn1[j] += DENORMAL_GUARD;
			hpfl = lyn1[j] + (1.0f - lgain) * lxn1[j]; //high pass filter -- Distortion depends on the high-pass part of the AP stage.

			lxn1[j] = lxn;
			lxn = lyn1[j];
			if (j == 1)
				lxn += fbl; //Insert feedback after first phase stage
		};

		//Right channel
		for (j = 0; j < Pstages; j++) { //Phasing routine
			mis = 1.0f + offsetpct * offset[j];
			d = (1.0f + 2.0f * (0.25f + gr) * hpfr * hpfr * distortion) * mis; // distortion
			Rconst = 1.0f + mis * Rmx;
			br = (Rconst - gr) / (d * Rmin);
			rgain = (CFs - br) / (CFs + br);

			ryn1[j] = rgain * (rxn + ryn1[j]) - rxn1[j];
			ryn1[j] += DENORMAL_GUARD;
			hpfr = ryn1[j] + (1.0f - rgain) * rxn1[j]; //high pass filter

			rxn1[j] = rxn;
			rxn = ryn1[j];
			if (j == 1)
				rxn += fbr; //Insert feedback after first phase stage
		};

		fbl = lxn * fb;
		fbr = rxn * fb;
		efxoutl[i] = lxn;
		efxoutr[i] = rxn;

	};

	if (Poutsub != 0)
		for (i = 0; i < iPERIOD; i++) {
			efxoutl[i] *= -1.0f;
			efxoutr[i] *= -1.0f;
		};

}
;

/*
 * Cleanup the effect
 */
void AnalogPhaser::cleanup() {
	fbl = 0.0;
	fbr = 0.0;
	oldlgain = 0.0;
	oldrgain = 0.0;
	Pbarber = 0;
	for (int i = 0; i < Pstages; i++) {
		lxn1[i] = 0.0;

		lyn1[i] = 0.0;

		rxn1[i] = 0.0;

		ryn1[i] = 0.0;

	}
}

/*
 * Parameter control
 */
void AnalogPhaser::setWidth(int Pwidth) {
	this->Pwidth = Pwidth;
	width = ((float) Pwidth / 127.0f);
}
;

void AnalogPhaser::setFb(int Pfb) {
	this->Pfb = Pfb;
	fb = (float) (Pfb - 64) / 64.2f;
}
;

void AnalogPhaser::setVolume(int Pvolume) {
	this->Pvolume = Pvolume;
	// outvolume is needed in calling program
	outvolume = (float) Pvolume / 127.0f;
}
;

void AnalogPhaser::setDistortion(int Pdistortion) {
	this->Pdistortion = Pdistortion;
	distortion = (float) Pdistortion / 127.0f;
}
;

void AnalogPhaser::setOffset(int Poffset) {
	this->Poffset = Poffset;
	offsetpct = (float) Poffset / 127.0f;
}
;

void AnalogPhaser::setStages(int Pstages) {

	if (Pstages >= MAX_PHASER_STAGES)
		Pstages = MAX_PHASER_STAGES;
	this->Pstages = Pstages;

	cleanup();
}
;

void AnalogPhaser::setDepth(int Pdepth) {
	this->Pdepth = Pdepth;
	depth = (float) (Pdepth - 64) / 127.0f; //Pdepth input should be 0-127.  depth shall range 0-0.5 since we don't need to shift the full spectrum.
}
;

int AnalogPhaser::getVolume() {
	return Pvolume;
}
int AnalogPhaser::getDistortion() {
	return Pdistortion;
}
int AnalogPhaser::getWidth() {
	return Pwidth;
}
int AnalogPhaser::getFb() {
	return Pfb;
}
int AnalogPhaser::getStages() {
	return Pstages;
}
int AnalogPhaser::getOffset() {
	return Poffset;
}
int AnalogPhaser::getDepth() {
	return Pdepth;
}

int AnalogPhaser::getOutsub() {
	return Poutsub;
}
void AnalogPhaser::setOutsub(int value) {
	Poutsub = value;
}
int AnalogPhaser::getHyper() {
	return Phyper;
}
void AnalogPhaser::setHyper(int value) {
	if (value > 1)
		value = 1;
	Phyper = value;
}

int AnalogPhaser::getLfoPstereo() {
	return lfo.getPstereo();
}
int AnalogPhaser::getLfoPlfOtype() {
	return lfo.getPlfOtype();
}
int AnalogPhaser::getLfoPrandomness() {
	return lfo.getPrandomness();
}
int AnalogPhaser::getLfoPfreq() {
	return lfo.getPfreq();
}

void AnalogPhaser::setLfoPstereo(int value) {
	lfo.setPstereo(value);
}
void AnalogPhaser::setLfoPlfOtype(int value) {
	lfo.setPlfOtype(value);
	barber = 0;
	if (value == 2)
		barber = 1;
}
void AnalogPhaser::setLfoPrandomness(int value) {
	lfo.setPrandomness(value);
}
void AnalogPhaser::setLfoPfreq(int value) {
	lfo.setPfreq(value);
}
/**
 * toXml member
*/
const char *AnalogPhaser::toXml() {
        char _buffer[256];
        char _formatd[] = {"<attribute name=\"%s\" value=\"%d\" />"};
        char _formatf[] = {"<attribute name=\"%s\" value=\"%9.40f\" />"};
        strcpy(_toXml,"<attributes>");
        sprintf(_buffer,_formatd,"Pbarber",Pbarber);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatd,"Pdepth",Pdepth);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatd,"Pdistortion",Pdistortion);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatd,"Pfb",Pfb);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatd,"Phyper",Phyper);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatd,"Poffset",Poffset);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatd,"Poutsub",Poutsub);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatd,"Pstages",Pstages);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatd,"Pvolume",Pvolume);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatf,"C",C);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatf,"CFs",CFs);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatf,"distortion",distortion);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatf,"fb",fb);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatf,"width",width);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatf,"offsetpct",offsetpct);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatf,"fbl",fbl);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatf,"fbr",fbr);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatf,"depth",depth);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatf,"mis",mis);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatf,"oldlgain",oldlgain);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatf,"oldrgain",oldrgain);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatf,"rdiff",rdiff);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatf,"ldiff",ldiff);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatf,"invperiod",invperiod);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatf,"outvolume",outvolume);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatf,"Rconst",Rconst);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatf,"Rmax",Rmax);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatf,"Rmin",Rmin);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatf,"Rmx",Rmx);
        strcat(_toXml,_buffer);
        strcat(_toXml,"</attributes>");
        return _toXml;
}
