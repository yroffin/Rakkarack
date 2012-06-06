/*
 Rakarrack Audio FX
 Ring DSP Code based on "(author)" LADSPA plugin(swh-plugins).
 ZynAddSubFX effect structure - Copyright (C) 2002-2005 Nasca Octavian Paul
 Modified and adapted for rakarrack by Josep Andreu

 Ring.C - Ring Modulator effect

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

#include <plugins/effect/Ring.h>
using namespace std;

Ring::Ring() :
		YroEffectPlugin("Ring", "Saw-Sin: -64,0,-64,64,35,1,0,20,0,40,0,64,1;"
				"Estring: 0,0,0,64,100,82,0,100,0,0,0,64,0;"
				"Astring: 0,0,0,64,100,110,0,0,100,50,0,64,0;"
				"dissonance: 0,0,0,64,100,817,0,20,0,100,0,64,1;"
				"FastBeat: 0,0,0,64,100,15,0,20,0,100,0,64,1;"
				"RingAmp: 0,0,0,64,100,1,0,20,0,100,0,64,0;") {

	sin_tbl = (float *) malloc(sizeof(float) * iSAMPLE_RATE);
	tri_tbl = (float *) malloc(sizeof(float) * iSAMPLE_RATE);
	squ_tbl = (float *) malloc(sizeof(float) * iSAMPLE_RATE);
	saw_tbl = (float *) malloc(sizeof(float) * iSAMPLE_RATE);

	Create_Tables();

	offset = 0;

	//default values
	Pvolume = 50;
	Plrcross = 40;
	Plevel = 64;
	Pstereo = 0;

	scale = 1.0f;
	sin = 0.0f;
	tri = 0.0f;
	saw = 0.0f;
	squ = 0.0f;

	setPreset(0);
	cleanup();
}
;

Ring::~Ring() {
}
;

/*
 * Create Tables
 */
void Ring::Create_Tables() {
	int i;
	float SR = fSAMPLE_RATE;

	for (i = 0; i < iSAMPLE_RATE; i++)
		sin_tbl[i] = sinf((float) i * D_PI / SR);
	for (i = 0; i < iSAMPLE_RATE; i++)
		tri_tbl[i] = acosf(cosf((float) i * D_PI / SR)) / D_PI - 1.0f;
	for (i = 0; i < iSAMPLE_RATE; i++)
		squ_tbl[i] = (i < iSAMPLE_RATE / 2) ? 1.0f : -1.0f;
	for (i = 0; i < iSAMPLE_RATE; i++)
		saw_tbl[i] = ((2.0f * i) - SR) / SR;

}

/*
 * Cleanup the effect
 */
void Ring::cleanup() {

}
;

/*
 * Apply the filters
 */

/*
 * Effect output
 */
void Ring::render(jack_nframes_t nframes, float * smpsl, float * smpsr) {
	int i;
	float l, r, lout, rout, tmpfactor;

	float inputvol = (float) Pinput / 127.0f;

	if (Pstereo != 0) { //Stereo
		for (i = 0; i < iPERIOD; i++) {
			efxoutl[i] = smpsl[i] * inputvol;
			efxoutr[i] = smpsr[i] * inputvol;
			if (inputvol == 0.0) {
				efxoutl[i] = 1.0;
				efxoutr[i] = 1.0;
			}
		};
	} else {
		for (i = 0; i < iPERIOD; i++) {
			efxoutl[i] = (smpsl[i] + smpsr[i]) * inputvol;
			if (inputvol == 0.0)
				efxoutl[i] = 1.0;
		};
	};

	for (i = 0; i < iPERIOD; i++) {
		tmpfactor = depth
				* (scale
						* (sin * sin_tbl[offset] + tri * tri_tbl[offset]
								+ saw * saw_tbl[offset] + squ * squ_tbl[offset])
						+ idepth); //This is now mathematically equivalent, but less computation
		efxoutl[i] *= tmpfactor;

		if (Pstereo != 0) {
			efxoutr[i] *= tmpfactor;
		}
		offset += Pfreq;
		if (offset > iSAMPLE_RATE)
			offset -= iSAMPLE_RATE;
	}

	if (Pstereo == 0)
		memcpy(efxoutr, efxoutl, iPERIOD * sizeof(float));

	float level = dB2rap (60.0f * (float)Plevel / 127.0f - 40.0f);

	for (i = 0; i < iPERIOD; i++) {
		lout = efxoutl[i];
		rout = efxoutr[i];

		l = lout * (1.0f - lrcross) + rout * lrcross;
		r = rout * (1.0f - lrcross) + lout * lrcross;

		lout = l;
		rout = r;

		efxoutl[i] = lout * level * panning;
		efxoutr[i] = rout * level * (1.0f - panning);

	}

}
;

