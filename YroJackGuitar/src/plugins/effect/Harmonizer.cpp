/*
 rakarrack - a guitar effects software

 Harmonizer.C  -  Harmonizer
 Copyright (C) 2008 Josep Andreu
 Author:  Josep Andreu

 Using Stephan M. Bernsee smbPitchShifter engine.

 This program is free software; you can redistribute it and/or modify
 it under the terms of version 2 of the GNU General Public License
 as published by the Free Software Foundation.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License (version 2) for more details.

 You should have received a copy of the GNU General Public License
 (version2)  along with this program; if not, write to the Free Software
 Foundation,
 Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA

 */

#include <plugins/effect/Harmonizer.h>

using namespace std;

Harmonizer::Harmonizer(long int Quality, int DS, int uq, int dq) :
		YroEffectPlugin("Harmonizer", "Plain: 64,64,64,12,6000,0,0,0,64,64,0;"
				"Octavador: 64,64,64,0,6000,0,0,0,64,64,0;"
				"3mdown: 64,64,64,9,6000,0,0,0,64,64,0;") {
	hq = Quality;
	adjust(DS);

	templ = (float *) malloc(sizeof(float) * iPERIOD);
	tempr = (float *) malloc(sizeof(float) * iPERIOD);

	outi = (float *) malloc(sizeof(float) * nPERIOD);
	outo = (float *) malloc(sizeof(float) * nPERIOD);

	memset(outi, 0, sizeof(float) * nPERIOD);
	memset(outo, 0, sizeof(float) * nPERIOD);

	U_Resample = new Resample(dq);
	D_Resample = new Resample(uq);

	pl = new AnalogFilter(6, 22000, 1, 0);

	PS = new PitchShifter(window, hq, nfSAMPLE_RATE);
	PS->ratio = 1.0f;

	PMIDI = 0;
	mira = 0;
	setPreset(0);

	cleanup();

}
;

Harmonizer::~Harmonizer() {
}
;

void Harmonizer::cleanup() {
	mira = 0;
	memset(outi, 0, sizeof(float) * nPERIOD);
	memset(outo, 0, sizeof(float) * nPERIOD);
}
;

void Harmonizer::applyfilters(float * smps) {
	pl->filterout(iPERIOD, fPERIOD, smps);
}
;

void Harmonizer::render(jack_nframes_t nframes, float *smpsl, float *smpsr) {

	int i;

	if ((DS_state != 0) && (Pinterval != 12)) {
		memcpy(templ, smpsl, sizeof(float) * iPERIOD);
		memcpy(tempr, smpsr, sizeof(float) * iPERIOD);
		U_Resample->out(templ, tempr, smpsl, smpsr, iPERIOD, u_up);
	}

	for (i = 0; i < nPERIOD; i++) {
		outi[i] = (smpsl[i] + smpsr[i]) * .5;
		if (outi[i] > 1.0)
			outi[i] = 1.0f;
		if (outi[i] < -1.0)
			outi[i] = -1.0f;

	}
	/**
	 * TODO handle r__ratio
	 float r__ratio[12];
	 if ((PMIDI) || (PSELECT))
	 PS->ratio = r__ratio[0];
	 */

	if (Pinterval != 12) {
		PS->smbPitchShift(PS->ratio, nPERIOD, window, hq, nfSAMPLE_RATE, outi,
				outo);

		if ((DS_state != 0) && (Pinterval != 12)) {
			D_Resample->mono_out(outo, templ, nPERIOD, u_down, iPERIOD);
		} else {
			memcpy(templ, outo, sizeof(float) * iPERIOD);
		}

		applyfilters(templ);

		for (i = 0; i < iPERIOD; i++) {
			efxoutl[i] = templ[i] * gain * panning;
			efxoutr[i] = templ[i] * gain * (1.0f - panning);
		}

	}

}
;

void Harmonizer::setVolume(int value) {
	this->Pvolume = value;
	outvolume = (float) Pvolume / 127.0f;
}

void Harmonizer::setPanning(int value) {
	this->Ppan = value;
	panning = (float) Ppan / 127.0f;
}

void Harmonizer::setGain(int value) {
	this->Pgain = value;
	gain = (float) Pgain / 127.0f;
	gain *= 2.0;
}

void Harmonizer::setInterval(int value) {

	this->Pinterval = value;
	interval = (float) Pinterval - 12.0f;
	PS->ratio = powf(2.0f, interval / 12.0f);
	if (Pinterval % 12 == 0)
		mira = 0;
	else
		mira = 1;

}

void Harmonizer::setFfreq(int value) {
	fPfreq = value;
	float tmp = (float) value;
	pl->setfreq(tmp);
}

void Harmonizer::setFgain(int value) {

	float tmp;

	this->fPgain = value;
	tmp = 30.0f * ((float) value - 64.0f) / 64.0f;
	pl->setgain(tmp);

}

void Harmonizer::setQ(int value) {
	float tmp;
	this->fPq = value;
	tmp = powf(30.0f, ((float) value - 64.0f) / 64.0f);
	pl->setq(tmp);

}

void Harmonizer::setMidi(int value) {
	this->PMIDI = value;
}

