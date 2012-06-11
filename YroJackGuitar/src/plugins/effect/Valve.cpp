/*
 Rakarrack Audio FX
 Valve DSP Code based Steve Harris valve LADSPA plugin(swh-plugins).
 ZynAddSubFX effect structure - Copyright (C) 2002-2005 Nasca Octavian Paul
 Modified and adapted for rakarrack by Josep Andreu

 Valve.C - Distorsion effect

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

#include <plugins/effect/Valve.h>
using namespace std;

Valve::Valve() :
		YroEffectPlugin("Valve",
				"Valve 1: 0, 64, 64, 127, 64, 0, 5841, 61, 1, 0, 69, 1, 84;"
						"Valve 2: 0, 64, 64, 127, 64, 0, 5078, 61, 1, 0, 112, 0, 30;"
						"Valve 3: 0, 64, 35, 80, 64, 1, 3134, 358, 1, 1, 100, 1, 30;") {

	lpfl = new AnalogFilter(2, 22000.0f, 1.0f, 0);
	lpfr = new AnalogFilter(2, 22000.0f, 1.0f, 0);
	hpfl = new AnalogFilter(3, 20.0f, 1.0f, 0);
	hpfr = new AnalogFilter(3, 20.0f, 1.0f, 0);
	harm = new HarmEnhancer(rm, 20.0f, 20000.0f, 1.0f);

	//default values
	Pvolume = 50;
	Plrcross = 40;
	Pdrive = 90;
	Plevel = 64;
	Pnegate = 0;
	Plpf = 127;
	Phpf = 0;
	Q_q = 64;
	q = (float) Q_q / 127.0f - 1.0f;
	Ped = 0;
	Pstereo = 0;
	Pprefiltering = 0;
	dist = 0.0f;
	setLpf(127);
	setHpf(1);
	atk = 1.0f - 40.0f / fSAMPLE_RATE;

	for (int i = 0; i < 10; i++)
		rm[i] = 0.0;
	rm[0] = 1.0f;
	rm[2] = -1.0f;
	rm[4] = 1.0f;
	rm[6] = -1.0f;
	rm[8] = 1.0f;
	harm->calcula_mag(rm);
	dist = 0.;

	setPreset(0);
	init_coefs();
	cleanup();
}
;

Valve::~Valve() {
}
;

/*
 * Cleanup the effect
 */
void Valve::cleanup() {
	lpfl->cleanup();
	hpfl->cleanup();
	lpfr->cleanup();
	hpfr->cleanup();
	otml = 0.0f;
	itml = 0.0f;
	otmr = 0.0f;
	itmr = 0.0f;

}
;

/*
 * Apply the filters
 */

void Valve::applyfilters(float * efxoutl, float * efxoutr) {
	lpfl->filterout(iPERIOD, fPERIOD, efxoutl);
	hpfl->filterout(iPERIOD, fPERIOD, efxoutl);

	if (Pstereo != 0) { //stereo
		lpfr->filterout(iPERIOD, fPERIOD, efxoutr);
		hpfr->filterout(iPERIOD, fPERIOD, efxoutr);
	};

}
;

float Valve::Wshape(float x) {

	if (x < factor)
		return (x);
	if (x > factor)
		return (factor
				+ (x - factor)
						/ powf(1.0f + ((x - factor) / (1.0f - factor)), 2.0f));
	if (x > 1.0f)
		return ((factor + 1.0f) * .5f);
	return (0.0);
}

/*
 * Effect output
 */
void Valve::render(jack_nframes_t nframes, float * smpsl, float * smpsr) {
	int i;

	float l, r, lout, rout, fx;

	if (Pstereo != 0) { //Stereo
		for (i = 0; i < iPERIOD; i++) {
			efxoutl[i] = smpsl[i] * inputvol;
			efxoutr[i] = smpsr[i] * inputvol;
		};
	} else {
		for (i = 0; i < iPERIOD; i++) {
			efxoutl[i] = (smpsl[i] + smpsr[i]) * inputvol;
		};
	};

	harm->harm_out(iPERIOD, efxoutl, efxoutr);

	if (Pprefiltering != 0)
		applyfilters(efxoutl, efxoutr);

	if (Ped) {
		for (i = 0; i < iPERIOD; i++) {
			efxoutl[i] = Wshape(efxoutl[i]);
			if (Pstereo != 0)
				efxoutr[i] = Wshape(efxoutr[i]);
		}
	}

	for (i = 0; i < iPERIOD; i++) //soft limiting to 3.0 (max)
			{
		fx = efxoutl[i];
		if (fx > 1.0f)
			fx = 3.0f - 2.0f / sqrtf(fx);
		efxoutl[i] = fx;
		fx = efxoutr[i];
		if (fx > 1.0f)
			fx = 3.0f - 2.0f / sqrtf(fx);
		efxoutr[i] = fx;
	}

	if (q == 0.0f) {
		for (i = 0; i < iPERIOD; i++) {
			if (efxoutl[i] == q)
				fx = fdist;
			else
				fx = efxoutl[i]
						/ (1.0f - powf(2.0f, -dist * efxoutl[i] * LN2R));
			otml = atk * otml + fx - itml;
			itml = fx;
			efxoutl[i] = otml;
		}
	} else {
		for (i = 0; i < iPERIOD; i++) {
			if (efxoutl[i] == q)
				fx = fdist + qcoef;
			else
				fx = (efxoutl[i] - q)
						/ (1.0f - powf(2.0f, -dist * (efxoutl[i] - q) * LN2R))
						+ qcoef;
			otml = atk * otml + fx - itml;
			itml = fx;
			efxoutl[i] = otml;

		}
	}

	if (Pstereo != 0) {

		if (q == 0.0f) {
			for (i = 0; i < iPERIOD; i++) {
				if (efxoutr[i] == q)
					fx = fdist;
				else
					fx = efxoutr[i]
							/ (1.0f - powf(2.0f, -dist * efxoutr[i] * LN2R));
				otmr = atk * otmr + fx - itmr;
				itmr = fx;
				efxoutr[i] = otmr;

			}
		} else {
			for (i = 0; i < iPERIOD; i++) {
				if (efxoutr[i] == q)
					fx = fdist + qcoef;
				else
					fx = (efxoutr[i] - q)
							/ (1.0f
									- powf(2.0f,
											-dist * (efxoutr[i] - q) * LN2R))
							+ qcoef;
				otmr = atk * otmr + fx - itmr;
				itmr = fx;
				efxoutr[i] = otmr;

			}
		}

	}

	if (Pprefiltering == 0)
		applyfilters(efxoutl, efxoutr);

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

		efxoutl[i] = lout * 2.0f * level * panning;
		efxoutr[i] = rout * 2.0f * level * (1.0f - panning);

	};

}
;

