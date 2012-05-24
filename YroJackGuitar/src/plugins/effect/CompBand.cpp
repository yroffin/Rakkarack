/*

 CompBand.C - 4 Bands Compressor

 Using Compressor and AnalogFilters by other authors.

 Based on artscompressor.cc by Matthias Kretz <kretz@kde.org>
 Stefan Westerfeld <stefan@space.twc.de>

 Modified by Ryan Billing & Josep Andreu

 ZynAddSubFX - a software synthesizer
 Copyright (C) 2002-2005 Nasca Octavian Paul
 Author: Nasca Octavian Paul

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

#include <plugins/effect/CompBand.h>

using namespace std;

CompBand::CompBand() :
		YroEffectPlugin("CompBand"
				"GoodStart: 0,16,16,16,16,0,0,0,0,1000,5000,10000,48;"
				"Loudness: 0,16,2,2,4,-16,24,24,-8,140,1000,5000,48;"
				"Loudness2: 64,16,2,2,2,-32,24,24,24,100,1000,5000,48;") {

	lowl = (float *) malloc(sizeof(float) * iPERIOD);
	lowr = (float *) malloc(sizeof(float) * iPERIOD);
	midll = (float *) malloc(sizeof(float) * iPERIOD);
	midlr = (float *) malloc(sizeof(float) * iPERIOD);
	midhl = (float *) malloc(sizeof(float) * iPERIOD);
	midhr = (float *) malloc(sizeof(float) * iPERIOD);
	highl = (float *) malloc(sizeof(float) * iPERIOD);
	highr = (float *) malloc(sizeof(float) * iPERIOD);

	lpf1l = new AnalogFilter(2, 500.0f, .7071f, 0);
	lpf1r = new AnalogFilter(2, 500.0f, .7071f, 0);
	hpf1l = new AnalogFilter(3, 500.0f, .7071f, 0);
	hpf1r = new AnalogFilter(3, 500.0f, .7071f, 0);
	lpf2l = new AnalogFilter(2, 2500.0f, .7071f, 0);
	lpf2r = new AnalogFilter(2, 2500.0f, .7071f, 0);
	hpf2l = new AnalogFilter(3, 2500.0f, .7071f, 0);
	hpf2r = new AnalogFilter(3, 2500.0f, .7071f, 0);
	lpf3l = new AnalogFilter(2, 5000.0f, .7071f, 0);
	lpf3r = new AnalogFilter(2, 5000.0f, .7071f, 0);
	hpf3l = new AnalogFilter(3, 5000.0f, .7071f, 0);
	hpf3r = new AnalogFilter(3, 5000.0f, .7071f, 0);

	CL = new Compressor();
	CML = new Compressor();
	CMH = new Compressor();
	CH = new Compressor();

	CL->setPreset(5);
	CML->setPreset(5);
	CMH->setPreset(5);
	CH->setPreset(5);

	//default values
	Pvolume = 50;

	setPreset(0);
	cleanup();
}
;

CompBand::~CompBand() {
}
;

/*
 * Cleanup the effect
 */
void CompBand::cleanup() {
	lpf1l->cleanup();
	hpf1l->cleanup();
	lpf1r->cleanup();
	hpf1r->cleanup();
	lpf2l->cleanup();
	hpf2l->cleanup();
	lpf2r->cleanup();
	hpf2r->cleanup();
	lpf3l->cleanup();
	hpf3l->cleanup();
	lpf3r->cleanup();
	hpf3r->cleanup();
	CL->cleanup();
	CML->cleanup();
	CMH->cleanup();
	CH->cleanup();

}
;
/*
 * Effect output
 */