/*
 * Parameter control
 */

void Ring::setPanning(int Ppan) {
	Ppanning = Ppan;
	panning = (float) (Ppanning + 64) / 128.0f;
// is Ok ... 

}
;

void Ring::setLrcross(int Plrc) {
	Plrcross = Plrc;
	lrcross = (float) (Plrcross + 64) / 128.0f;

}
;

void Ring::setScale() {
	scale = sin + tri + saw + squ;
	if (scale == 0.0)
		scale = 1.0;
	scale = 1.0 / scale;
}

void Ring::setVolume(int value) {
	Pvolume = value;
	outvolume = (float) (64 + value) / 128.0f;
}
void Ring::setLevel(int value) {
	Plevel = value;
}
void Ring::setDepthp(int value) {
	Pdepthp = value;
	depth = (float) Pdepthp / 100.0;
	idepth = 1.0f - depth;
}
void Ring::setFreq(int value) {
	if (value > 20000) //Make sure bad inputs can't cause buffer overflow
			{
		Pfreq = 20000;
	} else if (value < 1) {
		Pfreq = 1;
	} else {
		Pfreq = value;
	}
}
void Ring::setStereo(int value) {
	if (value > 1)
		value = 1;
	Pstereo = value;
}
void Ring::setSin(int value) {
	Psin = value;
	sin = (float) Psin / 100.0;
	setScale();
}
void Ring::setTri(int value) {
	Ptri = value;
	tri = (float) Ptri / 100.0;
	setScale();
}
void Ring::setSaw(int value) {
	Psaw = value;
	saw = (float) Psaw / 100.0;
	setScale();
}
void Ring::setSqu(int value) {
	Psqu = value;
	squ = (float) Psqu / 100.0;
	setScale();
}
void Ring::setInput(int value) {
	Pinput = value;
}
void Ring::setAfreq(int value) {
	Pafreq = value;
}

int Ring::getVolume() {
	return Pvolume;
}
int Ring::getPanning() {
	return Ppanning;
}
int Ring::getLrcross() {
	return Plrcross;
}
int Ring::getLevel() {
	return Plevel;
}
int Ring::getDepthp() {
	return Pdepthp;
}
int Ring::getFreq() {
	return Pfreq;
}
int Ring::getStereo() {
	return Pstereo;
}
int Ring::getSin() {
	return Psin;
}
int Ring::getTri() {
	return Ptri;
}
int Ring::getSaw() {
	return Psaw;
}
int Ring::getSqu() {
	return Psqu;
}
int Ring::getInput() {
	return Pinput;
}
int Ring::getAfreq() {
	return Pafreq;
}
/**
 * toXml member
*/
const char *Ring::toXml() {
        char _buffer[256];
        char _formatd[] = {"<attribute name=\"%s\" value=\"%d\" />"};
        char _formatf[] = {"<attribute name=\"%s\" value=\"%9.40f\" />"};
        strcpy(_toXml,"<attributes>");
        sprintf(_buffer,_formatd,"offset",offset);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatd,"Pafreq",Pafreq);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatd,"Pdepthp",Pdepthp);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatd,"Pfreq",Pfreq);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatd,"Pinput",Pinput);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatd,"Plevel",Plevel);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatd,"Plrcross",Plrcross);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatd,"Ppanning",Ppanning);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatd,"Psaw",Psaw);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatd,"Psin",Psin);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatd,"Psqu",Psqu);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatd,"Pstereo",Pstereo);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatd,"Ptri",Ptri);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatd,"Pvolume",Pvolume);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatf,"panning",panning);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatf,"lrcross",lrcross);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatf,"sin",sin);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatf,"tri",tri);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatf,"saw",saw);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatf,"squ",squ);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatf,"scale",scale);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatf,"depth",depth);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatf,"idepth",idepth);
        strcat(_toXml,_buffer);
        strcat(_toXml,"</attributes>");
        return _toXml;
}
