/*
 ZynAddSubFX - a software synthesizer
 
 Alienwah.C - "AlienWah" effect
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

#include <plugins/effect/Alienwah.h>
using namespace std;

Alienwah::Alienwah() :
		YroEffectPlugin("Alienwah",
				"AlienWah1: 64, 64, 80, 0, 0, 62, 60, 105, 25, 0, 64;"
						"AlienWah2: 64, 64, 95, 106, 0, 101, 60, 105, 17, 0, 64;"
						"AlienWah3: 64, 64, 55, 0, 1, 100, 112, 105, 31, 0, 42;"
						"AlienWah4: 64, 64, 1, 0, 1, 66, 101, 11, 47, 0, 86;") {
	oldclfol.a = fb;
	oldclfol.b = 0.0;
	oldclfor.a = fb;
	oldclfor.b = 0.0;
	cleanup();
}

Alienwah::~Alienwah() {
}
;

/*
 * Apply the effect
 */
void Alienwah::render(jack_nframes_t nframes, float * smpsl, float * smpsr) {
	int i;
	float lfol, lfor;
	COMPLEXTYPE clfol, clfor, out, tmp;

	lfo.render(nframes, &lfol, &lfor);
	lfol *= depth * D_PI;
	lfor *= depth * D_PI;
	clfol.a = cosf(lfol + phase) * fb;
	clfol.b = sinf(lfol + phase) * fb;
	clfor.a = cosf(lfor + phase) * fb;
	clfor.b = sinf(lfor + phase) * fb;

	for (i = 0; i < iPERIOD; i++) {
		float x = (float) i / fPERIOD;
		float x1 = 1.0f - x;
		//left
		tmp.a = clfol.a * x + oldclfol.a * x1;
		tmp.b = clfol.b * x + oldclfol.b * x1;

		out.a = tmp.a * oldl[oldk].a - tmp.b * oldl[oldk].b
				+ (1.0f - fabsf(fb)) * smpsl[i] * panning;
		out.b = tmp.a * oldl[oldk].b + tmp.b * oldl[oldk].a;
		oldl[oldk].a = out.a;
		oldl[oldk].b = out.b;
		float l = out.a * 10.0f * (fb + 0.1f);

		//right
		tmp.a = clfor.a * x + oldclfor.a * x1;
		tmp.b = clfor.b * x + oldclfor.b * x1;

		out.a = tmp.a * oldr[oldk].a - tmp.b * oldr[oldk].b
				+ (1.0f - fabsf(fb)) * smpsr[i] * (1.0f - panning);
		out.b = tmp.a * oldr[oldk].b + tmp.b * oldr[oldk].a;
		oldr[oldk].a = out.a;
		oldr[oldk].b = out.b;
		float r = out.a * 10.0f * (fb + 0.1f);

		if (++oldk >= Pdelay)
			oldk = 0;
		//LRcross
		efxoutl[i] = l * (1.0f - lrcross) + r * lrcross;
		efxoutr[i] = r * (1.0f - lrcross) + l * lrcross;
	};

	oldclfol.a = clfol.a;
	oldclfol.b = clfol.b;
	oldclfor.a = clfor.a;
	oldclfor.b = clfor.b;

}
;

/*
 * Cleanup the effect
 */
void Alienwah::cleanup() {
	for (int i = oldpdelay; i < MAX_ALIENWAH_DELAY; i++) {
		oldl[i].a = 0.0f;
		oldl[i].b = 0.0f;
		oldr[i].a = 0.0f;
		oldr[i].b = 0.0f;

	};
	oldk = 0;
}

/*
 * Parameter control
 */
void Alienwah::setDepth(int Pdepth) {
	this->Pdepth = Pdepth;
	depth = ((float) Pdepth / 127.0f);
}

void Alienwah::setFb(int Pfb) {
	this->Pfb = Pfb;
	fb = fabsf(((float) Pfb - 64.0f) / 64.1f);
	fb = sqrtf(fb);
	if (fb < 0.4f)
		fb = 0.4f;
	if (Pfb < 64)
		fb = -fb;
}

void Alienwah::setVolume(int Pvolume) {
	this->Pvolume = Pvolume;
	outvolume = (float) Pvolume / 127.0f;
}

void Alienwah::setPanning(int Ppanning) {
	this->Ppanning = Ppanning;
	panning = ((float) Ppanning + .5f) / 127.0f;
}

void Alienwah::setLrcross(int Plrcross) {
	this->Plrcross = Plrcross;
	lrcross = (float) Plrcross / 127.0f;
}

void Alienwah::setPhase(int Pphase) {
	this->Pphase = Pphase;
	phase = ((float) Pphase - 64.0f) / 64.0f * PI;
}

void Alienwah::setDelay(int Pdelay) {
	if (Pdelay > MAX_ALIENWAH_DELAY)
		Pdelay = MAX_ALIENWAH_DELAY;
	this->Pdelay = Pdelay;
	if (Pdelay > oldpdelay)
		cleanup();
	oldpdelay = Pdelay;
}

int Alienwah::getDepth() {
	return Pdepth;
}

int Alienwah::getFb() {
	return Pfb;
}

int Alienwah::getVolume() {
	return Pvolume;
}

int Alienwah::getPanning() {
	return Ppanning;
}

int Alienwah::getLrcross() {
	return Plrcross;
}

int Alienwah::getPhase() {
	return Pphase;
}

int Alienwah::getDelay() {
	return Pdelay;
}

int Alienwah::getLfoPstereo() {
	return lfo.getPstereo();
}
int Alienwah::getLfoPlfOtype() {
	return lfo.getPlfOtype();
}
int Alienwah::getLfoPrandomness() {
	return lfo.getPrandomness();
}
int Alienwah::getLfoPfreq() {
	return lfo.getPfreq();
}

void Alienwah::setLfoPstereo(int value) {
	lfo.setPstereo(value);
}
void Alienwah::setLfoPlfOtype(int value) {
	lfo.setPlfOtype(value);
}
void Alienwah::setLfoPrandomness(int value) {
	lfo.setPrandomness(value);
}
void Alienwah::setLfoPfreq(int value) {
	lfo.setPfreq(value);
}
