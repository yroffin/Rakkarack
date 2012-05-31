/*
 ZynAddSubFX - a software synthesizer

 Distorsion.C - Distorsion effect
 Copyright (C) 2002-2005 Nasca Octavian Paul
 Author: Nasca Octavian Paul

 Modified for rakarrack by Josep Andreu & Ryan Billing

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

#include <plugins/distortion/Distortion.h>

using namespace std;

Distortion::Distortion() :
		YroEffectPlugin("Distortion",
				"Distorsion 1: 0, 64,   0,  87, 14,  6, 0,  3134, 157, 0, 1;"
						"Distorsion 2: 0, 64, 127,  87, 14,  0, 1,  3134, 102, 0, 0;"
						"Distorsion 3: 0, 64, 127, 127, 12, 13, 0,  5078,  56, 0, 1;"
						"Guitar Amp:  84, 64,  35,  63, 50,  2, 0,   824,  21, 0, 0;"
						"Overdrive 1: 84, 64,  35,  56, 40,  0, 0,  6703,  21, 0, 0;"
						"Overdrive 2: 85, 64,  35,  29, 45,  1, 0, 25040,  21, 0, 0;") {
	/**
	 * fix klass attribute
	 */
	klass = YroEffectPlugin::_Distortion;

	/**
	 * allocate octoutl and octoutr
	 */
	octoutl = new float[iPERIOD];
	memset(octoutl, 0, iPERIOD * sizeof(float));
	octoutr = new float[iPERIOD];
	memset(octoutr, 0, iPERIOD * sizeof(float));

	lpfl = new AnalogFilter(2, 22000, 1, 0);
	lpfr = new AnalogFilter(2, 22000, 1, 0);
	hpfl = new AnalogFilter(3, 20, 1, 0);
	hpfr = new AnalogFilter(3, 20, 1, 0);
	blockDCl = new AnalogFilter(2, 440.0f, 1, 0);
	blockDCr = new AnalogFilter(2, 440.0f, 1, 0);
	blockDCl->setfreq(75.0f);
	blockDCr->setfreq(75.0f);
	DCl = new AnalogFilter(3, 30, 1, 0);
	DCr = new AnalogFilter(3, 30, 1, 0);
	DCl->setfreq(30.0f);
	DCr->setfreq(30.0f);

	dwshapel = new WaveShaper();
	dwshaper = new WaveShaper();

	//default values
	preset = 0;
	Pvolume = 50;
	Plrcross = 40;
	Pdrive = 90;
	Plevel = 64;
	Ptype = 0;
	Pnegate = 0;
	Plpf = 127;
	Phpf = 0;
	Pstereo = 0;
	Pprefiltering = 0;
	Poctave = 0;
	togglel = 1.0;
	octave_memoryl = -1.0;
	toggler = 1.0;
	octave_memoryr = -1.0;
	octmix = 0.0;

	setPreset(0);
	cleanup();
}

Distortion::~Distortion() {
	/**
	 * cleanup allocated resources
	 */
	if (octoutl == 0)
		delete octoutl;
	if (octoutr == 0)
		delete octoutr;
}

/*
 * Cleanup the effect
 */
void Distortion::cleanup() {
	lpfl->cleanup();
	hpfl->cleanup();
	lpfr->cleanup();
	hpfr->cleanup();
	blockDCr->cleanup();
	blockDCl->cleanup();
	DCl->cleanup();
	DCr->cleanup();
}

/*
 * Apply the filters
 */
void Distortion::applyFilters() {
	lpfl->filterout(iPERIOD, fPERIOD, efxoutl);
	hpfl->filterout(iPERIOD, fPERIOD, efxoutl);

	if (Pstereo != 0) { //stereo
		lpfr->filterout(iPERIOD, fPERIOD, efxoutr);
		hpfr->filterout(iPERIOD, fPERIOD, efxoutr);
	};

}

/*
 * Effect output
 * @param jack_nframes_t nframes, number of byte to render
 * @param float *smpsl left
 * @param float *smpsr right
 */