void Harmonizer::adjust(int DS) {

	DS_state = DS;

	switch (DS) {

	case 0:
		nPERIOD = iPERIOD;
		nSAMPLE_RATE = iSAMPLE_RATE;
		nfSAMPLE_RATE = fSAMPLE_RATE;
		window = 2048;
		break;

	case 1:
		nPERIOD = lrintf(fPERIOD * 96000.0f / fSAMPLE_RATE);
		nSAMPLE_RATE = 96000;
		nfSAMPLE_RATE = 96000.0f;
		window = 2048;
		break;

	case 2:
		nPERIOD = lrintf(fPERIOD * 48000.0f / fSAMPLE_RATE);
		nSAMPLE_RATE = 48000;
		nfSAMPLE_RATE = 48000.0f;
		window = 2048;
		break;

	case 3:
		nPERIOD = lrintf(fPERIOD * 44100.0f / fSAMPLE_RATE);
		nSAMPLE_RATE = 44100;
		nfSAMPLE_RATE = 44100.0f;
		window = 2048;
		break;

	case 4:
		nPERIOD = lrintf(fPERIOD * 32000.0f / fSAMPLE_RATE);
		nSAMPLE_RATE = 32000;
		nfSAMPLE_RATE = 32000.0f;
		window = 2048;
		break;

	case 5:
		nPERIOD = lrintf(fPERIOD * 22050.0f / fSAMPLE_RATE);
		nSAMPLE_RATE = 22050;
		nfSAMPLE_RATE = 22050.0f;
		window = 1024;
		break;

	case 6:
		nPERIOD = lrintf(fPERIOD * 16000.0f / fSAMPLE_RATE);
		nSAMPLE_RATE = 16000;
		nfSAMPLE_RATE = 16000.0f;
		window = 1024;
		break;

	case 7:
		nPERIOD = lrintf(fPERIOD * 12000.0f / fSAMPLE_RATE);
		nSAMPLE_RATE = 12000;
		nfSAMPLE_RATE = 12000.0f;
		window = 512;
		break;

	case 8:
		nPERIOD = lrintf(fPERIOD * 8000.0f / fSAMPLE_RATE);
		nSAMPLE_RATE = 8000;
		nfSAMPLE_RATE = 8000.0f;
		window = 512;
		break;

	case 9:
		nPERIOD = lrintf(fPERIOD * 4000.0f / fSAMPLE_RATE);
		nSAMPLE_RATE = 4000;
		nfSAMPLE_RATE = 4000.0f;
		window = 256;
		break;
	}
	u_up = (double) nPERIOD / (double) iPERIOD;
	u_down = (double) iPERIOD / (double) nPERIOD;
}

void Harmonizer::setSelect(int value) {
	PSELECT = value;
}
void Harmonizer::setNote(int value) {
	Pnote = value;
}
void Harmonizer::setType(int value) {
	Ptype = value;
}

int Harmonizer::getVolume() {
	return Pvolume;
}
int Harmonizer::getPanning() {
	return Ppan;
}
int Harmonizer::getGain() {
	return Pgain;
}
int Harmonizer::getInterval() {
	return Pinterval;
}
int Harmonizer::getFfreq() {
	return fPfreq;
}
int Harmonizer::getSelect() {
	return PSELECT;
}
int Harmonizer::getNote() {
	return Pnote;
}
int Harmonizer::getType() {
	return Ptype;
}
int Harmonizer::getFgain() {
	return fPgain;
}
int Harmonizer::getQ() {
	return fPq;
}
int Harmonizer::getMidi() {
	return PMIDI;
}
/**
 * toXml member
 */
const char *Harmonizer::toXml() {
	char _buffer[256];
	char _formatd[] = { "<attribute name=\"%s\" value=\"%d\" />" };
	char _formatf[] = { "<attribute name=\"%s\" value=\"%9.40f\" />" };
	strcpy(_toXml, "<attributes>");
	sprintf(_buffer, _formatd, "DS_state", DS_state);
	strcat(_toXml, _buffer);
	sprintf(_buffer, _formatd, "fPfreq", fPfreq);
	strcat(_toXml, _buffer);
	sprintf(_buffer, _formatd, "fPgain", fPgain);
	strcat(_toXml, _buffer);
	sprintf(_buffer, _formatd, "fPq", fPq);
	strcat(_toXml, _buffer);
	sprintf(_buffer, _formatd, "hq", hq);
	strcat(_toXml, _buffer);
	sprintf(_buffer, _formatd, "mira", mira);
	strcat(_toXml, _buffer);
	sprintf(_buffer, _formatd, "nPERIOD", nPERIOD);
	strcat(_toXml, _buffer);
	sprintf(_buffer, _formatd, "nSAMPLE_RATE", nSAMPLE_RATE);
	strcat(_toXml, _buffer);
	sprintf(_buffer, _formatd, "Pgain", Pgain);
	strcat(_toXml, _buffer);
	sprintf(_buffer, _formatd, "Pinterval", Pinterval);
	strcat(_toXml, _buffer);
	sprintf(_buffer, _formatd, "PMIDI", PMIDI);
	strcat(_toXml, _buffer);
	sprintf(_buffer, _formatd, "Pnote", Pnote);
	strcat(_toXml, _buffer);
	sprintf(_buffer, _formatd, "Ppan", Ppan);
	strcat(_toXml, _buffer);
	sprintf(_buffer, _formatd, "PSELECT", PSELECT);
	strcat(_toXml, _buffer);
	sprintf(_buffer, _formatd, "Ptype", Ptype);
	strcat(_toXml, _buffer);
	sprintf(_buffer, _formatd, "Pvolume", Pvolume);
	strcat(_toXml, _buffer);
	sprintf(_buffer, _formatf, "gain", gain);
	strcat(_toXml, _buffer);
	sprintf(_buffer, _formatf, "interval", interval);
	strcat(_toXml, _buffer);
	sprintf(_buffer, _formatf, "nfSAMPLE_RATE", nfSAMPLE_RATE);
	strcat(_toXml, _buffer);
	sprintf(_buffer, _formatf, "panning", panning);
	strcat(_toXml, _buffer);
	strcat(_toXml, "</attributes>");
	return _toXml;
}
