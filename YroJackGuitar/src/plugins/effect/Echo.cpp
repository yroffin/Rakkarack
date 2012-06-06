/*
 ZynAddSubFX - a software synthesizer
 
 Echo.C - Echo effect
 Copyright (C) 2002-2005 Nasca Octavian Paul
 Author: Nasca Octavian Paul

 Modified for rakarrack by Josep Andreu
 Reverse Echo effect by Transmogrifox
 Echo Direct patch From: Arnout Engelen <arnouten@bzzt.net>

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

#include <plugins/effect/Echo.h>
using namespace std;

Echo::Echo() :
		YroEffectPlugin("Echo",
				"Echo 1:         67, 64,  565,  64,  30, 59,  0, 127, 0;"
						"Echo 2:         67, 64,  357,  64,  30, 59,  0,  64, 0;"
						"Echo 3:         67, 75,  955,  64,  30, 59, 10,   0, 0;"
						"Simple Echo:    67, 60,  705,  64,  30,  0,  0,   0, 0;"
						"Canyon:         67, 60, 1610,  50,  30, 82, 48,   0, 0;"
						"Panning Echo 1: 67, 64,  705,  17,   0, 82, 24,   0, 0;"
						"Panning Echo 2: 81, 60,  737, 118, 100, 68, 18,   0, 0;"
						"Panning Echo 3: 81, 60,  472, 100, 127, 67, 36,   0, 0;"
						"Feedback Echo:  62, 64,  456,  64, 100, 90, 55,   0, 0;") {
	//default values
	Pvolume = 50;
	Ppanning = 64;
	Pdelay = 60;
	Plrdelay = 100;
	Plrcross = 100;
	Pfb = 40;
	Phidamp = 60;

	ldelay = NULL;
	rdelay = NULL;
	lrdelay = 0;
	Srate_Attack_Coeff = 1.0f / (fSAMPLE_RATE * ATTACK);
	maxx_delay = iSAMPLE_RATE * MAX_DELAY;
	fade = iSAMPLE_RATE / 5; //1/5 SR fade time available

	ldelay = new float[maxx_delay];
	rdelay = new float[maxx_delay];

	setPreset(0);
	cleanup();
}
;

Echo::~Echo() {
}
;

/*
 * Cleanup the effect
 */
