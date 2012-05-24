// Based in gate_1410.c LADSPA Swh-plugins

/*
 rakarrack - a guitar effects software

 Gate.C  -  Noise Gate Effect
 Based on Steve Harris LADSPA gate. 
 
 Copyright (C) 2008 Josep Andreu
 Author: Josep Andreu

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

#include <plugins/effect/Gate.h>
using namespace std;

Gate::Gate() :
		YroEffectPlugin("Gate", "+00:   0, 0, 1, 2, 6703, 76, 2;"
				"-10: 0, -10, 1, 2, 6703, 76, 2;"
				"-20: 0, -20, 1, 2, 6703, 76, 2;") {
	lpfl = new AnalogFilter(2, 22000, 1, 0);
	lpfr = new AnalogFilter(2, 22000, 1, 0);
	hpfl = new AnalogFilter(3, 20, 1, 0);
	hpfr = new AnalogFilter(3, 20, 1, 0);

	env = 0.0;
	gate = 0.0;
	fs = fSAMPLE_RATE;
	state = CLOSED;
	hold_count = 0;
}

Gate::~Gate() {
}

void Gate::cleanup() {
	lpfl->cleanup();
	hpfl->cleanup();
	lpfr->cleanup();
	hpfr->cleanup();
}

void Gate::setLpf(int value) {
	Plpf = value;
	float fr = (float) Plpf;
	lpfl->setfreq(fr);
	lpfr->setfreq(fr);
}

void Gate::setHpf(int value) {
	Phpf = value;
	float fr = (float) Phpf;
	hpfl->setfreq(fr);
	hpfr->setfreq(fr);
}

void Gate::setThreshold(int value) {
	Pthreshold = value;
	t_level = dB2rap ((float)Pthreshold);
}
void Gate::setRange(int value) {
	Prange = value;
	cut = dB2rap ((float)Prange);
}
void Gate::setAttack(int value) {
	Pattack = value;
	a_rate = 1000.0f / ((float) Pattack * fs);
}
void Gate::setDecay(int value) {
	Pdecay = value;
	d_rate = 1000.0f / ((float) Pdecay * fs);
}
void Gate::setHold(int value) {
	Phold = value;
	hold = (float) Phold;
}

void Gate::render(jack_nframes_t nframes, float *efxoutl, float *efxoutr) {

	int i;
	float sum;

	lpfl->filterout(iPERIOD, fPERIOD, efxoutl);
	hpfl->filterout(iPERIOD, fPERIOD, efxoutl);
	lpfr->filterout(iPERIOD, fPERIOD, efxoutr);
	hpfr->filterout(iPERIOD, fPERIOD, efxoutr);

	for (i = 0; i < iPERIOD; i++) {

		sum = fabsf(efxoutl[i]) + fabsf(efxoutr[i]);

		if (sum > env)
			env = sum;
		else
			env = sum * ENV_TR + env * (1.0f - ENV_TR);

		if (state == CLOSED) {
			if (env >= t_level)
				state = OPENING;
		} else if (state == OPENING) {
			gate += a_rate;
			if (gate >= 1.0) {
				gate = 1.0f;
				state = OPEN;
				hold_count = lrintf(hold * fs * 0.001f);
			}
		} else if (state == OPEN) {
			if (hold_count <= 0) {
				if (env < t_level) {
					state = CLOSING;
				}
			} else
				hold_count--;

		} else if (state == CLOSING) {
			gate -= d_rate;
			if (env >= t_level)
				state = OPENING;
			else if (gate <= 0.0) {
				gate = 0.0;
				state = CLOSED;
			}
		}

		efxoutl[i] *= (cut * (1.0f - gate) + gate);
		efxoutr[i] *= (cut * (1.0f - gate) + gate);

	}

}

int Gate::getThreshold() {
	return Pthreshold;
}
int Gate::getRange() {
	return Prange;
}
int Gate::getAttack() {
	return Pattack;
}
int Gate::getDecay() {
	return Pdecay;
}
int Gate::getLpf() {
	return Plpf;
}
int Gate::getHpf() {
	return Phpf;
}
int Gate::getHold() {
	return Phold;
}