void Distortion::render(jack_nframes_t nframes, float *smpsl, float *smpsr) {
	int i;
	float l, r, lout, rout;

	float inputvol = powf(5.0f, ((float) Pdrive - 32.0f) / 127.0f);
	if (Pnegate != 0)
		inputvol *= -1.0f;

	if (Pstereo != 0) { //Stereo
		for (i = 0; i < iPERIOD; i++) {
			efxoutl[i] = smpsl[i] * inputvol * 2.0f;
			efxoutr[i] = smpsr[i] * inputvol * 2.0f;
		};
	} else {
		for (i = 0; i < iPERIOD; i++) {
			efxoutl[i] = (smpsl[i] + smpsr[i]) * inputvol;
		};
	};

	if (Pprefiltering != 0)
		applyFilters();

	//no optimised, yet (no look table)

	dwshapel->waveshapesmps(iPERIOD, efxoutl, Ptype, Pdrive, 1);
	if (Pstereo != 0)
		dwshaper->waveshapesmps(iPERIOD, efxoutr, Ptype, Pdrive, 1);

	if (Pprefiltering == 0)
		applyFilters();

	if (Pstereo == 0)
		memcpy(efxoutr, efxoutl, iPERIOD * sizeof(float));

	if (octmix > 0.01f) {
		for (i = 0; i < iPERIOD; i++) {
			lout = efxoutl[i];
			rout = efxoutr[i];

			if ((octave_memoryl < 0.0f) && (lout > 0.0f))
				togglel *= -1.0f;

			octave_memoryl = lout;

			if ((octave_memoryr < 0.0f) && (rout > 0.0f))
				toggler *= -1.0f;

			octave_memoryr = rout;

			octoutl[i] = lout * togglel;
			octoutr[i] = rout * toggler;
		}

		blockDCr->filterout(iPERIOD,fPERIOD,octoutr);
		blockDCl->filterout(iPERIOD,fPERIOD,octoutl);
	}

	float level = dB2rap (60.0f * (float)Plevel / 127.0f - 40.0f);

	for (i = 0; i < iPERIOD; i++) {
		lout = efxoutl[i];
		rout = efxoutr[i];

		l = lout * (1.0f - lrcross) + rout * lrcross;
		r = rout * (1.0f - lrcross) + lout * lrcross;

		if (octmix > 0.01f) {
			lout = l * (1.0f - octmix) + octoutl[i] * octmix;
			rout = r * (1.0f - octmix) + octoutr[i] * octmix;
		} else {
			lout = l;
			rout = r;
		}

		efxoutl[i] = lout * 2.0f * level * panning;
		efxoutr[i] = rout * 2.0f * level * (1.0f - panning);

	};

	DCr->filterout(iPERIOD,fPERIOD,efxoutr);
	DCl->filterout(iPERIOD,fPERIOD,efxoutl);
}

void Distortion::setPlrcross(int value) {
	this->Plrcross = value;
	lrcross = (float) Plrcross / 127.0f * 1.0f;
	onChange(_lrcross);
}

int Distortion::getPdrive() {
	return Pdrive;
}

void Distortion::setPdrive(int pdrive) {
	Pdrive = pdrive;
	onChange(_drive);
}

int Distortion::getPhpf() const {
	return Phpf;
}

void Distortion::setPhpf(int value) {
	Phpf = value;
	float fr = (float) Phpf;

	hpfl->setfreq(fr);
	hpfr->setfreq(fr);
	//Prefiltering of 51 is approx 630 Hz. 50 - 60 generally good for OD pedal.
}

int Distortion::getPlevel() const {
	return Plevel;
}

void Distortion::setPlevel(int plevel) {
	Plevel = plevel;
	onChange(_level);
}

int Distortion::getPlpf() const {
	return Plpf;
}

void Distortion::setPlpf(int value) {
	Plpf = value;
	float fr = (float) Plpf;
	lpfl->setfreq(fr);
	lpfr->setfreq(fr);
	onChange(_lpf);
}

int Distortion::getPoctave() const {
	return Poctave;
}

void Distortion::setPoctave(int poctave) {
	this->Poctave = poctave;
	octmix = (float) (Poctave) / 127.0f;
	onChange(_octave);
}

int Distortion::getPpanning() const {
	return Ppanning;
}

void Distortion::setPpanning(int ppanning) {
	this->Ppanning = ppanning;
	panning = ((float) Ppanning + 0.5f) / 127.0f;
	onChange(_panning);
}