void Echo::cleanup() {
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
void Echo::initdelays() {
	int i;

	kl = 0;
	kr = 0;

	dl = delay - lrdelay;
	if (dl < 1)
		dl = 1;
	dr = delay + lrdelay;
	if (dr < 1)
		dr = 1;

	rvkl = dl - 1;
	rvkr = dr - 1;
	Srate_Attack_Coeff = 15.0f / (dl + dr); // Set swell time to 1/10th of average delay time

	for (i = dl; i < maxx_delay; i++)
		ldelay[i] = 0.0;
	for (i = dr; i < maxx_delay; i++)
		rdelay[i] = 0.0;

	oldl = 0.0;
	oldr = 0.0;

}
;

/*
 * Effect output
 */
void Echo::render(jack_nframes_t nframes, float * smpsl, float * smpsr) {
	int i;
	float l, r, ldl, rdl, ldlout, rdlout, rswell, lswell;

	for (i = 0; i < iPERIOD; i++) {
		ldl = ldelay[kl];
		rdl = rdelay[kr];
		l = ldl * (1.0f - lrcross) + rdl * lrcross;
		r = rdl * (1.0f - lrcross) + ldl * lrcross;
		ldl = l;
		rdl = r;

		ldlout = 0.0 - ldl * fb;
		rdlout = 0.0 - rdl * fb;
		if (!Pdirect) {
			ldlout = ldl = smpsl[i] * panning + ldlout;
			rdlout = rdl = smpsr[i] * (1.0f - panning) + rdlout;
		} else {
			ldl = smpsl[i] * panning + ldlout;
			rdl = smpsr[i] * (1.0f - panning) + rdlout;
		}

		if (reverse > 0.0) {

			lswell = (float) (abs(kl - rvkl)) * Srate_Attack_Coeff;
			if (lswell <= PI) {
				lswell = 0.5f * (1.0f - cosf(lswell)); //Clickless transition
				efxoutl[i] = reverse
						* (ldelay[rvkl] * lswell
								+ ldelay[rvfl] * (1.0f - lswell))
						+ (ldlout * (1 - reverse)); //Volume ducking near zero crossing.
			} else {
				efxoutl[i] = (ldelay[rvkl] * reverse)
						+ (ldlout * (1 - reverse));
			}

			rswell = (float) (abs(kr - rvkr)) * Srate_Attack_Coeff;
			if (rswell <= PI) {
				rswell = 0.5f * (1.0f - cosf(rswell)); //Clickless transition
				efxoutr[i] = reverse
						* (rdelay[rvkr] * rswell
								+ rdelay[rvfr] * (1.0f - rswell))
						+ (rdlout * (1 - reverse)); //Volume ducking near zero crossing.
			} else {
				efxoutr[i] = (rdelay[rvkr] * reverse)
						+ (rdlout * (1 - reverse));
			}

		} else {
			efxoutl[i] = ldlout;
			efxoutr[i] = rdlout;
		}

		//LowPass Filter
		ldelay[kl] = ldl = ldl * hidamp + oldl * (1.0f - hidamp);
		rdelay[kr] = rdl = rdl * hidamp + oldr * (1.0f - hidamp);
		oldl = ldl + DENORMAL_GUARD;
		oldr = rdl + DENORMAL_GUARD;

		if (++kl >= dl)
			kl = 0;
		if (++kr >= dr)
			kr = 0;
		rvkl = dl - 1 - kl;
		rvkr = dr - 1 - kr;

		rvfl = dl + fade - kl;
		rvfr = dr + fade - kr;
		//Safety checks to avoid addressing data outside of buffer
		if (rvfl > dl)
			rvfl = rvfl - dl;
		if (rvfl < 0)
			rvfl = 0;
		if (rvfr > dr)
			rvfr = rvfr - dr;
		if (rvfr < 0)
			rvfr = 0;
	};

}
;

/*
 * Parameter control
 */
void Echo::setVolume(int Pvolume) {
	this->Pvolume = Pvolume;
	outvolume = (float) Pvolume / 127.0f;
	if (Pvolume == 0)
		cleanup();

}
;

void Echo::setPanning(int Ppanning) {
	this->Ppanning = Ppanning;
	panning = ((float) Ppanning + 0.5f) / 127.0f;
}
;

void Echo::setReverse(int Preverse) {
	this->Preverse = Preverse;
	reverse = (float) Preverse / 127.0f;
}
;

void Echo::Tempo2Delay(int value) {
	Pdelay = 60.0f / (float) value * 1000.0f;
	delay = (float) Pdelay / 1000.0f * fSAMPLE_RATE;
	if ((unsigned int) delay > (iSAMPLE_RATE * MAX_DELAY))
		delay = iSAMPLE_RATE * MAX_DELAY;
	initdelays();
}

void Echo::setDelay(int Pdelay) {
	this->Pdelay = Pdelay;
	delay = Pdelay;
	if (delay < 10)
		delay = 10;
	if (delay > MAX_DELAY * 1000)
		delay = 1000 * MAX_DELAY; //Constrains 10ms ... MAX_DELAY
	delay = 1 + lrintf(((float) delay / 1000.0f) * fSAMPLE_RATE);
	initdelays();
}

void Echo::setLrdelay(int Plrdelay) {
	float tmp;
	this->Plrdelay = Plrdelay;
	tmp = (powf(2.0, fabsf((float) Plrdelay - 64.0f) / 64.0f * 9.0f) - 1.0f)
			/ 1000.0f * fSAMPLE_RATE;
	if (Plrdelay < 64.0)
		tmp = -tmp;
	lrdelay = lrintf(tmp);
	initdelays();
}

void Echo::setLrcross(int value) {
	this->Plrcross = value;
	lrcross = (float) value / 127.0f * 1.0f;
}
;

void Echo::setFb(int Pfb) {
	this->Pfb = Pfb;
	fb = (float) Pfb / 128.0f;
}
;

void Echo::setHidamp(int Phidamp) {
	this->Phidamp = Phidamp;
	hidamp = 1.0f - (float) Phidamp / 127.0f;
}
;

void Echo::setDirect(int Pdirect) {
	if (Pdirect > 0)
		Pdirect = 1;
	this->Pdirect = Pdirect;
}
;

int Echo::getVolume() {
	return Pvolume;
}
int Echo::getPanning() {
	return Ppanning;
}
int Echo::getDelay() {
	return Pdelay;
}
int Echo::getLrdelay() {
	return Plrdelay;
}
int Echo::getLrcross() {
	return Plrcross;
}
int Echo::getFb() {
	return Pfb;
}
int Echo::getHidamp() {
	return Phidamp;
}
int Echo::getReverse() {
	return Preverse;
}
int Echo::getDirect() {
	return Pdirect;
}

/**
 * toXml member
 */
const char *Echo::toXml() {
	char _buffer[256];
	char _formatd[] = { "<attribute name=\"%s\" value=\"%d\" />" };
	char _formatf[] = { "<attribute name=\"%s\" value=\"%9.40f\" />" };
	strcpy(_toXml, "<attributes>");
	sprintf(_buffer, _formatd, "dl", dl);
	strcat(_toXml, _buffer);
	sprintf(_buffer, _formatd, "dr", dr);
	strcat(_toXml, _buffer);
	sprintf(_buffer, _formatd, "delay", delay);
	strcat(_toXml, _buffer);
	sprintf(_buffer, _formatd, "lrdelay", lrdelay);
	strcat(_toXml, _buffer);
	sprintf(_buffer, _formatd, "kl", kl);
	strcat(_toXml, _buffer);
	sprintf(_buffer, _formatd, "kr", kr);
	strcat(_toXml, _buffer);
	sprintf(_buffer, _formatd, "rvkl", rvkl);
	strcat(_toXml, _buffer);
	sprintf(_buffer, _formatd, "rvkr", rvkr);
	strcat(_toXml, _buffer);
	sprintf(_buffer, _formatd, "rvfl", rvfl);
	strcat(_toXml, _buffer);
	sprintf(_buffer, _formatd, "rvfr", rvfr);
	strcat(_toXml, _buffer);
	sprintf(_buffer, _formatd, "maxx_delay", maxx_delay);
	strcat(_toXml, _buffer);
	sprintf(_buffer, _formatd, "fade", fade);
	strcat(_toXml, _buffer);
	sprintf(_buffer, _formatd, "Pdelay", Pdelay);
	strcat(_toXml, _buffer);
	sprintf(_buffer, _formatd, "Pdirect", Pdirect);
	strcat(_toXml, _buffer);
	sprintf(_buffer, _formatd, "Pfb", Pfb);
	strcat(_toXml, _buffer);
	sprintf(_buffer, _formatd, "Phidamp", Phidamp);
	strcat(_toXml, _buffer);
	sprintf(_buffer, _formatd, "Plrcross", Plrcross);
	strcat(_toXml, _buffer);
	sprintf(_buffer, _formatd, "Plrdelay", Plrdelay);
	strcat(_toXml, _buffer);
	sprintf(_buffer, _formatd, "Ppanning", Ppanning);
	strcat(_toXml, _buffer);
	sprintf(_buffer, _formatd, "Preverse", Preverse);
	strcat(_toXml, _buffer);
	sprintf(_buffer, _formatd, "Pvolume", Pvolume);
	strcat(_toXml, _buffer);
	sprintf(_buffer, _formatf, "oldl", oldl);
	strcat(_toXml, _buffer);
	sprintf(_buffer, _formatf, "oldr", oldr);
	strcat(_toXml, _buffer);
	sprintf(_buffer, _formatf, "outvolume", outvolume);
	strcat(_toXml, _buffer);
	sprintf(_buffer, _formatf, "panning", panning);
	strcat(_toXml, _buffer);
	sprintf(_buffer, _formatf, "lrcross", lrcross);
	strcat(_toXml, _buffer);
	sprintf(_buffer, _formatf, "fb", fb);
	strcat(_toXml, _buffer);
	sprintf(_buffer, _formatf, "hidamp", hidamp);
	strcat(_toXml, _buffer);
	sprintf(_buffer, _formatf, "reverse", reverse);
	strcat(_toXml, _buffer);
	sprintf(_buffer, _formatf, "Srate_Attack_Coeff", Srate_Attack_Coeff);
	strcat(_toXml, _buffer);
	strcat(_toXml, "</attributes>");
	return _toXml;
}
