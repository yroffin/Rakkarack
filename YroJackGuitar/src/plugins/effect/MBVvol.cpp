/*
 MBVvol.C - Vary Band Volumen effect

 ZynAddSubFX - a software synthesizer
 Copyright (C) 2002-2005 Nasca Octavian Paul
 Author: Nasca Octavian Paul

 Modified for rakarrack by Josep Andreu

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

#include <plugins/effect/MBVvol.h>
using namespace std;

/*
 * Waveshape (this is called by OscilGen::waveshape and Distorsion::process)
 */

MBVvol::MBVvol() :
		YroEffectPlugin("MBVvol", "Vary1: 0,40,0,64,80,0,0,500,2500,5000,0;"
				"Vary2: 0,80,0,64,40,0,0,120,600,2300,1;"
				"Vary3: 0,120,0,64,40,0,0,800,2300,5200,2;") {

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

	//default values
	Pvolume = 50;
	coeff = 1.0 / (float) iPERIOD;
	volL = volLr = volML = volMLr = volMH = volMHr = volH = volHr = 2.0f;
	v1l = 0.;
	v1r = 0.;
	v2l = 0.;
	v2r = 0.;
	d1 = 0.;
	d2 = 0.;
	d3 = 0.;
	d4 = 0.;
	setPreset(0);
	cleanup();
}

MBVvol::~MBVvol() {
}
;

/*
 * Cleanup the effect
 */
void MBVvol::cleanup() {
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
}

/*
 * Effect output
 */
void MBVvol::render(jack_nframes_t nframes, float * smpsl, float * smpsr) {
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

	lfo1.render(1, &lfo1l, &lfo1r);
	lfo2.render(1, &lfo2l, &lfo2r);

	d1 = (lfo1l - v1l) * coeff;
	d2 = (lfo1r - v1r) * coeff;
	d3 = (lfo2l - v2l) * coeff;
	d4 = (lfo2r - v2r) * coeff;

	for (i = 0; i < iPERIOD; i++) {
		compute(Pcombi);
		efxoutl[i] = lowl[i] * volL + midll[i] * volML + midhl[i] * volMH
				+ highl[i] * volH;
		efxoutr[i] = lowr[i] * volLr + midlr[i] * volMLr + midhr[i] * volMHr
				+ highr[i] * volHr;
	}

}
;

/*
 * Parameter control
 */
void MBVvol::setVolume(int value) {
	Pvolume = value;
	outvolume = (float) Pvolume / 127.0f;
}

void MBVvol::setCombi(int value) {
	Pcombi = value;
}

void MBVvol::compute(int value) {
	Pcombi = value;
	v1l += d1;
	v1r += d2;
	v2l += d3;
	v2r += d4;

	switch (value) {
	case 0:
		volL = v1l;
		volLr = v1r;
		volML = v1l;
		volMLr = v1r;
		volMH = v2l;
		volMHr = v2r;
		volH = v2l;
		volHr = v2r;
		break;
	case 1:
		volL = v1l;
		volLr = v1r;
		volML = v2l;
		volMLr = v2r;
		volMH = v2l;
		volMHr = v2r;
		volH = v1l;
		volHr = v1r;
		break;
	case 2:
		volL = v1l;
		volLr = v1r;
		volML = v2l;
		volMLr = v2r;
		volMH = v1l;
		volMHr = v1r;
		volH = v2l;
		volHr = v2r;
		break;
	case 3:
		volL = 1.0f;
		volLr = 1.0f;
		volML = v1l;
		volMLr = v1r;
		volMH = v1l;
		volMHr = v1r;
		volH = 1.0f;
		volHr = 1.0f;
		break;
	case 4:
		volL = 1.0f;
		volLr = 1.0f;
		volML = v1l;
		volMLr = v1r;
		volMH = v2l;
		volMHr = v2r;
		volH = 1.0f;
		volHr = 1.0f;
		break;
	case 5:
		volL = 0.0f;
		volLr = 0.0f;
		volML = v1l;
		volMLr = v1r;
		volMH = v1l;
		volMHr = v1r;
		volH = 0.0f;
		volHr = 0.0f;
		break;
	case 6:
		volL = 0.0f;
		volLr = 0.0f;
		volML = v1l;
		volMLr = v1r;
		volMH = v2l;
		volMHr = v2r;
		volH = 0.0f;
		volHr = 0.0f;
		break;
	case 7:
		volL = v1l;
		volLr = v1r;
		volML = 1.0f;
		volMLr = 1.0f;
		volMH = 1.0f;
		volMHr = 1.0f;
		volH = v1l;
		volHr = v1r;
		break;
	case 8:
		volL = v1l;
		volLr = v1r;
		volML = 1.0f;
		volMLr = 1.0f;
		volMH = 1.0f;
		volMHr = 1.0f;
		volH = v2l;
		volHr = v2r;
		break;
	case 9:
		volL = v1l;
		volLr = v1r;
		volML = 0.0f;
		volMLr = 0.0f;
		volMH = 0.0f;
		volMHr = 0.0f;
		volH = v1l;
		volHr = v1r;
		break;
	case 10:
		volL = v1l;
		volLr = v1r;
		volML = 0.0f;
		volMLr = 0.0f;
		volMH = 0.0f;
		volMHr = 0.0f;
		volH = v2l;
		volHr = v2r;
		break;

	}

}