int Distortion::getPvolume() const {
	return Pvolume;
}

void Distortion::setPvolume(int pvolume) {
	this->Pvolume = pvolume;

	outvolume = (float) Pvolume / 127.0f;
	if (Pvolume == 0)
		cleanup();
	onChange(_volume);
}

int Distortion::getPlrcross() const {
	return Plrcross;
}

int Distortion::getPtype() const {
	return Ptype;
}

void Distortion::setPtype(int ptype) {
	Ptype = ptype;
	onChange(_type);
}

int Distortion::getPnegate() const {
	return Pnegate;
}

void Distortion::setPnegate(int value) {
	if (value > 1)
		value = 1;
	Pnegate = value;
	onChange(_negate);
}

int Distortion::getPstereo() const {
	return Pstereo;
}

void Distortion::setPstereo(int value) {
	if (value > 1)
		value = 1;
	Pstereo = value;
	onChange(_stereo);
}

int Distortion::getPprefiltering() const {
	return Pprefiltering;
}

void Distortion::setPprefiltering(int pprefiltering) {
	Pprefiltering = pprefiltering;
	onChange(_prefiltering);
}

const char *Distortion::toXml() {
	char _buffer[256];
	char _formatd[] = { "<attribute name=\"%s\" value=\"%d\" />" };
	char _formatf[] = { "<attribute name=\"%s\" value=\"%f\" />" };
	strcpy(_toXml, "<attributes>");
	sprintf(_buffer, _formatd, "waveshapeUpQuality", helper->getWaveshapeUpQuality());
	strcat(_toXml, _buffer);
	sprintf(_buffer, _formatd, "waveshapeDownQuality", helper->getWaveshapeDownQuality());
	strcat(_toXml, _buffer);
	sprintf(_buffer, _formatd, "Pdrive", Pdrive);
	strcat(_toXml, _buffer);
	sprintf(_buffer, _formatd, "Phpf", Phpf);
	strcat(_toXml, _buffer);
	sprintf(_buffer, _formatd, "Plevel", Plevel);
	strcat(_toXml, _buffer);
	sprintf(_buffer, _formatd, "Plpf", Plpf);
	strcat(_toXml, _buffer);
	sprintf(_buffer, _formatd, "Plrcross", Plrcross);
	strcat(_toXml, _buffer);
	sprintf(_buffer, _formatd, "Pnegate", Pnegate);
	strcat(_toXml, _buffer);
	sprintf(_buffer, _formatd, "Poctave", Poctave);
	strcat(_toXml, _buffer);
	sprintf(_buffer, _formatd, "Ppanning", Ppanning);
	strcat(_toXml, _buffer);
	sprintf(_buffer, _formatd, "Pprefiltering", Pprefiltering);
	strcat(_toXml, _buffer);
	sprintf(_buffer, _formatd, "Pstereo", Pstereo);
	strcat(_toXml, _buffer);
	sprintf(_buffer, _formatd, "Ptype", Ptype);
	strcat(_toXml, _buffer);
	sprintf(_buffer, _formatd, "Pvolume", Pvolume);
	strcat(_toXml, _buffer);
	sprintf(_buffer, _formatf, "outvolume", outvolume);
	strcat(_toXml, _buffer);
	sprintf(_buffer, _formatf, "panning", panning);
	strcat(_toXml, _buffer);
	sprintf(_buffer, _formatf, "lrcross", lrcross);
	strcat(_toXml, _buffer);
	sprintf(_buffer, _formatf, "octave_memoryl", octave_memoryl);
	strcat(_toXml, _buffer);
	sprintf(_buffer, _formatf, "togglel", togglel);
	strcat(_toXml, _buffer);
	sprintf(_buffer, _formatf, "octave_memoryr", octave_memoryr);
	strcat(_toXml, _buffer);
	sprintf(_buffer, _formatf, "toggler", toggler);
	strcat(_toXml, _buffer);
	sprintf(_buffer, _formatf, "octmix", octmix);
	strcat(_toXml, _buffer);
	strcat(_toXml, "</attributes>");
	return _toXml;
}

void std::Distortion::toStringCompute() {
	toStringCat("Pprefiltering=%d\n", Pprefiltering);
}

