/*
 ZynAddSubFX - a software synthesizer
 
 Distorsion.C - Distorsion effect
 Copyright (C) 2002-2005 Nasca Octavian Paul
 Author: Nasca Octavian Paul

 Modified for rakarrack by Josep Andreu & Ryan Billing

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

#include <plugins/effect/NewDist.h>
using namespace std;

/*
 * Waveshape (this is called by OscilGen::waveshape and Distorsion::process)
 */

NewDist::NewDist() :
		YroEffectPlugin("NewDist", "NewDist1: 0,64,64,83,65,15,0,2437,169,68,0;"
				"NewDist2: 0,64,64,95,45,6,0,3459,209,60,1;"
				"NewDist3: 0,64,64,43,77,16,0,2983,118,83,0;") {
	octoutl = (float *) malloc(sizeof(float) * iPERIOD);
	octoutr = (float *) malloc(sizeof(float) * iPERIOD);

	lpfl = new AnalogFilter(2, 22000, 1, 0);
	lpfr = new AnalogFilter(2, 22000, 1, 0);
	hpfl = new AnalogFilter(3, 20, 1, 0);
	hpfr = new AnalogFilter(3, 20, 1, 0);
	blockDCl = new AnalogFilter(2, 75.0f, 1, 0);
	blockDCr = new AnalogFilter(2, 75.0f, 1, 0);
	wshapel = new WaveShaper();
	wshaper = new WaveShaper();

	blockDCl->setfreq(75.0f);
	blockDCr->setfreq(75.0f);

	DCl = new AnalogFilter(3, 30, 1, 0);
	DCr = new AnalogFilter(3, 30, 1, 0);
	DCl->setfreq(30.0f);
	DCr->setfreq(30.0f);

	filterpars = new FilterParams(0, 64, 64);

	filterpars->Pcategory = 2;
	filterpars->Ptype = 0;
	filterpars->Pfreq = 72;
	filterpars->Pq = 76;
	filterpars->Pstages = 0;
	filterpars->Pgain = 76;

	filterl = new Filter(filterpars);
	filterr = new Filter(filterpars);

	setPreset(0);

	//default values
	Pvolume = 50;
	Plrcross = 40;
	Pdrive = 1;
	Plevel = 32;
	Ptype = 0;
	Pnegate = 0;
	Plpf = 127;
	Phpf = 0;
	Prfreq = 64;
	Pprefiltering = 0;
	Poctave = 0;
	togglel = 1.0;
	octave_memoryl = -1.0;
	toggler = 1.0;
	octave_memoryr = -1.0;
	octmix = 0.0;

	cleanup();
}
;

NewDist::~NewDist() {
}
;

/*
 * Cleanup the effect
 */
void NewDist::cleanup() {
	lpfl->cleanup();
	hpfl->cleanup();
	lpfr->cleanup();
	hpfr->cleanup();
	blockDCr->cleanup();
	blockDCl->cleanup();
	DCl->cleanup();
	DCr->cleanup();

}
;

/*
 * Apply the filters
 */

void NewDist::applyfilters(float * efxoutl, float * efxoutr) {
	lpfl->filterout(iPERIOD, fPERIOD, efxoutl);
	hpfl->filterout(iPERIOD, fPERIOD, efxoutl);
	lpfr->filterout(iPERIOD, fPERIOD, efxoutr);
	hpfr->filterout(iPERIOD, fPERIOD, efxoutr);

}
;

/*
 * Effect output
 */
void NewDist::render(jack_nframes_t nframes, float * smpsl, float * smpsr) {
	int i;
	float l, r, lout, rout;

	float inputvol = .5f;

	if (Pnegate != 0)
		inputvol *= -1.0f;

	if (Pprefiltering != 0)
		applyfilters(smpsl, smpsr);

	//no optimised, yet (no look table)

	wshapel->waveshapesmps(iPERIOD, smpsl, Ptype, Pdrive, 2);
	wshaper->waveshapesmps(iPERIOD, smpsr, Ptype, Pdrive, 2);

	memcpy(efxoutl, smpsl, iPERIOD * sizeof(float));
	memcpy(efxoutr, smpsl, iPERIOD * sizeof(float));

	if (octmix > 0.01f) {
		for (i = 0; i < iPERIOD; i++) {
			lout = efxoutl[i];
			rout = efxoutr[i];

			if ((octave_memoryl < 0.0f) && (lout > 0.0f))
				togglel *= -1.0f;
			octave_memoryl = lout;

			if ((octave_memoryr < 0.0f) && (rout > 0.0f))
				toggler *= -1.0f;
			octave_memoryr = rout;

			octoutl[i] = lout * togglel;
			octoutr[i] = rout * toggler;

		}

		blockDCr->filterout(iPERIOD, fPERIOD, octoutr);
		blockDCl->filterout(iPERIOD, fPERIOD, octoutl);
	}

	filterl->filterout(smpsl);
	filterr->filterout(smpsr);

	if (Pprefiltering == 0)
		applyfilters(efxoutl, efxoutr);

	float level = dB2rap (60.0f * (float)Plevel / 127.0f - 40.0f);

	for (i = 0; i < iPERIOD; i++) {
		lout = efxoutl[i];
		rout = efxoutr[i];

		l = lout * (1.0f - lrcross) + rout * lrcross;
		r = rout * (1.0f - lrcross) + lout * lrcross;

		if (octmix > 0.01f) {
			lout = l * (1.0f - octmix) + octoutl[i] * octmix;
			rout = r * (1.0f - octmix) + octoutr[i] * octmix;
		} else {
			lout = l;
			rout = r;
		}

		efxoutl[i] = lout * level * panning;
		efxoutr[i] = rout * level * (1.0f - panning);

	};

	DCr->filterout(iPERIOD, fPERIOD, efxoutr);
	DCl->filterout(iPERIOD, fPERIOD, efxoutl);

}
;

