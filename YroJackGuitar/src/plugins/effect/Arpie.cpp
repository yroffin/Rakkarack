/*
 Arpie.C - Arpeggiated Echo effect
 Copyright (C) 2002-2005 Nasca Octavian Paul
 Author: Nasca Octavian Paul

 Modified for rakarrack by Josep Andreu
 Arpeggio Echo effect by Transmogrifox
 Modified by Yannick Roffin

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

#include <plugins/effect/Arpie.h>
using namespace std;

Arpie::Arpie() :
		YroEffectPlugin("Arpie", "Arpie 1: 67, 64, 35, 64, 30, 59, 0, 127, 4;"
				"Arpie 2: 67, 64, 21, 64, 30, 59, 0, 64, 4;"
				"Arpie 3: 67, 75, 60, 64, 30, 59, 10, 0, 4;"
				"Simple Arpie: 67, 60, 44, 64, 30, 0, 0, 0, 4;"
				"Canyon: 67, 60, 102, 50, 30, 82, 48, 0, 4;"
				"Panning Arpie 1: 67, 64, 44, 17, 0, 82, 24, 0, 4;"
				"Panning Arpie 2: 81, 60, 46, 118, 100, 68, 18, 0, 4;"
				"Panning Arpie 3: 81, 60, 26, 100, 127, 67, 36, 0, 4;"
				"Feedback Arpie: 62, 64, 28, 64, 100, 90, 55, 0, 4;") {

	//default values
	Pvolume = 50;
	Ppanning = 64;
	Pdelay = 60;
	Plrdelay = 100;
	Plrcross = 100;
	Pfb = 40;
	Phidamp = 60;
	Pharms = 3;
	Psubdiv = 1;

	ldelay = NULL;
	rdelay = NULL;
	lrdelay = 0;
	harmonic = 1;
	Srate_Attack_Coeff = 1.0f / (fSAMPLE_RATE * ATTACK);
	invattack = iSAMPLE_RATE / 15;
	envattack = 1.0f / (float) invattack;
	maxx_delay = iSAMPLE_RATE * MAX_DELAY;
	fade = iSAMPLE_RATE / 10; //200ms fade time available

	ldelay = new float[maxx_delay];
	rdelay = new float[maxx_delay];
	pattern = new int[MAXHARMS];

	setPreset(0);
	setPattern(0);
	cleanup();
}
;

Arpie::~Arpie() {
}
;

/*
 * Cleanup the effect
 */
void Arpie::cleanup() {
	memset(ldelay, 0, sizeof(float) * maxx_delay);
	memset(rdelay, 0, sizeof(float) * maxx_delay);
	oldl = 0.0;
	oldr = 0.0;
	rvkl = 0;
	rvkr = 0;
	kl = 0;
	kr = 0;
	harmonic = 1;
}
;

/*
 * Initialize the delays
 */
