/*
 rakarrack - a guitar effects software

 Shifter.C  -  Shifter
 Copyright (C) 2008-2010 Josep Andreu
 Author: Josep Andreu

 Using Stephan M. Bernsee smbPtichShifter engine.

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

#include <plugins/effect/Shifter.h>
using namespace std;

Shifter::Shifter(long int Quality, int DS, int uq, int dq) :
		YroEffectPlugin("Shifter", "Fast: 0, 64, 64, 200, 200, -20, 2, 0, 0, 0;"
				"Slowup: 0, 64, 64, 900, 200, -20, 2, 0, 0, 0;"
				"Slowdown: 0, 64, 64, 900, 200, -20, 3, 1, 0, 0;"
				"Chorus: 64, 64, 64, 0, 0, -20, 1, 0, 1, 22;"
				"Trig Chorus: 64, 64, 64, 250, 100, -10, 0, 0, 0, 25;") {
	hq = Quality;
	adjust(DS);

	templ = (float *) malloc(sizeof(float) * iPERIOD);
	tempr = (float *) malloc(sizeof(float) * iPERIOD);

	outi = (float *) malloc(sizeof(float) * nPERIOD);
	outo = (float *) malloc(sizeof(float) * nPERIOD);

	U_Resample = new Resample(dq);
	D_Resample = new Resample(uq);

	PS = new PitchShifter(window, hq, nfSAMPLE_RATE);
	PS->ratio = 1.0f;

	state = IDLE;
	env = 0.0f;
	tune = 0.0f;
	Pupdown = 0;
	Pinterval = 0;
	setPreset(0);
	cleanup();
}

Shifter::~Shifter() {
}
;

void Shifter::cleanup() {
	state = IDLE;
	memset(outi, 0, sizeof(float) * nPERIOD);
	memset(outo, 0, sizeof(float) * nPERIOD);
}
;

void Shifter::adjust(int DS) {

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

void Shifter::render(jack_nframes_t nframes, float *smpsl, float *smpsr) {

	int i;
	float sum;
	float use;

	if (DS_state != 0) {
		memcpy(templ, smpsl, sizeof(float) * iPERIOD);
		memcpy(tempr, smpsr, sizeof(float) * iPERIOD);
		U_Resample->out(templ, tempr, smpsl, smpsr, iPERIOD, u_up);
	}

	for (i = 0; i < nPERIOD; i++) {
		if (Pmode == 0) {
			sum = fabsf(smpsl[i]) + fabsf(smpsr[i]);
			if (sum > env)
				env = sum;
			else
				env = sum * ENV_TR + env * (1.0f - ENV_TR);

			if (env <= tz_level) {
				state = IDLE;
				tune = 0.0;
			}

			if ((state == IDLE) && (env >= t_level))
				state = UP;

			if (state == UP) {
				tune += a_rate;
				if (tune >= 1.0f)
					state = WAIT;
			}

			if (state == WAIT) {
				if (env < td_level)
					state = DOWN;
			}

			if (state == DOWN) {
				tune -= d_rate;
				if (tune <= 0.0) {
					tune = 0.0;
					state = IDLE;
				}
			}
		}
		outi[i] = (smpsl[i] + smpsr[i]) * .5;
		if (outi[i] > 1.0)
			outi[i] = 1.0f;
		if (outi[i] < -1.0)
			outi[i] = -1.0f;

	}

	if (Pmode == 1)
		use = whammy;
	else
		use = tune;
	if ((Pmode == 0) && (Pinterval == 0))
		use = tune * whammy;

	if (Pupdown)
		PS->ratio = 1.0f - (1.0f - range) * use;
	else
		PS->ratio = 1.0f + ((range - 1.0f) * use);

	PS->smbPitchShift(PS->ratio, nPERIOD, window, hq, nfSAMPLE_RATE, outi,
			outo);

	for (i = 0; i < nPERIOD; i++) {
		templ[i] = outo[i] * gain * panning;
		tempr[i] = outo[i] * gain * (1.0f - panning);
	}

	if (DS_state != 0) {
		D_Resample->out(templ, tempr, efxoutl, efxoutr, nPERIOD, u_down);

	} else {
		memcpy(efxoutl, templ, sizeof(float) * iPERIOD);
		memcpy(efxoutr, tempr, sizeof(float) * iPERIOD);
	}

}
;

void Shifter::setVolume(int value) {
	this->Pvolume = value;
	outvolume = (float) Pvolume / 127.0f;
}
;

void Shifter::setPanning(int value) {
	this->Ppan = value;
	panning = (float) Ppan / 127.0f;
}
;

void Shifter::setGain(int value) {
	this->Pgain = value;
	gain = (float) Pgain / 127.0f;
	gain *= 2.0f;
}
;

void Shifter::fixinterval(int value) {
	interval = (float) value;
	if ((Pmode == 0) && (Pinterval == 0))
		interval = 1.0f;
	if (Pupdown)
		interval *= -1.0f;
	range = powf(2.0f, interval / 12.0f);

}
;

void Shifter::setAttack(int value) {
	Pattack = value;
	a_rate = 1000.0f / ((float) Pattack * nfSAMPLE_RATE);
}
void Shifter::setDecay(int value) {
	Pdecay = value;
	d_rate = 1000.0f / ((float) Pdecay * nfSAMPLE_RATE);
}
void Shifter::setThreshold(int value) {
	Pthreshold = value;
	t_level = dB2rap ((float)Pthreshold);
	td_level = t_level * .75f;
	tz_level = t_level * .5f;
}
void Shifter::setInterval(int value) {
	Pinterval = value;
	fixinterval(Pinterval);
}
void Shifter::setUpdown(int value) {
	Pupdown = value;
	fixinterval(Pinterval);
}
void Shifter::setMode(int value) {
	Pmode = value;
}
void Shifter::setWhammy(int value) {
Pwhammy = value;
whammy = (float) value / 127.0f;
}
int Shifter::getVolume() {
	return Pvolume;
}
int Shifter::getPanning() {
	return Ppan;
}
int Shifter::getGain() {
	return Pgain;
}
int Shifter::getAttack() {
	return Pattack;
}
int Shifter::getDecay() {
	return Pdecay;
}
int Shifter::getThreshold() {
	return Pthreshold;
}
int Shifter::getInterval() {
	return Pinterval;
}
int Shifter::getUpdown() {
	return Pupdown;
}
int Shifter::getMode() {
	return Pmode;
}
int Shifter::getWhammy() {
	return Pwhammy;
}
/**
 * toXml member
*/
const char *Shifter::toXml() {
        char _buffer[256];
        char _formatd[] = {"<attribute name=\"%s\" value=\"%d\" />"};
        char _formatf[] = {"<attribute name=\"%s\" value=\"%9.40f\" />"};
        strcpy(_toXml,"<attributes>");
        sprintf(_buffer,_formatd,"DS_state",DS_state);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatd,"hq",hq);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatd,"nPERIOD",nPERIOD);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatd,"nSAMPLE_RATE",nSAMPLE_RATE);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatd,"Pattack",Pattack);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatd,"Pdecay",Pdecay);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatd,"Pgain",Pgain);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatd,"Pinterval",Pinterval);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatd,"Pmode",Pmode);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatd,"Ppan",Ppan);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatd,"Pthreshold",Pthreshold);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatd,"Pupdown",Pupdown);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatd,"Pvolume",Pvolume);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatd,"Pwhammy",Pwhammy);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatd,"state",state);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatf,"a_rate",a_rate);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatf,"d_rate",d_rate);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatf,"tune",tune);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatf,"range",range);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatf,"whammy",whammy);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatf,"env",env);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatf,"t_level",t_level);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatf,"td_level",td_level);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatf,"tz_level",tz_level);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatf,"gain",gain);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatf,"interval",interval);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatf,"nfSAMPLE_RATE",nfSAMPLE_RATE);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatf,"outvolume",outvolume);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatf,"panning",panning);
        strcat(_toXml,_buffer);
        strcat(_toXml,"</attributes>");
        return _toXml;
}