void CompBand::render(jack_nframes_t nframes, float * smpsl, float * smpsr) {
	int i;

	memcpy(lowl, smpsl, sizeof(float) * iPERIOD);
	memcpy(midll, smpsl, sizeof(float) * iPERIOD);
	memcpy(midhl, smpsl, sizeof(float) * iPERIOD);
	memcpy(highl, smpsl, sizeof(float) * iPERIOD);

	lpf1l->filterout(iPERIOD, fPERIOD, lowl);
	hpf1l->filterout(iPERIOD, fPERIOD, midll);
	lpf2l->filterout(iPERIOD, fPERIOD, midll);
	hpf2l->filterout(iPERIOD, fPERIOD, midhl);
	lpf3l->filterout(iPERIOD, fPERIOD, midhl);
	hpf3l->filterout(iPERIOD, fPERIOD, highl);

	memcpy(lowr, smpsr, sizeof(float) * iPERIOD);
	memcpy(midlr, smpsr, sizeof(float) * iPERIOD);
	memcpy(midhr, smpsr, sizeof(float) * iPERIOD);
	memcpy(highr, smpsr, sizeof(float) * iPERIOD);

	lpf1r->filterout(iPERIOD, fPERIOD, lowr);
	hpf1r->filterout(iPERIOD, fPERIOD, midlr);
	lpf2r->filterout(iPERIOD, fPERIOD, midlr);
	hpf2r->filterout(iPERIOD, fPERIOD, midhr);
	lpf3r->filterout(iPERIOD, fPERIOD, midhr);
	hpf3r->filterout(iPERIOD, fPERIOD, highr);

	CL->render(iPERIOD, lowl, lowr);
	CML->render(iPERIOD, midll, midlr);
	CMH->render(iPERIOD, midhl, midhr);
	CH->render(iPERIOD, highl, highr);

	for (i = 0; i < iPERIOD; i++) {
		efxoutl[i] = (lowl[i] + midll[i] + midhl[i] + highl[i]) * level;
		efxoutr[i] = (lowr[i] + midlr[i] + midhr[i] + highr[i]) * level;
	}

}
;

/*
 * Parameter control
 */
void CompBand::setVolume(int value) {
	Pvolume = value;
	outvolume = (float) Pvolume / 128.0f;

}
;

void CompBand::setLevel(int value) {
	Plevel = value;
	level = dB2rap (60.0f * (float)value / 127.0f - 36.0f);

}
;

void CompBand::setRatio(int ch, int value) {

	switch (ch) {
	case 0:
		CL->setRatio(value);
		break;
	case 1:
		CML->setRatio(value);
		break;
	case 2:
		CMH->setRatio(value);
		break;
	case 3:
		CH->setRatio(value);
		break;
	}
}

void CompBand::setThreshold(int ch, int value) {

	switch (ch) {
	case 0:
		CL->setThreshold(value);
		break;
	case 1:
		CML->setThreshold(value);
		break;
	case 2:
		CMH->setThreshold(value);
		break;
	case 3:
		CH->setThreshold(value);
		break;
	}
}

void CompBand::setCross1(int value) {
	Cross1 = value;
	lpf1l->setfreq((float) value);
	lpf1r->setfreq((float) value);
	hpf1l->setfreq((float) value);
	hpf1r->setfreq((float) value);

}
;

void CompBand::setCross2(int value) {
	Cross2 = value;
	hpf2l->setfreq((float) value);
	hpf2r->setfreq((float) value);
	lpf2l->setfreq((float) value);
	lpf2r->setfreq((float) value);

}
;

void CompBand::setCross3(int value) {
	Cross3 = value;
	hpf3l->setfreq((float) value);
	hpf3r->setfreq((float) value);
	lpf3l->setfreq((float) value);
	lpf3r->setfreq((float) value);

}
;

void CompBand::setLratio(int value) {
	PLratio = value;
	setRatio(0, value);
}
void CompBand::setMLratio(int value) {
	PMLratio = value;
	setRatio(1, value);
}
void CompBand::setMHratio(int value) {
	PMHratio = value;
	setRatio(2, value);
}
void CompBand::setHratio(int value) {
	PHratio = value;
	setRatio(3, value);
}
void CompBand::setLthreshold(int value) {
	PLthres = value;
	setThreshold(0, value);
}
void CompBand::setMLthreshold(int value) {
	PMLthres = value;
	setThreshold(1, value);
}
void CompBand::setMHthreshold(int value) {
	PMHthres = value;
	setThreshold(2, value);
}
void CompBand::setHthreshold(int value) {
	PHthres = value;
	setThreshold(3, value);
}

int CompBand::getVolume() {
	return Pvolume;
}
int CompBand::getLratio() {
	return PLratio;
}
int CompBand::getMLratio() {
	return PMLratio;
}
int CompBand::getMHratio() {
	return PMHratio;
}
int CompBand::getHratio() {
	return PHratio;
}
int CompBand::getLthreshold() {
	return PLthres;
}
int CompBand::getMLthreshold() {
	return PMLthres;
}
int CompBand::getMHthreshold() {
	return PMHthres;
}
int CompBand::getHthreshold() {
	return PHthres;
}
int CompBand::getCross1() {
	return Cross1;
}
int CompBand::getCross2() {
	return Cross2;
}
int CompBand::getCross3() {
	return Cross3;
}
int CompBand::getLevel() {
	return Plevel;
}
