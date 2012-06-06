/*
 rakarrack - a guitar effects software

 StereoHarm.C  -  Stereo Harmonizer
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

#include <plugins/effect/StereoHarm.h>
using namespace std;

StereoHarm::StereoHarm(long int Quality, int DS, int uq, int dq) :
		YroEffectPlugin("StereoHarm",
				"Plain: 64, 64, 12, 0, 64, 12, 0, 0, 0, 0, 0, 64;"
						"Octavador: 64, 64, 0, 0, 64, 0, 0, 0, 0, 0, 0, 64;"
						"Chorus: 64, 64, 12, 80, 64, 12, -80, 0, 0, 0, 0, 64;"
						"Chorus: 64, 64, 12, 280, 64, 12, -280, 0, 0, 0, 0, 64;") {
	hq = Quality;
	adjust(DS);

	templ = (float *) malloc(sizeof(float) * iPERIOD);
	tempr = (float *) malloc(sizeof(float) * iPERIOD);

	outil = (float *) malloc(sizeof(float) * nPERIOD);
	outir = (float *) malloc(sizeof(float) * nPERIOD);

	outol = (float *) malloc(sizeof(float) * nPERIOD);
	outor = (float *) malloc(sizeof(float) * nPERIOD);

	memset(outil, 0, sizeof(float) * nPERIOD);
	memset(outir, 0, sizeof(float) * nPERIOD);

	memset(outol, 0, sizeof(float) * nPERIOD);
	memset(outor, 0, sizeof(float) * nPERIOD);

	U_Resample = new Resample(dq);
	D_Resample = new Resample(uq);

	chromel = 0.0;
	chromer = 0.0;

	PSl = new PitchShifter(window, hq, nfSAMPLE_RATE);
	PSl->ratio = 1.0f;
	PSr = new PitchShifter(window, hq, nfSAMPLE_RATE);
	PSr->ratio = 1.0f;

	PMIDI = 0;
	mira = 0;
	setPreset(0);
	cleanup();
}

StereoHarm::~StereoHarm() {
}
;

void StereoHarm::cleanup() {
	mira = 0;
	chromel = 0;
	chromer = 0;
	memset(outil, 0, sizeof(float) * nPERIOD);
	memset(outir, 0, sizeof(float) * nPERIOD);
	memset(outol, 0, sizeof(float) * nPERIOD);
	memset(outor, 0, sizeof(float) * nPERIOD);

}
;

void StereoHarm::render(jack_nframes_t nframes, float *smpsl, float *smpsr) {

	int i;

	if (DS_state != 0) {
		memcpy(templ, smpsl, sizeof(float) * iPERIOD);
		memcpy(tempr, smpsr, sizeof(float) * iPERIOD);
		U_Resample->out(templ, tempr, smpsl, smpsr, iPERIOD, u_up);
	}

	for (i = 0; i < nPERIOD; i++) {

		outil[i] = smpsl[i] * (1.0f - lrcross) + smpsr[i] * lrcross;
		if (outil[i] > 1.0)
			outil[i] = 1.0f;
		if (outil[i] < -1.0)
			outil[i] = -1.0f;

		outir[i] = smpsr[i] * (1.0f - lrcross) + smpsl[i] * lrcross;
		if (outir[i] > 1.0)
			outir[i] = 1.0f;
		if (outir[i] < -1.0)
			outir[i] = -1.0f;

	}

	/**
	 * TODO
	 if ((PMIDI) || (PSELECT))
	 {
	 PSl->ratio = r__ratio[1];
	 PSr->ratio = r__ratio[2];
	 }
	 */

	if (PSl->ratio != 1.0f) {
		PSl->smbPitchShift(PSl->ratio, nPERIOD, window, hq, nfSAMPLE_RATE,
				outil, outol);
	} else
		memcpy(outol, outil, sizeof(float) * nPERIOD);

	if (PSr->ratio != 1.0f) {
		PSr->smbPitchShift(PSr->ratio, nPERIOD, window, hq, nfSAMPLE_RATE,
				outir, outor);
	} else
		memcpy(outor, outir, sizeof(float) * nPERIOD);

	if (DS_state != 0) {
		D_Resample->out(outol, outor, templ, tempr, nPERIOD, u_down);
	} else {
		memcpy(templ, outol, sizeof(float) * iPERIOD);
		memcpy(tempr, outor, sizeof(float) * iPERIOD);

	}

	for (i = 0; i < iPERIOD; i++) {
		efxoutl[i] = templ[i] * gainl;
		efxoutr[i] = tempr[i] * gainr;
	}

}
;

void StereoHarm::setVolume(int value) {
	this->Pvolume = value;
	outvolume = (float) Pvolume / 127.0f;
}
;

