/*
 ZynAddSubFX - a software synthesizer
 
 YroChorus.C - YroChorus and Flange effects
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

#include <stdio.h>
#include <math.h>
#include <plugins/effect/YroChorus.h>

using namespace std;

YroChorus::YroChorus() :
		YroEffectPlugin("Chorus") {
	dlk = 0;
	drk = 0;
	maxdelay = lrintf(MAX_CHORUS_DELAY / 1000.0 * iSAMPLE_RATE);
	delayl = new float[maxdelay];
	delayr = new float[maxdelay];

	lfo.render(1, &lfol, &lfor);
	dl2 = getdelay(lfol);
	dr2 = getdelay(lfor);
	cleanup();
}
;

YroChorus::~YroChorus() {
}

int YroChorus::getPflangemode() const {
	return Pflangemode;
}

void YroChorus::setPflangemode(int pflangemode) {
	if (pflangemode > 1)
		pflangemode = 1;
	Pflangemode = pflangemode;
}

int YroChorus::getPoutsub() const {
	return Poutsub;
}

void YroChorus::setPoutsub(int poutsub) {
	if (poutsub > 1)
		poutsub = 1;
	Poutsub = poutsub;
}

/*
 * get the delay value in samples; xlfo is the current lfo value
 */
float YroChorus::getdelay(float xlfo) {
	float result;
	if (Pflangemode == 0) {
		result = (delay + xlfo * depth) * fSAMPLE_RATE;
	} else
		result = 0;

	//check if it is too big delay(caused bu errornous setdelay() and setdepth()
	if ((result + 0.5) >= maxdelay) {
		fprintf(stderr, "%s",
				"WARNING: YroChorus.C::getdelay(..) too big delay (see setdelay and setdepth funcs.)\n");
		printf("%f %d\n", result, maxdelay);
		result = (float) maxdelay - 1.0f;
	};
	return (result);
}
;

/*
 * Apply the effect
 */
void YroChorus::render(jack_nframes_t nframes, float * smpsl, float * smpsr) {
	dl1 = dl2;
	dr1 = dr2;
	lfo.render(1, &lfol, &lfor);

	dl2 = getdelay(lfol);
	dr2 = getdelay(lfor);

	for (jack_nframes_t i = 0; i < nframes; i++) {
		float inl = smpsl[i];
		float inr = smpsr[i];
		//LRcross
		float l = inl;
		float r = inr;
		inl = l * (1.0f - lrcross) + r * lrcross;
		inr = r * (1.0f - lrcross) + l * lrcross;

		//Left channel

		//compute the delay in samples using linear interpolation between the lfo delays
		mdel = (dl1 * (float) (nframes - i) + dl2 * (float) i) / fPERIOD;
		if (++dlk >= maxdelay)
			dlk = 0;
		float tmp = (float) dlk - mdel + (float) maxdelay * 2.0f; //where should I get the sample from

		F2I(tmp, dlhi);
		dlhi %= maxdelay;

		dlhi2 = (dlhi - 1 + maxdelay) % maxdelay;
		dllo = 1.0f - fmodf(tmp, 1.0f);
		efxoutl[i] = delayl[dlhi2] * dllo + delayl[dlhi] * (1.0f - dllo);
		delayl[dlk] = inl + efxoutl[i] * fb;

		//Right channel

		//compute the delay in samples using linear interpolation between the lfo delays
		mdel = (dr1 * (float) (nframes - i) + dr2 * (float) i) / fPERIOD;
		if (++drk >= maxdelay)
			drk = 0;
		tmp = (float) drk - mdel + (float) maxdelay * 2.0f; //where should I get the sample from

		F2I(tmp, dlhi);
		dlhi %= maxdelay;

		dlhi2 = (dlhi - 1 + maxdelay) % maxdelay;
		dllo = 1.0f - fmodf(tmp, 1.0f);
		efxoutr[i] = delayr[dlhi2] * dllo + delayr[dlhi] * (1.0f - dllo);
		delayr[dlk] = inr + efxoutr[i] * fb;

	};

	if (Poutsub != 0)
		for (jack_nframes_t i = 0; i < nframes; i++) {
			efxoutl[i] *= -1.0f;
			efxoutr[i] *= -1.0f;
		};

	for (jack_nframes_t i = 0; i < nframes; i++) {
		efxoutl[i] *= panning;
		efxoutr[i] *= (1.0f - panning);
	};
}
;

