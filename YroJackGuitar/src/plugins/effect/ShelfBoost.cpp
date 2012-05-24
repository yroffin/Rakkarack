/*
 Rakarrack   Audio FX software
 ShelfBoost.C - Tone Booster
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

#include <stdlib.h>

#include <plugins/effect/ShelfBoost.h>
using namespace std;

ShelfBoost::ShelfBoost() :
		YroEffectPlugin("ShelfBoost", "Trebble: 127,64,16000,1,24;"
				"Mid: 127,64,4400,1,24;"
				"Bass: 127,64,220,1,24;"
				"Distortion1: 6,40,12600,1,127;") {
	//default values
	Pvolume = 50;
	Pstereo = 0;

	RB1l = new AnalogFilter(7, 3200.0f, 0.5f, 0);
	RB1r = new AnalogFilter(7, 3200.0f, 0.5f, 0);

	setPreset(0);
	cleanup();
}
;

ShelfBoost::~ShelfBoost() {
}
;

/*
 * Cleanup the effect
 */
void ShelfBoost::cleanup() {

	RB1l->cleanup();
	RB1r->cleanup();

}
;

/*
 * Effect output
 */
void ShelfBoost::render(jack_nframes_t nframes, float * smpsl, float * smpsr) {
	int i;

	RB1l->filterout(iPERIOD, fPERIOD, smpsl);
	if (Pstereo)
		RB1r->filterout(iPERIOD, fPERIOD, smpsr);

	for (i = 0; i < iPERIOD; i++) {
		smpsl[i] *= outvolume * u_gain;
		if (Pstereo)
			smpsr[i] *= outvolume * u_gain;
	}

	if (!Pstereo)
		memcpy(smpsr, smpsl, sizeof(float) * iPERIOD);

}
;

/*
 * Parameter control
 */
void ShelfBoost::setVolume(int value) {
	Pvolume = value;
	outvolume = (float) Pvolume / 127.0f;

}
;

void ShelfBoost::setQ1(int value) {
	Pq1 = value;
	q1 = powf(30.0f, ((float) value - 64.0f) / 64.0f);
	RB1l->setq(q1);
	RB1r->setq(q1);
}
void ShelfBoost::setFreq1(int value) {
	Pfreq1 = value;
	freq1 = (float) value;
	RB1l->setfreq(freq1);
	RB1r->setfreq(freq1);
}
void ShelfBoost::setStereo(int value) {
	Pstereo = value;
}
void ShelfBoost::setLevel(int value) {
	Plevel = value;
	gain = .375f * (float) value;
	u_gain = 1.0f / gain;
	RB1l->setgain(gain);
	RB1r->setgain(gain);
}

int ShelfBoost::getVolume() {
	return Pvolume;
}
int ShelfBoost::getQ1() {
	return Pq1;
}
int ShelfBoost::getFreq1() {
	return Pfreq1;
}
int ShelfBoost::getStereo() {
	return Pstereo;
}
int ShelfBoost::getLevel() {
	return Plevel;
}