/*
 * Parameter control
 */
void NewDist::setVolume(int Pvolume) {
	this->Pvolume = Pvolume;

	outvolume = (float) Pvolume / 127.0f;
	if (Pvolume == 0)
		cleanup();

}
;

void NewDist::setPanning(int Ppanning) {
	this->Ppanning = Ppanning;
	panning = ((float) Ppanning + 0.5f) / 127.0f;
}
;

void NewDist::setLrcross(int Plrcross) {
	this->Plrcross = Plrcross;
	lrcross = (float) Plrcross / 127.0f * 1.0f;
}
;

void NewDist::setLpf(int value) {
	Plpf = value;
	float fr = (float) Plpf;
	lpfl->setfreq(fr);
	lpfr->setfreq(fr);
}
;

void NewDist::setHpf(int value) {
	Phpf = value;
	float fr = (float) Phpf;
	hpfl->setfreq(fr);
	hpfr->setfreq(fr);
}
void NewDist::setOctave(int Poctave) {
	this->Poctave = Poctave;
	octmix = (float) (Poctave) / 127.0f;
}
void NewDist::setDrive(int value) {
	Pdrive = value;
}
void NewDist::setLevel(int value) {
	Plevel = value;
}
void NewDist::setType(int value) {
	Ptype = value;
}
void NewDist::setNegate(int value) {
	if (value > 1)
		value = 1;
	Pnegate = value;
}
void NewDist::setRfreq(int value) {
	Prfreq = value;
	rfreq = expf(powf((float) value / 127.0f, 0.5f) * logf(25000.0f)) + 40.0f;
	filterl->setfreq(rfreq);
	filterr->setfreq(rfreq);
}
void NewDist::setPrefiltering(int value) {
	Pprefiltering = value;
}
int NewDist::getOctave() {
	return this->Poctave;
}
int NewDist::getDrive() {
	return Pdrive ;
}
int NewDist::getLevel() {
	return Plevel ;
}
int NewDist::getType() {
	return Ptype ;
}
int NewDist::getNegate() {
	return Pnegate ;
}
int NewDist::getRfreq() {
	return Prfreq ;
}
int NewDist::getPrefiltering() {
	return Pprefiltering ;
}

int NewDist::getHpf() {
	return Phpf ;
}
int NewDist::getLpf() {
	return Plpf ;
}

int NewDist::getPanning() {
	return Ppanning;
}
int NewDist::getLrcross() {
	return lrcross;
}
int NewDist::getVolume() {
	return Pvolume;
}
/**
 * toXml member
*/
const char *NewDist::toXml() {
        char _buffer[256];
        char _formatd[] = {"<attribute name=\"%s\" value=\"%d\" />"};
        char _formatf[] = {"<attribute name=\"%s\" value=\"%9.40f\" />"};
        strcpy(_toXml,"<attributes>");
        sprintf(_buffer,_formatd,"Pdrive",Pdrive);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatd,"Phpf",Phpf);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatd,"Plevel",Plevel);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatd,"Plpf",Plpf);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatd,"Plrcross",Plrcross);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatd,"Pnegate",Pnegate);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatd,"Poctave",Poctave);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatd,"Ppanning",Ppanning);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatd,"Pprefiltering",Pprefiltering);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatd,"Prfreq",Prfreq);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatd,"Ptype",Ptype);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatd,"Pvolume",Pvolume);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatf,"inpll[4096]",inpll[4096]);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatf,"inplr[4096]",inplr[4096]);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatf,"panning",panning);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatf,"lrcross",lrcross);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatf,"octave_memoryl",octave_memoryl);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatf,"togglel",togglel);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatf,"octave_memoryr",octave_memoryr);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatf,"toggler",toggler);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatf,"rfreq",rfreq);
        strcat(_toXml,_buffer);
        strcat(_toXml,"</attributes>");
        return _toXml;
}