/*
 * Cleanup the effect
 */
void YroChorus::cleanup() {
	for (int i = 0; i < maxdelay; i++) {
		delayl[i] = 0.0;
		delayr[i] = 0.0;
	};

}
;

/*
 * Parameter control
 */
void YroChorus::setPdepth(int Pdepth) {
	this->Pdepth = Pdepth;
	depth = (powf(8.0f, ((float) Pdepth / 127.0f) * 2.0f) - 1.0f) / 1000.0f; //seconds
}
;

void YroChorus::setPdelay(int Pdelay) {
	this->Pdelay = Pdelay;
	delay = (powf(10.0f, ((float) Pdelay / 127.0f) * 2.0f) - 1.0f) / 1000.0f; //seconds
}
;

void YroChorus::setPfb(int Pfb) {
	this->Pfb = Pfb;
	fb = ((float) Pfb - 64.0f) / 64.1f;
}
;

void YroChorus::setPvolume(int Pvolume) {
	this->Pvolume = Pvolume;
	outvolume = (float) Pvolume / 127.0f;
}
;

void YroChorus::setPpanning(int Ppanning) {
	this->Ppanning = Ppanning;
	panning = ((float) Ppanning + .5f) / 127.0f;
}
;

void YroChorus::setPlrcross(int Plrcross) {
	this->Plrcross = Plrcross;
	lrcross = (float) Plrcross / 127.0f;
}

void YroChorus::setPreset(int npreset) {
	YroEffectPlugin::setPreset(npreset);
	const int PRESET_SIZE = 12;
	const int NUM_PRESETS = 10;
	int presets[NUM_PRESETS][PRESET_SIZE] = {
	//Chorus1
			{ 64, 64, 33, 0, 0, 90, 40, 85, 64, 119, 0, 0 },
			//Chorus2
			{ 64, 64, 19, 0, 0, 98, 56, 90, 64, 19, 0, 0 },
			//Chorus3
			{ 64, 64, 7, 0, 1, 42, 97, 95, 90, 127, 0, 0 },
			//Celeste1
			{ 64, 64, 1, 0, 0, 42, 115, 18, 90, 127, 0, 0 },
			//Celeste2
			{ 64, 64, 7, 117, 0, 50, 115, 9, 31, 127, 0, 1 },
			//Flange1
			{ 64, 64, 39, 0, 0, 60, 23, 3, 62, 0, 0, 0 },
			//Flange2
			{ 64, 64, 9, 34, 1, 40, 35, 3, 109, 0, 0, 0 },
			//Flange3
			{ 64, 64, 31, 34, 1, 94, 35, 3, 54, 0, 0, 1 },
			//Flange4
			{ 64, 64, 14, 0, 1, 62, 12, 19, 97, 0, 0, 0 },
			//Flange5
			{ 64, 64, 34, 105, 0, 24, 39, 19, 17, 0, 0, 1 } };

	setPvolume(presets[npreset][0]);
	setPpanning(presets[npreset][1]);
	lfo.setPfreq(presets[npreset][2]);
	lfo.setPrandomness(presets[npreset][3]);
	lfo.setPlfOtype(presets[npreset][4]);
	lfo.setPstereo(presets[npreset][5]);
	setPdepth(presets[npreset][6]);
	setPdelay(presets[npreset][7]);
	setPfb(presets[npreset][8]);
	setPlrcross(presets[npreset][9]);
	setPflangemode(presets[npreset][10]);
	setPoutsub(presets[npreset][11]);
}

