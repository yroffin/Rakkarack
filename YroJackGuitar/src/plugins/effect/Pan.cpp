/*
 rakarrack - a guitar effects software

 pan.C  -  Auto/Pan -  Extra Stereo
 Copyright (C) 2008-2010 Josep Andreu
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

#include <plugins/effect/Pan.h>
using namespace std;

Pan::Pan() :
		YroEffectPlugin("Pan", "AutoPan: 64,64,26,0,0,0,0,1,0;"
				"ExtraStereo: 64,64,80,0,0,0,10,0,1;") {
	setPreset(0);
	/**
	 * TODO verify initialization of lfol and lfor
	 */
	lfo.render(1, &lfol, &lfor);
	cleanup();
}
;

Pan::~Pan() {
}
;

void Pan::cleanup() {
}
;

void Pan::render(jack_nframes_t nframes, float *smpsl, float *smpsr) {

	int i;
	float avg, ldiff, rdiff, tmp;
	float pp;
	float coeff_PERIOD = 1.0 / fPERIOD;
	float fi, P_i;

	if (PextraON) {

		for (i = 0; i < iPERIOD; i++)

		{

			avg = (smpsl[i] + smpsr[i]) * .5f;
			ldiff = smpsl[i] - avg;
			rdiff = smpsr[i] - avg;

			tmp = avg + ldiff * mul;
			smpsl[i] = tmp * cosf(dvalue);

			tmp = avg + rdiff * mul;
			smpsr[i] = tmp * sinf(dvalue);

		}

	}

	if (PAutoPan) {

		ll = lfol;
		lr = lfor;
		lfo.render(nframes, &lfol, &lfor);
		for (i = 0; i < iPERIOD; i++) {
			fi = (float) i;
			P_i = (float) (iPERIOD - i);

			pp = (ll * P_i + lfol * fi) * coeff_PERIOD;

			smpsl[i] *= pp * panning;

			pp = (lr * P_i + lfor * fi) * coeff_PERIOD;

			smpsr[i] *= pp * (1.0f - panning);

		}

	}

}
;

void Pan::setVolume(int Pvolume) {
	this->Pvolume = Pvolume;
	outvolume = (float) Pvolume / 127.0f;
}
;

void Pan::setPanning(int Ppanning) {
	this->Ppanning = Ppanning;
	panning = ((float) Ppanning) / 127.0f;
	dvalue = panning * M_PI_2;

}
;

void Pan::setExtra(int Pextra) {
	this->Pextra = Pextra;
	mul = 4.0f * (float) Pextra / 127.0f;
}
;

void Pan::setLfoFreq(int value) {
	lfo.setPfreq(value);
}
void Pan::setLfoRandomness(int value) {
	lfo.setPrandomness(value);
}
void Pan::setLfoType(int value) {
	lfo.setPlfOtype(value);
}
void Pan::setLfoStereo(int value) {
	lfo.setPstereo(value);
}
void Pan::setAutoPan(int value) {
	PAutoPan = value;
}
void Pan::setExtraOn(int value) {
	PextraON = value;
}

int Pan::getVolume() {
	return Pvolume;
}
int Pan::getPanning() {
	return Ppanning;
}
int Pan::getLfoFreq() {
	return lfo.getPfreq();
}
int Pan::getLfoRandomness() {
	return lfo.getPrandomness();
}
int Pan::getLfoType() {
	return lfo.getPlfOtype();
}
int Pan::getLfoStereo() {
	return lfo.getPstereo();
}
int Pan::getExtra() {
	return Pextra;
}
int Pan::getAutoPan() {
	return PAutoPan;
}
int Pan::getExtraOn() {
	return PextraON;
}

