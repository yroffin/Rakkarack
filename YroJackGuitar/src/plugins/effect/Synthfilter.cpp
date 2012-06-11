/*

 Synthfilter.C  - Approximate digital model of an analog JFET phaser.
 Analog modeling implemented by Ryan Billing aka Transmogrifox.
 November, 2009

 Credit to:
 ///////////////////
 ZynAddSubFX - a software synthesizer
 
 Phaser.C - Phaser effect
 Copyright (C) 2002-2005 Nasca Octavian Paul
 Author: Nasca Octavian Paul

 Modified for rakarrack by Josep Andreu

 DSP analog modeling theory & practice largely influenced by various CCRMA publications, particularly works by Julius O. Smith.
 ////////////////////


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

#include <plugins/effect/Synthfilter.h>
using namespace std;

#define ONE_  0.99999f        // To prevent LFO ever reaching 1.0 for filter stability purposes
#define ZERO_ 0.00001f        // Same idea as above.
Synthfilter::Synthfilter() :
		YroEffectPlugin("Synthfilter",
				"Low Pass: 0, 20, 14, 0, 1, 64, 110, -40, 6, 0, 0, 32, -32, 500, 100, 0;"
						"High Pass: 0, 20, 14, 0, 1, 64, 110, -40, 0, 6, 0, 32, -32, 500, 100, 0;"
						"Band Pass: 0, 20, 14, 0, 1, 64, 110, -40, 4, 4, 0, 32, -32, 500, 100, 0;"
						"Lead Synth: 0, 89, 31, 0, 1, 95, 38, -16, 1, 2, 1, 114, -32, 92, 215, 29;"
						"Water: 20, 69, 88, 0, 6, 0, 76, -50, 6, 2, 1, 0, 19, 114, 221, 127;"
						"Pan Filter: 0, 20, 100, 0, 5, 127, 127, -64, 2, 0, 0, 57, 0, 340, 288, 110;"
						"Multi: 64, 45, 88, 0, 1, 127, 81, 0, 4, 2, 0, 67, 0, 394, 252, 61;") {

	lyn1 = new float[MAX_SFILTER_STAGES];
	ryn1 = new float[MAX_SFILTER_STAGES];
	lx1hp = new float[MAX_SFILTER_STAGES];
	rx1hp = new float[MAX_SFILTER_STAGES];
	ly1hp = new float[MAX_SFILTER_STAGES];
	ry1hp = new float[MAX_SFILTER_STAGES];

	Plpstages = 4;
	Phpstages = 2;

	inv_period = 1.0f / fPERIOD;

	delta = cSAMPLE_RATE;
	Rmin = 185.0f; // 2N5457 typical on resistance at Vgs = 0
	Rmax = 22000.0f; // Resistor
	C = 0.00000005f; // 50 nF
	Chp = 0.00000005f;
	Clp = 0.00000005f;
	att = delta * 5.0f; //200ms
	rls = delta * 5.0f; //200ms
	env = 0.;

	setPreset(0);
	cleanup();
}
;

Synthfilter::~Synthfilter() {
}
;

/*
 * Effect output
 */