void Arpie::initdelays() {
	int i;

	kl = 0;
	kr = 0;

	dl = delay - lrdelay;
	if (dl < 1)
		dl = 1;
	dr = delay + lrdelay;
	if (dr < 1)
		dr = 1;

	rvkl = 0;
	rvkr = 0;
	Srate_Attack_Coeff = 15.0f / (dl + dr); // Set swell time to 1/10th of average delay time
	fade = (dl + dr) / 5;

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
void Arpie::render(jack_nframes_t nframes, float * smpsl, float * smpsr) {
	int i;
	float l, r, ldl, rdl, rswell, lswell;

	for (i = 0; i < iPERIOD; i++) {
		ldl = ldelay[kl];
		rdl = rdelay[kr];
		l = ldl * (1.0f - lrcross) + rdl * lrcross;
		r = rdl * (1.0f - lrcross) + ldl * lrcross;
		ldl = l;
		rdl = r;

		ldl = smpsl[i] * panning - ldl * fb;
		rdl = smpsr[i] * (1.0f - panning) - rdl * fb;

		if (reverse > 0.0) {

			lswell = (float) (abs(kl - rvkl)) * Srate_Attack_Coeff;
			envswell = (1.0f - cosf(PI * envcnt * envattack));
			if (envswell > 1.0f)
				envswell = 1.0f;
			if (lswell <= PI) {
				lswell = 0.5f * (1.0f - cosf(lswell)); //Clickless transition
				efxoutl[i] = envswell
						* (reverse
								* (ldelay[rvkl] * lswell
										+ ldelay[rvfl] * (1.0f - lswell))
								+ (ldl * (1 - reverse))); //Volume ducking near zero crossing.
			} else {
				efxoutl[i] = ((ldelay[rvkl] * reverse) + (ldl * (1 - reverse)))
						* envswell;
			}

			rswell = (float) (abs(kr - rvkr)) * Srate_Attack_Coeff;
			if (rswell <= PI) {
				rswell = 0.5f * (1.0f - cosf(rswell)); //Clickless transition
				efxoutr[i] = envswell
						* (reverse
								* (rdelay[rvkr] * rswell
										+ rdelay[rvfr] * (1.0f - rswell))
								+ (rdl * (1 - reverse))); //Volume ducking near zero crossing.
			} else {
				efxoutr[i] = envswell
						* ((rdelay[rvkr] * reverse) + (rdl * (1 - reverse)));
			}

		} else {
			efxoutl[i] = ldl;
			efxoutr[i] = rdl;
		}

		//LowPass Filter
		ldelay[kl] = ldl = ldl * hidamp + oldl * (1.0f - hidamp);
		rdelay[kr] = rdl = rdl * hidamp + oldr * (1.0f - hidamp);
		oldl = ldl + DENORMAL_GUARD;
		oldr = rdl + DENORMAL_GUARD;

		if (++envcnt >= invattack)
			envcnt = invattack;
		if (kl > (dl - invattack))
			envcnt -= 2;
		if (envcnt < 0)
			envcnt = 0;

		if (++kl >= dl) {
			kl = 0;
			envcnt = 0;
			if (++harmonic >= Pharms)
				harmonic = 0;
		}
		if (++kr >= dr)
			kr = 0;
		rvkl += pattern[harmonic];
		if (rvkl >= (dl))
			rvkl = rvkl % (dl);
		rvkr += pattern[harmonic];
		if (rvkr >= (dr))
			rvkr = rvkr % (dr);

		rvfl = rvkl + fade;
		if (rvfl >= (dl))
			rvfl = rvfl % (dl);
		rvfr = rvkr + fade;
		if (rvfr >= (dr))
			rvfr = rvfr % (dr);
	};

}
;

/*
 * Parameter control
 */
void Arpie::setVolume(int Pvolume) {
	this->Pvolume = Pvolume;
	outvolume = (float) Pvolume / 127.0f;
	if (Pvolume == 0)
		cleanup();

}
;

void Arpie::setPanning(int Ppanning) {
	this->Ppanning = Ppanning;
	panning = ((float) Ppanning + 0.5f) / 127.0f;
}
;

void Arpie::setReverse(int Preverse) {
	this->Preverse = Preverse;
	reverse = (float) Preverse / 127.0f;
}
;

void Arpie::setDelay(int value) {
	this->Pdelay = Pdelay;
	if (Pdelay < 2)
		Pdelay = 2;
	if (Pdelay > 600)
		Pdelay = 600; //100ms .. 2 sec constraint
	delay = 1 + lrintf((60.0f / ((float) (subdiv * Pdelay))) * fSAMPLE_RATE); //quarter notes
	initdelays();

}

void Arpie::setLrdelay(int Plrdelay) {
	float tmp;
	this->Plrdelay = Plrdelay;
	tmp = (powf(2.0, fabsf((float) Plrdelay - 64.0f) / 64.0f * 9.0f) - 1.0f)
			/ 1000.0f * fSAMPLE_RATE;
	if (Plrdelay < 64.0)
		tmp = -tmp;
	lrdelay = lrintf(tmp);
	initdelays();
}
;

void Arpie::setLrcross(int Plrcross) {
	this->Plrcross = Plrcross;
	lrcross = (float) Plrcross / 127.0f * 1.0f;
}
;

void Arpie::setFb(int Pfb) {
	this->Pfb = Pfb;
	fb = (float) Pfb / 128.0f;
}

void Arpie::setHidamp(int Phidamp) {
	this->Phidamp = Phidamp;
	hidamp = 0.5f - (float) Phidamp / 254.0f;
}

void Arpie::setPattern(int Ppattern) {
	this->Ppattern = Ppattern;

	const int PATTERN_SIZE = MAXHARMS;
	const int NUM_PATTERNS = 7;
	int setpatterns[NUM_PATTERNS][PATTERN_SIZE] = { { 2, 3, 4, 5, 6, 7, 8, 9 },
			{ 9, 8, 7, 6, 5, 4, 3, 2 }, { 2, 4, 3, 5, 4, 6, 5, 7 }, { 2, 2, 4,
					3, 6, 2, 5, 3 }, { 3, 2, 4, 3, 5, 4, 6, 5 }, { 4, 3, 2, 7,
					5, 3, 4, 2 }, { 2, 3, 4, 5, 6, 7, 8, 9 } };

	if (Ppattern >= PATTERN_SIZE)
		Ppattern = PATTERN_SIZE - 1;
	for (int ii = 0; ii < PATTERN_SIZE; ii++)
		pattern[ii] = setpatterns[Ppattern][ii];

}
;

int Arpie::getHarms() {
	return Pharms;
}
void Arpie::setHarms(int value) {
	Pharms = value;
	if ((Pharms < 2) && (Pharms >= MAXHARMS)) {
		Pharms = 2;
	}
}

int Arpie::getSubdiv() {
	return Psubdiv;
}
void Arpie::setSubdiv(int value) {
	Psubdiv = value;
	subdiv = Psubdiv + 1;
	setDelay(Pdelay);
}

int Arpie::getVolume() {
	return Pvolume;
}
int Arpie::getPanning() {
	return Ppanning;
}
int Arpie::getDelay() {
	return Pdelay;
}
int Arpie::getLrdelay() {
	return Plrdelay;
}
int Arpie::getLrcross() {
	return Plrcross;
}
int Arpie::getFb() {
	return Pfb;
}
int Arpie::getHidamp() {
	return Phidamp;
}
int Arpie::getReverse() {
	return Preverse;
}
int Arpie::getPattern() {
	return Ppattern;
}

