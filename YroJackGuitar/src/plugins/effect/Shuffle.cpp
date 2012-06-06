/*
 Shuffle.C - Distorsion effect

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

#include <plugins/effect/Shuffle.h>

using namespace std;

/*
 * Waveshape (this is called by OscilGen::waveshape and Distorsion::process)
 */

Shuffle::Shuffle() :
		YroEffectPlugin("Shuffle",
				"Shuffle 1: 64, 10, 0, 0, 0, 600, 1200, 2000, 6000, -14, 1;"
						"Shuffle 2: 64, 0, 0, 0, 0, 120, 1000, 2400, 8000, -7, 1;"
						"Shuffle 3: 64, 0, 0, 0, 0, 60, 1800, 3700, 12000, 7, 0;"
						"Remover: 0, 17, 0, 7, 5, 600, 1200, 2000, 13865, -45, 1;") {

	inputl = (float *) malloc(sizeof(float) * iPERIOD);
	inputr = (float *) malloc(sizeof(float) * iPERIOD);

	lr = new AnalogFilter(6, 300.0f, .3f, 0);
	hr = new AnalogFilter(6, 8000.0f, .3f, 0);
	mlr = new AnalogFilter(6, 1200.0f, .3f, 0);
	mhr = new AnalogFilter(6, 2400.0f, .3f, 0);

	//default values
	Pvolume = 50;
	PvolL = 0;
	PvolML = 0;
	PvolMH = 0;
	PvolH = 0;
	E = 0;
	setPreset(0);
	cleanup();
}
;

Shuffle::~Shuffle() {
}
;

/*
 * Cleanup the effect
 */
void Shuffle::cleanup() {
	lr->cleanup();
	hr->cleanup();
	mlr->cleanup();
	mhr->cleanup();

}
;
/*
 * Effect output
 */
void Shuffle::render(jack_nframes_t nframes, float * smpsl, float * smpsr) {
	int i;

	for (i = 0; i < iPERIOD; i++) {

		inputl[i] = smpsl[i] + smpsr[i];
		inputr[i] = smpsl[i] - smpsr[i];
	}

	if (E) {

		lr->filterout(iPERIOD, fPERIOD, inputr);
		mlr->filterout(iPERIOD, fPERIOD, inputr);
		mhr->filterout(iPERIOD, fPERIOD, inputr);
		hr->filterout(iPERIOD, fPERIOD, inputr);
	} else {
		lr->filterout(iPERIOD, fPERIOD, inputl);
		mlr->filterout(iPERIOD, fPERIOD, inputl);
		mhr->filterout(iPERIOD, fPERIOD, inputl);
		hr->filterout(iPERIOD, fPERIOD, inputl);
	}

	for (i = 0; i < iPERIOD; i++) {
		efxoutl[i] = (inputl[i] + inputr[i] - smpsl[i]) * .333333f;
		efxoutr[i] = (inputl[i] - inputr[i] - smpsr[i]) * .333333f;

	}

}
;

/*
 * Parameter control
 */
void Shuffle::setVolume(int value) {
	Pvolume = value;
	outvolume = (float) Pvolume / 128.0f;
}
;

void Shuffle::setCross1(int value) {
	Cross1 = value;
	lr->setfreq((float) value);

}
;

void Shuffle::setCross2(int value) {
	Cross2 = value;
	mlr->setfreq((float) value);

}
;

void Shuffle::setCross3(int value) {
	Cross3 = value;
	mhr->setfreq((float) value);

}
;

void Shuffle::setCross4(int value) {
	Cross4 = value;
	hr->setfreq((float) value);

}
;

void Shuffle::setGainL(int value) {
	PvolL = value + 64;
	volL = 30.0f * ((float) PvolL - 64.0f) / 64.0f;
	lr->setgain(volL);
}

void Shuffle::setGainML(int value) {
	PvolML = value + 64;
	volML = 30.0f * ((float) PvolML - 64.0f) / 64.0f;
	;
	mlr->setgain(volML);
}

void Shuffle::setGainMH(int value) {
	PvolMH = value + 64;
	volMH = 30.0f * ((float) PvolMH - 64.0f) / 64.0f;
	;
	mhr->setgain(volMH);
}

void Shuffle::setGainH(int value) {
	PvolH = value + 64;
	volH = 30.0f * ((float) PvolH - 64.0f) / 64.0f;
	;
	hr->setgain(volH);
}

void Shuffle::setPq(int value) {
	PQ = value;
	value += 64;
	tmp = powf(30.0f, ((float) value - 64.0f) / 64.0f);
	lr->setq(tmp);
	mlr->setq(tmp);
	mhr->setq(tmp);
	hr->setq(tmp);
}
void Shuffle::setE(int value) {
	E = value;
}

int Shuffle::getVolume() {
	return Pvolume;
}
int Shuffle::getGainL() {
	return PvolL;
}
int Shuffle::getGainML() {
	return PvolML;
}
int Shuffle::getGainMH() {
	return PvolMH;
}
int Shuffle::getGainH() {
	return PvolH;
}
int Shuffle::getCross1() {
	return Cross1;
}
int Shuffle::getCross2() {
	return Cross2;
}
int Shuffle::getCross3() {
	return Cross3;
}
int Shuffle::getCross4() {
	return Cross4;
}
int Shuffle::getPq() {
	return PQ;
}
int Shuffle::getE() {
	return E;
}
/**
 * toXml member
*/
const char *Shuffle::toXml() {
        char _buffer[256];
        char _formatd[] = {"<attribute name=\"%s\" value=\"%d\" />"};
        char _formatf[] = {"<attribute name=\"%s\" value=\"%9.40f\" />"};
        strcpy(_toXml,"<attributes>");
        sprintf(_buffer,_formatd,"Cross1",Cross1);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatd,"Cross2",Cross2);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatd,"Cross3",Cross3);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatd,"Cross4",Cross4);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatd,"E",E);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatd,"PQ",PQ);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatd,"PvolH",PvolH);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatd,"PvolL",PvolL);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatd,"PvolMH",PvolMH);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatd,"PvolML",PvolML);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatd,"Pvolume",Pvolume);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatf,"outvolume",outvolume);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatf,"tmp",tmp);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatf,"volL",volL);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatf,"volML",volML);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatf,"volMH",volMH);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatf,"volH",volH);
        strcat(_toXml,_buffer);
        strcat(_toXml,"</attributes>");
        return _toXml;
}
