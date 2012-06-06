/*
 ZynAddSubFX - a software synthesizer
 
 RBEcho.C - Echo effect
 Copyright (C) 2002-2005 Nasca Octavian Paul
 Author: Nasca Octavian Paul

 Modified for rakarrack by Josep Andreu
 Reverse Echo effect by Transmogrifox

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

#include <stdlib.h>

#include <plugins/effect/RBEcho.h>
using namespace std;

RBEcho::RBEcho() :
		YroEffectPlugin("RBEcho", "Echo1: 64,64,90,64,64,64,64,0,1,96;"
				"Echo2: 64,64,90,64,64,64,64,0,2,96;"
				"Echo3: 64,64,90,64,64,64,64,0,3,96;") {

	//default values
	Pvolume = 50;
	Ppanning = 64;
	Pdelay = 60;
	Plrdelay = 100;
	Plrcross = 100;
	Pfb = 40;
	Phidamp = 60;
	Psubdiv = 1;
	subdiv = 1.0f;

	ldelay = NULL;
	rdelay = NULL;
	lrdelay = 0;
	Srate_Attack_Coeff = 1.0f / (fSAMPLE_RATE * ATTACK);
	maxx_delay = 1 + iSAMPLE_RATE * MAX_DELAY;

	ldelay = new float[maxx_delay];
	rdelay = new float[maxx_delay];

	setPreset(0);
	cleanup();
}
;

RBEcho::~RBEcho() {
}
;

/*
 * Cleanup the effect
 */
void RBEcho::cleanup() {
	int i;
	for (i = 0; i < maxx_delay; i++)
		ldelay[i] = 0.0;
	for (i = 0; i < maxx_delay; i++)
		rdelay[i] = 0.0;
	oldl = 0.0;
	oldr = 0.0;
}
;

/*
 * Initialize the delays
 */
void RBEcho::initdelays() {
	int i;

	kl = 0;
	kr = 0;

	if (Plrdelay > 0) {
		rkl = lrdelay;
		rkr = lrdelay / 2;
	} else {
		rkr = lrdelay;
		rkl = lrdelay / 2;
	}
	if (rkl > delay)
		rkl = delay;
	if (rkr < 1)
		rkr = 1;
	if (rkr > delay)
		rkr = delay;
	if (rkl < 1)
		rkl = 1;

	rvkl = delay;
	rvkr = delay;
	Srate_Attack_Coeff = 15.0f * cSAMPLE_RATE; // Set swell time to 66ms

	for (i = delay; i < maxx_delay; i++) {
		ldelay[i] = 0.0;
		rdelay[i] = 0.0;
	}

	oldl = 0.0;
	oldr = 0.0;

}
;

/*
 * Effect output
 */
void RBEcho::render(jack_nframes_t nframes, float * smpsl, float * smpsr) {
	int i;
	float ldl, rdl, rswell, lswell;
	float avg, ldiff, rdiff, tmp;

	for (i = 0; i < iPERIOD; i++) {

		//LowPass Filter
		ldl = lfeedback * hidamp + oldl * (1.0f - hidamp);
		rdl = rfeedback * hidamp + oldr * (1.0f - hidamp);
		oldl = ldl + DENORMAL_GUARD;
		oldr = rdl + DENORMAL_GUARD;

		ldelay[kl] = ldl + smpsl[i];
		rdelay[kr] = ldl + smpsr[i];

		if (++kl > delay)
			kl = 0;
		if (++kr > delay)
			kr = 0;
		rvkl = delay - kl;
		rvkr = delay - kr;

		if (++rkl > delay)
			rkl = 0;
		if (++rkr > delay)
			rkr = 0;

		if (reverse > 0.0f) {

			lswell = ((float) (kl - rvkl)) * Srate_Attack_Coeff;
			lswell = 1.0f - 1.0f / (lswell * lswell + 1.0f);
			efxoutl[i] = reverse * (ldelay[rvkl] * lswell)
					+ (ldelay[kl] * (1.0f - reverse));

			rswell = ((float) (kr - rvkr)) * Srate_Attack_Coeff;
			rswell = 1.0f - 1.0f / (rswell * rswell + 1.0f);
			efxoutr[i] = reverse * (rdelay[rvkr] * rswell)
					+ (rdelay[kr] * (1.0f - reverse)); //Volume ducking near zero crossing.

		} else {
			efxoutl[i] = ldelay[kl];
			efxoutr[i] = rdelay[kr];
		}

		lfeedback = lpanning * fb * efxoutl[i];
		rfeedback = rpanning * fb * efxoutr[i];

		if (Pes) {
			efxoutl[i] *= cosf(lrcross);
			efxoutr[i] *= sinf(lrcross);

			avg = (efxoutl[i] + efxoutr[i]) * 0.5f;
			ldiff = efxoutl[i] - avg;
			rdiff = efxoutr[i] - avg;

			tmp = avg + ldiff * pes;
			efxoutl[i] = 0.5 * tmp;

			tmp = avg + rdiff * pes;
			efxoutr[i] = 0.5f * tmp;

		}
		efxoutl[i] = (efxoutl[i] + pingpong * rdelay[rkl]) * lpanning;
		efxoutr[i] = (efxoutr[i] + pingpong * ldelay[rkr]) * rpanning;

	};

}
;