/*
 * Parameter control
 */
void Valve::init_coefs() {
	coef = 1.0 / (1.0f - powf(2.0f, dist * q * LN2R));
	qcoef = q * coef;
	fdist = 1.0f / dist;
	inputvol = powf(4.0f, ((float) Pdrive - 32.0f) / 127.0f);
	if (Pnegate != 0)
		inputvol *= -1.0f;
}
;

void Valve::setVolume(int Pvolume) {
	this->Pvolume = Pvolume;

	outvolume = (float) Pvolume / 127.0f;
	if (Pvolume == 0)
		cleanup();
	init_coefs();
}

void Valve::setPanning(int Ppanning) {
	this->Ppanning = Ppanning;
	panning = ((float) Ppanning + 0.5f) / 127.0f;
	init_coefs();
}

void Valve::setLrcross(int Plrcross) {
	this->Plrcross = Plrcross;
	lrcross = (float) Plrcross / 127.0f * 1.0f;
	init_coefs();
}

void Valve::setLpf(int value) {
	Plpf = value;
	float fr = (float) Plpf;

	lpfl->setfreq(fr);
	lpfr->setfreq(fr);
	init_coefs();
}

void Valve::setHpf(int value) {
	Phpf = value;
	float fr = (float) Phpf;

	hpfl->setfreq(fr);
	hpfr->setfreq(fr);

	//Prefiltering of 51 is approx 630 Hz. 50 - 60 generally good for OD pedal.
	init_coefs();
}

void Valve::setPresence(int value) {
	Presence = value;

	float freq = 5.0f * (100.0f - (float) value);
	float nvol = (float) (value * .01f);

	harm->set_freqh(1, freq);
	harm->set_vol(1, nvol);
	init_coefs();
}

void Valve::setDrive(int value) {
	Pdrive = value;
	dist = (float) Pdrive / 127.0f * 40.0f + .5f;
	init_coefs();
}

void Valve::setLevel(int value) {
	Plevel = value;
	init_coefs();
}
void Valve::setNegate(int value) {
	if (value > 1)
		value = 1;
	Pnegate = value;
	init_coefs();
}

void Valve::setStereo(int value) {
	if (value > 1)
		value = 1;
	Pstereo = value;
	init_coefs();
}

void Valve::setPrefiltering(int value) {
	Pprefiltering = value;
	init_coefs();
}

void Valve::setQq(int value) {
	Q_q = value;
	q = (float) Q_q / 127.0f - 1.0f;
	factor = 1.0f - ((float) Q_q / 128.0f);
	init_coefs();
}

void Valve::setEd(int value) {
	Ped = value;
	init_coefs();
}

int  Valve::getVolume() {return Pvolume;}
int  Valve::getPanning() {return Ppanning;}
int  Valve::getLrcross() {return Plrcross;}
int  Valve::getDrive() {return Pdrive;}
int  Valve::getLevel() {return Plevel;}
int  Valve::getNegate() {return Pnegate;}
int  Valve::getLpf() {return Plpf;}
int  Valve::getHpf() {return Phpf;}
int  Valve::getStereo() {return Pstereo;}
int  Valve::getPrefiltering() {return Pprefiltering;}
int  Valve::getQq() {return Q_q;}
int  Valve::getEd() {return Ped;}
int  Valve::getPresence() {return Presence;}

/**
 * toXml member
*/
const char *Valve::toXml() {
        char _buffer[256];
        char _formatd[] = {"<attribute name=\"%s\" value=\"%d\" />"};
        char _formatf[] = {"<attribute name=\"%s\" value=\"%9.40f\" />"};
        strcpy(_toXml,"<attributes>");
        sprintf(_buffer,_formatd,"Pdrive",Pdrive);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatd,"Ped",Ped);
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
        sprintf(_buffer,_formatd,"Ppanning",Ppanning);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatd,"Pprefiltering",Pprefiltering);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatd,"Presence",Presence);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatd,"Pstereo",Pstereo);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatd,"Pvolume",Pvolume);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatd,"Q_q",Q_q);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatf,"coef",coef);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatf,"fdist",fdist);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatf,"inputvol",inputvol);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatf,"outvolume",outvolume);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatf,"panning",panning);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatf,"lrcross",lrcross);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatf,"q",q);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatf,"dist",dist);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatf,"otml",otml);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatf,"otmr",otmr);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatf,"itml",itml);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatf,"itmr",itmr);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatf,"factor",factor);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatf,"atk",atk);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatf,"qcoef",qcoef);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatf,"rm[10]",rm[10]);
        strcat(_toXml,_buffer);
        strcat(_toXml,"</attributes>");
        return _toXml;
}