void StereoHarm::setGain(int chan, int value) {

	switch (chan) {
	case 0:
		Pgainl = value;
		gainl = (float) Pgainl / 127.0f;
		gainl *= 2.0;
		break;
	case 1:
		Pgainr = value;
		gainr = (float) Pgainr / 127.0f;
		gainr *= 2.0;
		break;

	}
}
;

void StereoHarm::setInterval(int chan, int value) {

	switch (chan) {
	case 0:
		Pintervall = value;
		intervall = (float) Pintervall - 12.0f;
		PSl->ratio = powf(2.0f, intervall / 12.0f) + chromel;
		if (Pintervall % 12 == 0)
			mira = 0;
		else
			mira = 1;
		break;

	case 1:
		Pintervalr = value;
		intervalr = (float) Pintervalr - 12.0f;
		PSr->ratio = powf(2.0f, intervalr / 12.0f) + chromer;
		if (Pintervalr % 12 == 0)
			mira = 0;
		else
			mira = 1;
		break;
	}
}
;

void StereoHarm::setChrome(int chan, int value) {

	float max, min;
	max = 0.0;
	min = 0.0;

	switch (chan) {
	case 0:
		max = powf(2.0f, (intervall + 1.0f) / 12.0f);
		min = powf(2.0f, (intervall - 1.0f) / 12.0f);
		break;
	case 1:
		max = powf(2.0f, (intervalr + 1.0f) / 12.0f);
		min = powf(2.0f, (intervalr - 1.0f) / 12.0f);
		break;
	}

	if (max > 2.0)
		max = 2.0f;
	if (min < 0.5)
		min = 0.5f;

	switch (chan) {
	case 0:
		Pchromel = value;
		chromel = (max - min) / 4000.0f * (float) value;
		PSl->ratio = powf(2.0f, intervall / 12.0f) + chromel;
		break;
	case 1:
		Pchromer = value;
		chromer = (max - min) / 4000.0f * (float) value;
		PSr->ratio = powf(2.0f, intervalr / 12.0f) + chromer;
		break;
	}

}

void StereoHarm::setMIDI(int value) {

	this->PMIDI = value;
}

void StereoHarm::adjust(int DS) {

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

void StereoHarm::setLrcross(int value) {
	Plrcross = value;
	lrcross = (float) Plrcross / 127.0f;

}
;

void StereoHarm::setSelect(int value) {
	PSELECT = value;
}
void StereoHarm::setNote(int value) {
	Pnote = value;
}
void StereoHarm::setType(int value) {
	Ptype = value;
	if (Ptype == 0) {
		setChrome(0, Pchromel);
		setChrome(1, Pchromer);
	}
}

int StereoHarm::getVolume() {
	return Pvolume;
}
int StereoHarm::getGain(int sel) {
	if(sel == 0) return Pgainl;
	else return Pgainr;
}
int StereoHarm::getInterval(int sel) {
	if(sel == 0) return Pintervalr;
	else return Pintervall;
}
int StereoHarm::getChrome(int sel) {
	if(sel == 0) return Pchromer;
	else return Pchromel;
}
int StereoHarm::getSelect() {
	return PSELECT;
}
int StereoHarm::getNote() {
	return Pnote;
}
int StereoHarm::getType() {
	return Ptype;
}
int StereoHarm::getMIDI() {
	return PMIDI;
}
int StereoHarm::getLrcross() {
	return Plrcross;
}

/**
 * toXml member
*/
const char *StereoHarm::toXml() {
        char _buffer[256];
        char _formatd[] = {"<attribute name=\"%s\" value=\"%d\" />"};
        char _formatf[] = {"<attribute name=\"%s\" value=\"%9.40f\" />"};
        strcpy(_toXml,"<attributes>");
        sprintf(_buffer,_formatd,"DS_state",DS_state);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatd,"hq",hq);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatd,"mira",mira);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatd,"nPERIOD",nPERIOD);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatd,"nSAMPLE_RATE",nSAMPLE_RATE);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatd,"Pchromel",Pchromel);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatd,"Pchromer",Pchromer);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatd,"Pgainl",Pgainl);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatd,"Pgainr",Pgainr);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatd,"Pintervall",Pintervall);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatd,"Pintervalr",Pintervalr);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatd,"Plrcross",Plrcross);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatd,"PMIDI",PMIDI);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatd,"Pnote",Pnote);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatd,"PSELECT",PSELECT);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatd,"Ptype",Ptype);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatd,"Pvolume",Pvolume);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatf,"chromel",chromel);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatf,"chromer",chromer);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatf,"gainl",gainl);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatf,"gainr",gainr);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatf,"intervall",intervall);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatf,"intervalr",intervalr);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatf,"lrcross",lrcross);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatf,"nfSAMPLE_RATE",nfSAMPLE_RATE);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatf,"outvolume",outvolume);
        strcat(_toXml,_buffer);
        strcat(_toXml,"</attributes>");
        return _toXml;
}