/*
 * Parameter control
 */
void RBEcho::setVolume(int Pvolume) {
	this->Pvolume = Pvolume;
	outvolume = (float) Pvolume / 127.0f;

}
;

void RBEcho::setPanning(int Ppanning) {
	this->Ppanning = Ppanning;
	lpanning = ((float) Ppanning) / 64.0f;
	rpanning = 2.0f - lpanning;
	lpanning = 10.0f * powf(lpanning, 4);
	rpanning = 10.0f * powf(rpanning, 4);
	lpanning = 1.0f - 1.0f / (lpanning + 1.0f);
	rpanning = 1.0f - 1.0f / (rpanning + 1.0f);
	lpanning *= 1.1f;
	rpanning *= 1.1f;
}
;

void RBEcho::setReverse(int Preverse) {
	this->Preverse = Preverse;
	reverse = (float) Preverse / 127.0f;
}
;

void RBEcho::setDelay(int Pdelay) {
	this->Pdelay = Pdelay;
	fdelay = 60.0f / ((float) Pdelay);
	if (fdelay < 0.01f)
		fdelay = 0.01f;
	if (fdelay > (float) MAX_DELAY)
		fdelay = (float) MAX_DELAY; //Constrains 10ms ... MAX_DELAY
	delay = 1 + lrintf(subdiv * fdelay * fSAMPLE_RATE);
	initdelays();
}
;

void RBEcho::setLrdelay(int Plrdelay) {
	float tmp;
	this->Plrdelay = Plrdelay;
	tmp = ((float) delay) * fabs(((float) Plrdelay - 64.0f) / 65.0f);
	lrdelay = lrintf(tmp);

	tmp = fabs(((float) Plrdelay - 64.0f) / 32.0f);
	pingpong = 1.0f - 1.0f / (5.0f * tmp * tmp + 1.0f);
	pingpong *= 1.2f;
	initdelays();
}
;

void RBEcho::setLrcross(int Plrcross) {
	this->Plrcross = Plrcross;
	lrcross = D_PI * (float) Plrcross / 128.0f;

}
;

void RBEcho::setFb(int Pfb) {
	this->Pfb = Pfb;
	fb = (float) Pfb / 128.0f;
}
;

void RBEcho::setHidamp(int Phidamp) {
	this->Phidamp = Phidamp;
	hidamp = expf(-D_PI * 500.0f * ((float) Phidamp) / fSAMPLE_RATE);
}
;

void RBEcho::setSubdiv(int value) {
	Psubdiv = value;
	subdiv = 1.0f / ((float) (value + 1));
	delay = 1 + lrintf(subdiv * fdelay * fSAMPLE_RATE);
	initdelays();
}
void RBEcho::setEs(int value) {
	Pes = value;
	pes = 8.0f * (float) Pes / 127.0f;
}

int RBEcho::getVolume() {
	return Pvolume;
}
int RBEcho::getPanning() {
	return Ppanning;
}
int RBEcho::getDelay() {
	return Pdelay;
}
int RBEcho::getLrdelay() {
	return Plrdelay;
}
int RBEcho::getLrcross() {
	return Plrcross;
}
int RBEcho::getFb() {
	return Pfb;
}
int RBEcho::getHidamp() {
	return Phidamp;
}
int RBEcho::getReverse() {
	return Preverse;
}
int RBEcho::getSubdiv() {
	return Psubdiv;
}
int RBEcho::getEs() {
	return Pes;
}
/**
 * toXml member
*/
const char *RBEcho::toXml() {
        char _buffer[256];
        char _formatd[] = {"<attribute name=\"%s\" value=\"%d\" />"};
        char _formatf[] = {"<attribute name=\"%s\" value=\"%9.40f\" />"};
        strcpy(_toXml,"<attributes>");
        sprintf(_buffer,_formatd,"dl",dl);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatd,"dr",dr);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatd,"delay",delay);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatd,"lrdelay",lrdelay);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatd,"kl",kl);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatd,"kr",kr);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatd,"rkr",rkr);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatd,"rkl",rkl);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatd,"rvkl",rvkl);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatd,"rvkr",rvkr);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatd,"rvfl",rvfl);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatd,"rvfr",rvfr);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatd,"maxx_delay",maxx_delay);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatd,"Pdelay",Pdelay);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatd,"Pes",Pes);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatd,"Pfb",Pfb);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatd,"Phidamp",Phidamp);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatd,"Plrcross",Plrcross);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatd,"Plrdelay",Plrdelay);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatd,"Ppanning",Ppanning);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatd,"Preverse",Preverse);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatd,"Psubdiv",Psubdiv);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatd,"Pvolume",Pvolume);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatf,"fdelay",fdelay);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatf,"subdiv",subdiv);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatf,"pes",pes);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatf,"pingpong",pingpong);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatf,"oldl",oldl);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatf,"oldr",oldr);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatf,"rpanning",rpanning);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatf,"lpanning",lpanning);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatf,"lrcross",lrcross);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatf,"fb",fb);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatf,"hidamp",hidamp);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatf,"reverse",reverse);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatf,"lfeedback",lfeedback);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatf,"Srate_Attack_Coeff",Srate_Attack_Coeff);
        strcat(_toXml,_buffer);
        strcat(_toXml,"</attributes>");
        return _toXml;
}
