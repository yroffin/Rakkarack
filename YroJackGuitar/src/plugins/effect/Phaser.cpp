/*
 ZynAddSubFX - a software synthesizer
 
 Phaser.C - Phaser effect
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

#include <plugins/effect/Phaser.h>
using namespace std;

#define PHASER_LFO_SHAPE 2

Phaser::Phaser() :
		YroEffectPlugin("Phaser",
				"Phaser1: 64, 64, 11, 0, 0, 64, 110, 64, 1, 0, 0, 20;"
						"Phaser2: 64, 64, 10, 0, 0, 88, 40, 64, 3, 0, 0, 20;"
						"Phaser3: 64, 64, 8, 0, 0, 66, 68, 107, 2, 0, 0, 20;"
						"Phaser4: 39, 64, 1, 0, 0, 66, 67, 10, 5, 0, 1, 20;"
						"Phaser5: 64, 64, 1, 0, 1, 110, 67, 78, 10, 0, 0, 20;"
						"Phaser6: 64, 64, 31, 100, 0, 58, 37, 78, 3, 0, 0, 20;") {
	oldl = (float *) malloc(sizeof(float) * MAX_PHASER_STAGES * 2);
	oldr = (float *) malloc(sizeof(float) * MAX_PHASER_STAGES * 2);

	setPreset(0);
	cleanup();
}
;

Phaser::~Phaser() {
}
;

/*
 * Effect output
 */
void Phaser::render(jack_nframes_t nframes, float * smpsl, float * smpsr) {
	int i, j;
	float lfol, lfor, lgain, rgain, tmp;

	lfo.render(nframes, &lfol, &lfor);
	lgain = lfol;
	rgain = lfor;
	lgain = (expf(lgain * PHASER_LFO_SHAPE) - 1.0f)
			/ (expf(PHASER_LFO_SHAPE) - 1.0f);
	rgain = (expf(rgain * PHASER_LFO_SHAPE) - 1.0f)
			/ (expf(PHASER_LFO_SHAPE) - 1.0f);

	lgain = 1.0f - phase * (1.0f - depth) - (1.0f - phase) * lgain * depth;
	rgain = 1.0f - phase * (1.0f - depth) - (1.0f - phase) * rgain * depth;

	if (lgain > 1.0)
		lgain = 1.0f;
	else if (lgain < 0.0)
		lgain = 0.0f;
	if (rgain > 1.0)
		rgain = 1.0f;
	else if (rgain < 0.0)
		rgain = 0.0f;

	for (i = 0; i < iPERIOD; i++) {
		float x = (float) i / fPERIOD;
		float x1 = 1.0f - x;
		float gl = lgain * x + oldlgain * x1;
		float gr = rgain * x + oldrgain * x1;
		float inl = smpsl[i] * panning + fbl;
		float inr = smpsr[i] * (1.0f - panning) + fbr;

		//Left channel
		for (j = 0; j < Pstages * 2; j++) { //Phasing routine
			tmp = oldl[j] + DENORMAL_GUARD;
			oldl[j] = gl * tmp + inl;
			inl = tmp - gl * oldl[j];
		};
		//Right channel
		for (j = 0; j < Pstages * 2; j++) { //Phasing routine
			tmp = oldr[j] + DENORMAL_GUARD;
			oldr[j] = (gr * tmp) + inr;
			inr = tmp - (gr * oldr[j]);
		};
		//Left/Right crossing
		float l = inl;
		float r = inr;
		inl = l * (1.0f - lrcross) + r * lrcross;
		inr = r * (1.0f - lrcross) + l * lrcross;

		fbl = inl * fb;
		fbr = inr * fb;
		efxoutl[i] = inl;
		efxoutr[i] = inr;

	};

	oldlgain = lgain;
	oldrgain = rgain;

	if (Poutsub != 0)
		for (i = 0; i < iPERIOD; i++) {
			efxoutl[i] *= -1.0f;
			efxoutr[i] *= -1.0f;
		};

}
;

/*
 * Cleanup the effect
 */
void Phaser::cleanup() {
	fbl = 0.0;
	fbr = 0.0;
	oldlgain = 0.0;
	oldrgain = 0.0;
	for (int i = 0; i < Pstages * 2; i++) {
		oldl[i] = 0.0;
		oldr[i] = 0.0;
	};
}
;

/*
 * Parameter control
 */
void Phaser::setDepth(int Pdepth) {
	this->Pdepth = Pdepth;
	depth = ((float) Pdepth / 127.0f);
}
;

void Phaser::setFb(int Pfb) {
	this->Pfb = Pfb;
	fb = ((float) Pfb - 64.0f) / 64.1f;
}
;

void Phaser::setVolume(int Pvolume) {
	this->Pvolume = Pvolume;
	outvolume = (float) Pvolume / 127.0f;
}
;

void Phaser::setPanning(int Ppanning) {
	this->Ppanning = Ppanning;
	panning = ((float) Ppanning + .5f) / 127.0f;
}
;

void Phaser::setLrcross(int Plrcross) {
	this->Plrcross = Plrcross;
	lrcross = (float) Plrcross / 127.0f;
}
;

void Phaser::setStages(int Pstages) {
	if (Pstages >= MAX_PHASER_STAGES)
		Pstages = MAX_PHASER_STAGES - 1;
	this->Pstages = Pstages;
	cleanup();
}
;

void Phaser::setPhase(int Pphase) {
	this->Pphase = Pphase;
	phase = ((float) Pphase / 127.0f);
}
;

void Phaser::setOutsub(int value) {
	if (value > 1)
		value = 1;
	Poutsub = value;
}

int Phaser::getVolume() {
	return Pvolume;
}
int Phaser::getPanning() {
	return Ppanning;
}
int Phaser::getLfoPfreq() {
	return lfo.getPfreq();
}
int Phaser::getLfoPrandomness() {
	return lfo.getPrandomness();
}
int Phaser::getLfoPlfOtype() {
	return lfo.getPlfOtype();
}
int Phaser::getLfoPstereo() {
	return lfo.getPstereo();
}
int Phaser::getDepth() {
	return Pdepth;
}
int Phaser::getFb() {
	return Pfb;
}
int Phaser::getStages() {
	return Pstages;
}
int Phaser::getLrcross() {
	return Plrcross;
}
int Phaser::getOutsub() {
	return Poutsub;
}
int Phaser::getPhase() {
	return Pphase;
}
void Phaser::setLfoPfreq(int value) {
	lfo.setPfreq(value);
}
void Phaser::setLfoPrandomness(int value) {
	lfo.setPrandomness(value);
}
void Phaser::setLfoPlfOtype(int value) {
	lfo.setPlfOtype(value);
}
void Phaser::setLfoPstereo(int value) {
	lfo.setPstereo(value);
}