void MBVvol::setCross1(int value) {
	Cross1 = value;
	lpf1l->setfreq((float) value);
	lpf1r->setfreq((float) value);
	hpf1l->setfreq((float) value);
	hpf1r->setfreq((float) value);

}
;

void MBVvol::setCross2(int value) {
	Cross2 = value;
	hpf2l->setfreq((float) value);
	hpf2r->setfreq((float) value);
	lpf2l->setfreq((float) value);
	lpf2r->setfreq((float) value);

}
;

void MBVvol::setCross3(int value) {
	Cross3 = value;
	hpf3l->setfreq((float) value);
	hpf3r->setfreq((float) value);
	lpf3l->setfreq((float) value);
	lpf3r->setfreq((float) value);

}
;

void MBVvol::setLfoFreq1(int value) {
	lfo1.setPfreq(value);
}
void MBVvol::setLfoType1(int value) {
	lfo1.setPlfOtype(value);
}
void MBVvol::setLfoStereo1(int value) {
	lfo1.setPstereo(value);
}
void MBVvol::setLfoFreq2(int value) {
	lfo2.setPfreq(value);
}
void MBVvol::setLfoType2(int value) {
	lfo2.setPlfOtype(value);
}
void MBVvol::setLfoStereo2(int value) {
	lfo2.setPstereo(value);
}

int MBVvol::getVolume() {
	return Pvolume;
}
int MBVvol::getLfoFreq1() {
	return lfo1.getPfreq();
}
int MBVvol::getLfoType1() {
	return lfo1.getPlfOtype();
}
int MBVvol::getLfoStereo1() {
	return lfo1.getPstereo();
}
int MBVvol::getLfoFreq2() {
	return lfo2.getPfreq();
}
int MBVvol::getLfoType2() {
	return lfo2.getPlfOtype();
}
int MBVvol::getLfoStereo2() {
	return lfo2.getPstereo();
}
int MBVvol::getCross1() {
	return Cross1;
}
int MBVvol::getCross2() {
	return Cross2;
}
int MBVvol::getCross3() {
	return Cross3;
}
int MBVvol::getCombi() {
	return Pcombi;
}
/**
 * toXml member
 */
const char *MBVvol::toXml() {
	char _buffer[256];
	char _formatd[] = { "<attribute name=\"%s\" value=\"%d\" />" };
	char _formatf[] = { "<attribute name=\"%s\" value=\"%9.40f\" />" };
	strcpy(_toXml, "<attributes>");
	sprintf(_buffer, _formatd, "Cross1", Cross1);
	strcat(_toXml, _buffer);
	sprintf(_buffer, _formatd, "Cross2", Cross2);
	strcat(_toXml, _buffer);
	sprintf(_buffer, _formatd, "Cross3", Cross3);
	strcat(_toXml, _buffer);
	sprintf(_buffer, _formatd, "Pcombi", Pcombi);
	strcat(_toXml, _buffer);
	sprintf(_buffer, _formatd, "Pvolume", Pvolume);
	strcat(_toXml, _buffer);
	sprintf(_buffer, _formatf, "coeff", coeff);
	strcat(_toXml, _buffer);
	sprintf(_buffer, _formatf, "d1", d1);
	strcat(_toXml, _buffer);
	sprintf(_buffer, _formatf, "d2", d2);
	strcat(_toXml, _buffer);
	sprintf(_buffer, _formatf, "d3", d3);
	strcat(_toXml, _buffer);
	sprintf(_buffer, _formatf, "d4", d4);
	strcat(_toXml, _buffer);
	sprintf(_buffer, _formatf, "lfo1l", lfo1l);
	strcat(_toXml, _buffer);
	sprintf(_buffer, _formatf, "lfo1r", lfo1r);
	strcat(_toXml, _buffer);
	sprintf(_buffer, _formatf, "lfo2l", lfo2l);
	strcat(_toXml, _buffer);
	sprintf(_buffer, _formatf, "lfo2r", lfo2r);
	strcat(_toXml, _buffer);
	sprintf(_buffer, _formatf, "v1l", v1l);
	strcat(_toXml, _buffer);
	sprintf(_buffer, _formatf, "v1r", v1r);
	strcat(_toXml, _buffer);
	sprintf(_buffer, _formatf, "v2l", v2l);
	strcat(_toXml, _buffer);
	sprintf(_buffer, _formatf, "v2r", v2r);
	strcat(_toXml, _buffer);
	sprintf(_buffer, _formatf, "volLr", volLr);
	strcat(_toXml, _buffer);
	sprintf(_buffer, _formatf, "volMLr", volMLr);
	strcat(_toXml, _buffer);
	sprintf(_buffer, _formatf, "volMHr", volMHr);
	strcat(_toXml, _buffer);
	sprintf(_buffer, _formatf, "volHr", volHr);
	strcat(_toXml, _buffer);
	sprintf(_buffer, _formatf, "volL", volL);
	strcat(_toXml, _buffer);
	sprintf(_buffer, _formatf, "volML", volML);
	strcat(_toXml, _buffer);
	sprintf(_buffer, _formatf, "volMH", volMH);
	strcat(_toXml, _buffer);
	sprintf(_buffer, _formatf, "volH", volH);
	strcat(_toXml, _buffer);
	strcat(_toXml, "</attributes>");
	return _toXml;
}