void Synthfilter::render(jack_nframes_t nframes, float * smpsl, float * smpsr) {
	int i, j;
	float lfol, lfor, lgain, rgain, rmod, lmod, d;
	lgain = 0.0;
	rgain = 0.0;

	lfo.render(1, &lfol, &lfor);
	lmod = lfol * width + depth + env * sns;
	rmod = lfor * width + depth + env * sns;

	if (lmod > ONE_)
		lmod = ONE_;
	else if (lmod < ZERO_)
		lmod = ZERO_;
	if (rmod > ONE_)
		rmod = ONE_;
	else if (rmod < ZERO_)
		rmod = ZERO_;

	lmod = 1.0f - lmod;
	rmod = 1.0f - rmod;
	lmod *= lmod;
	rmod *= rmod;

	float xl = (lmod - oldlgain) * inv_period;
	float xr = (rmod - oldrgain) * inv_period;
	float gl = oldlgain; // Linear interpolation between LFO samples
	float gr = oldrgain;

	for (i = 0; i < iPERIOD; i++) {

		float lxn = bandgain * smpsl[i];
		float rxn = bandgain * smpsr[i]; //extra gain

		gl += xl;
		gr += xr; //linear interpolation of LFO

		//Envelope detection
		envdelta = (fabsf(smpsl[i]) + fabsf(smpsr[i])) - env; //envelope follower from Compressor.C
		if (delta > 0.0)
			env += att * envdelta;
		else
			env += rls * envdelta;

		//End envelope power detection

		if (Plpstages < 1) {
			lxn += fbl;
			rxn += fbr;
		}

		//Left channel Low Pass Filter
		for (j = 0; j < Plpstages; j++) {
			d = 1.0f + fabs(lxn) * distortion; // gain decreases as signal amplitude increases.

			//low pass filter:  alpha*x[n] + (1-alpha)*y[n-1]
			// alpha = lgain = dt/(RC + dt)
			lgain = delta / ((Rmax * gl * d + Rmin) * Clp + delta);
			lyn1[j] = lgain * lxn + (1.0f - lgain) * lyn1[j];
			lyn1[j] += DENORMAL_GUARD;
			lxn = lyn1[j];
			if (j == 0)
				lxn += fbl; //Insert feedback after first filter stage
		};

		//Left channel High Pass Filter
		for (j = 0; j < Phpstages; j++) {

			//high pass filter:  alpha*(y[n-1] + x[n] - x[n-1]) // alpha = lgain = RC/(RC + dt)
			lgain = (Rmax * gl + Rmin) * Chp
					/ ((Rmax * gl + Rmin) * Chp + delta);
			ly1hp[j] = lgain * (lxn + ly1hp[j] - lx1hp[j]);

			ly1hp[j] += DENORMAL_GUARD;
			lx1hp[j] = lxn;
			lxn = ly1hp[j];

		};

		//Right channel Low Pass Filter
		for (j = 0; j < Plpstages; j++) {
			d = 1.0f + fabs(rxn) * distortion; //This is symmetrical. FET is not, so this deviates slightly, however sym dist. is better sounding than a real FET.

			rgain = delta / ((Rmax * gr * d + Rmin) * Clp + delta);
			ryn1[j] = rgain * rxn + (1.0f - rgain) * ryn1[j];
			ryn1[j] += DENORMAL_GUARD;
			rxn = ryn1[j];
			if (j == 0)
				rxn += fbr; //Insert feedback after first filter stage
		};

		//Right channel High Pass Filter
		for (j = 0; j < Phpstages; j++) {
			d = 1.0f + fabs(rxn) * distortion; // gain decreases as signal amplitude increases.

			//high pass filter:  alpha*(y[n-1] + x[n] - x[n-1]) // alpha = rgain = RC/(RC + dt)
			rgain = (Rmax * gr + Rmin) * Chp
					/ ((Rmax * gr + Rmin) * Chp + delta);
			ry1hp[j] = rgain * (rxn + ry1hp[j] - rx1hp[j]);

			ry1hp[j] += DENORMAL_GUARD;
			rx1hp[j] = rxn;
			rxn = ry1hp[j];
		};

		fbl = lxn * fb;
		fbr = rxn * fb;

		efxoutl[i] = lxn;
		efxoutr[i] = rxn;

	};

	oldlgain = lmod;
	oldrgain = rmod;

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
void Synthfilter::cleanup() {
	fbl = 0.0f;
	fbr = 0.0f;
	oldlgain = 0.0f;
	oldrgain = 0.0f;
	env = 0.0f;
	envdelta = 0.0f;
	for (int i = 0; i < MAX_SFILTER_STAGES; i++) {
		lyn1[i] = 0.0f;
		ryn1[i] = 0.0f;

		ly1hp[i] = 0.0f;
		lx1hp[i] = 0.0f;
		ry1hp[i] = 0.0f;
		rx1hp[i] = 0.0f;
	};
}
;

/*
 * Parameter control
 */
void Synthfilter::setWidth(int Pwidth) {
	this->Pwidth = Pwidth;
	width = ((float) Pwidth / 127.0f);
}
;

void Synthfilter::setFb(int Pfb) {
	this->Pfb = Pfb;
	fb = (float) Pfb;
	if (fb < 0.0f)
		fb /= 18.0f;
	else if (fb > 0.0f)
		fb /= 65.0f;
	if (Plpstages <= 2)
		fb *= 0.3; //keep filter stable when phase shift is small

}
;

void Synthfilter::setVolume(int Pvolume) {
	this->Pvolume = Pvolume;
	// outvolume is needed in calling program
	outvolume = (float) Pvolume / 127.0f;
}
;

void Synthfilter::setDistortion(int Pdistortion) {
	this->Pdistortion = Pdistortion;
	distortion = (float) Pdistortion / 127.0f;
}
;

void Synthfilter::setDepth(int Pdepth) {
	this->Pdepth = Pdepth;
	depth = (float) (Pdepth - 32) / 95.0f; //Pdepth input should be 0-127. .
}
;

void Synthfilter::setPstage(int value) {
	Plpstages = value;
	if (Plpstages >= MAX_SFILTER_STAGES)
		Plpstages = MAX_SFILTER_STAGES;
	if (Plpstages <= 2)
		fb = (float) Pfb * 0.25 / 65.0f; //keep filter stable when phase shift is small
	cleanup();
	if (Phpstages && Plpstages)
		bandgain = powf(((float) (Phpstages * Plpstages + 3)),
				(1.0f - (float) Pbandwidth / 127.0f));
	else
		bandgain = 1.0f;
}

void Synthfilter::setHpstages(int value) {
	Phpstages = value;
	if (Phpstages >= MAX_SFILTER_STAGES)
		Phpstages = MAX_SFILTER_STAGES;
	cleanup();
	if (Phpstages && Plpstages)
		bandgain = powf(((float) (Phpstages * Plpstages + 3)),
				(1.0f - (float) Pbandwidth / 127.0f));
	else
		bandgain = 1.0f;
}

void Synthfilter::setOutsub(int value) {
	if (value > 1)
		value = 1;
	Poutsub = value;
}

void Synthfilter::setEnvelope(int value) {
	Penvelope = value;
	sns = (float) Penvelope / 8.0f;
}

void Synthfilter::setAttack(int value) {
	Pattack = value;
	if (Pattack < 5)
		Pattack = 5;
	att = delta * 1000.0f / ((float) Pattack);
}

void Synthfilter::setBandwidth(int value) {
	Pbandwidth = value;
	Chp = C * (1.0f + ((float) value) / 64.0f); // C*3
	Clp = C * (1.0f - ((float) value) / 190.0f); // C/3
}

void Synthfilter::setRelease(int value) {
	Prelease = value;
	if (Prelease < 5)
		Prelease = 5;
	rls = delta * 1000.0f / ((float) Prelease);
}

int  Synthfilter::getVolume() {return Pvolume;}
int  Synthfilter::getDistortion() {return Pdistortion;}
int  Synthfilter::getLfoPfreq() {return lfo.getPfreq();}
int  Synthfilter::getLfoPrandomness() {return lfo.getPrandomness();}
int  Synthfilter::getLfoPlfOtype() {return lfo.getPlfOtype();}
int  Synthfilter::getLfoPstereo() {return lfo.getPstereo();}
int  Synthfilter::getWidth() {return Pwidth;}
int  Synthfilter::getFb() {return Pfb;}
int  Synthfilter::getPstage() {return Plpstages;}
int  Synthfilter::getHpstages() {return Phpstages;}
int  Synthfilter::getOutsub() {return Poutsub;}
int  Synthfilter::getDepth() {return Pdepth;}
int  Synthfilter::getEnvelope() {return Penvelope;}
int  Synthfilter::getAttack() {return Pattack;}
int  Synthfilter::getRelease() {return Prelease;}
int  Synthfilter::getBandwidth() {return Pbandwidth;}

void Synthfilter::setLfoPfreq(int value) {lfo.setPfreq(value);}
void Synthfilter::setLfoPrandomness(int value) {lfo.setPrandomness(value);}
void Synthfilter::setLfoPlfOtype(int value) {lfo.setPlfOtype(value);}
void Synthfilter::setLfoPstereo(int value) {lfo.setPstereo(value);}



/**
 * toXml member
*/
const char *Synthfilter::toXml() {
        char _buffer[256];
        char _formatd[] = {"<attribute name=\"%s\" value=\"%d\" />"};
        char _formatf[] = {"<attribute name=\"%s\" value=\"%9.40f\" />"};
        strcpy(_toXml,"<attributes>");
        sprintf(_buffer,_formatd,"Pattack",Pattack);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatd,"Pbandwidth",Pbandwidth);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatd,"Pdepth",Pdepth);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatd,"Pdistortion",Pdistortion);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatd,"Penvelope",Penvelope);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatd,"Pfb",Pfb);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatd,"Phpstages",Phpstages);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatd,"Plpstages",Plpstages);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatd,"Poutsub",Poutsub);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatd,"Prelease",Prelease);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatd,"Pvolume",Pvolume);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatf,"C",C);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatf,"Clp",Clp);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatf,"Chp",Chp);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatf,"delta",delta);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatf,"distortion",distortion);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatf,"fb",fb);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatf,"width",width);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatf,"env",env);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatf,"envdelta",envdelta);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatf,"sns",sns);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatf,"att",att);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatf,"rls",rls);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatf,"fbl",fbl);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatf,"fbr",fbr);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatf,"depth",depth);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatf,"oldlgain",oldlgain);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatf,"oldrgain",oldrgain);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatf,"inv_period",inv_period);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatf,"outvolume",outvolume);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatf,"Rmax",Rmax);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatf,"Rmin",Rmin);
        strcat(_toXml,_buffer);
        strcat(_toXml,"</attributes>");
        return _toXml;
}
