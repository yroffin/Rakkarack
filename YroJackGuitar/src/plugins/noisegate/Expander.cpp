/*
 rakarrack - a guitar effects software

 Expander.C  -  Noise Gate Effect
 
 Copyright (C) 2010 Ryan Billing & Josep Andreu
 Author: Ryan Billing & Josep Andreu
 Adapted from swh-plugins Noise Gate by Steve Harris

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

#include <plugins/noisegate/Expander.h>

using namespace std;

Expander::Expander() :
		YroEffectPlugin(
				"Expander",
				"Boost Gate: -55, 30, 50, 50, 1441, 157, 50;"
				"Noise Gate: -50, 20, 50, 50, 3134, 76, 0;"
				"Treble swell: -30, 9, 950, 25, 6703, 526, 90;") {

	lpfl = new AnalogFilter(2, 22000, 1, 0);
	lpfr = new AnalogFilter(2, 22000, 1, 0);
	hpfl = new AnalogFilter(3, 20, 1, 0);
	hpfr = new AnalogFilter(3, 20, 1, 0);

	env = 0.0;
	oldgain = 0.0;
	efollower = 0;
	setPreset(0);
}

Expander::~Expander() {
}

void Expander::cleanup() {
	lpfl->cleanup();
	hpfl->cleanup();
	lpfr->cleanup();
	hpfr->cleanup();
	oldgain = 0.0f;

}

int Expander::getEfollower() const {
	return efollower;
}

void Expander::setEfollower(int efollower) {
	this->efollower = efollower;
}

int Expander::getPattack() const {
	return Pattack;
}

void Expander::setPattack(int value) {
	Pattack = value;
	a_rate = 1000.0f / ((float) Pattack * fSAMPLE_RATE);
	onChange(_attack);
}

int Expander::getPdecay() const {
	return Pdecay;
}

void Expander::setPdecay(int value) {
	Pdecay = value;
	d_rate = 1000.0f / ((float) Pdecay * fSAMPLE_RATE);
	onChange(_decay);
}

int Expander::getPhpf() const {
	return Phpf;
}

void Expander::setPhpf(int value) {
	Phpf = value;
	float fr = (float) Phpf;
	hpfl->setfreq(fr);
	hpfr->setfreq(fr);
	onChange(_hpf);
}

int Expander::getPlevel() const {
	return Plevel;
}

void Expander::setPlevel(int value) {
	Plevel = value;
	level = dB2rap((float) value/6.0f);
	onChange(_level);
}

int Expander::getPlpf() const {
	return Plpf;
}

void Expander::setPlpf(int value) {
	Plpf = value;
	float fr = (float) Plpf;
	lpfl->setfreq(fr);
	lpfr->setfreq(fr);
	onChange(_lpf);
}

int Expander::getPshape() const {
	return Pshape;
}

void Expander::setPshape(int value) {
	Pshape = value;
	sfactor = dB2rap ((float)Pshape/2);
	sgain = expf(-sfactor);
	onChange(_shape);
}

int Expander::getPthreshold() const {
	return Pthreshold;
}

void Expander::setPthreshold(int value) {
	Pthreshold = value;
	tfactor = dB2rap (-((float) Pthreshold));
	tlevel = 1.0f / tfactor;
	onChange(_threshold);
}

/**
 * render the effect
 */
void Expander::render(float *smpl, float *smpr) {

	int i;
	float delta = 0.0f;
	float expenv = 0.0f;

	lpfl->filterout(iPERIOD, fPERIOD, efxoutl);
	hpfl->filterout(iPERIOD, fPERIOD, efxoutl);
	lpfr->filterout(iPERIOD, fPERIOD, efxoutr);
	hpfr->filterout(iPERIOD, fPERIOD, efxoutr);

	for (i = 0; i < iPERIOD; i++) {

		delta = 0.5f * (fabsf(efxoutl[i]) + fabsf(efxoutr[i])) - env; //envelope follower from Compressor.C
		if (delta > 0.0)
			env += a_rate * delta;
		else
			env += d_rate * delta;

		//End envelope power detection

		if (env > tlevel)
			env = tlevel;
		expenv = sgain * (expf(env * sfactor * tfactor) - 1.0f); //Envelope waveshaping

		gain = (1.0f - d_rate) * oldgain + d_rate * expenv;
		oldgain = gain; //smooth it out a little bit

		if (efollower) {
			efxoutl[i] = gain;
			efxoutr[i] += gain;
		} else {
			efxoutl[i] *= gain * level;
			efxoutr[i] *= gain * level;
		}

	}
}
