/*
 * YroDistortion.cpp
 *
 *  Created on: 7 mai 2012
 *      Author: yannick
 */

#include "YroDistortion.h"

using namespace std;

YroDistortion::YroDistortion() :
		YroEffectPlugin("Distortion") {

	/**
	 * allocated during the first render
	 */
	octoutl = 0;
	octoutr = 0;

	lpfl = new YroAnalogFilter(2, 22000, 1, 0);
	lpfr = new YroAnalogFilter(2, 22000, 1, 0);
	hpfl = new YroAnalogFilter(3, 20, 1, 0);
	hpfr = new YroAnalogFilter(3, 20, 1, 0);
	blockDCl = new YroAnalogFilter(2, 440.0f, 1, 0);
	blockDCr = new YroAnalogFilter(2, 440.0f, 1, 0);
	blockDCl->setfreq(75.0f);
	blockDCr->setfreq(75.0f);
	DCl = new YroAnalogFilter(3, 30, 1, 0);
	DCr = new YroAnalogFilter(3, 30, 1, 0);
	DCl->setfreq(30.0f);
	DCr->setfreq(30.0f);

	dwshapel = new YroWaveShaper();
	dwshaper = new YroWaveShaper();

	//default values
	Ppreset = 0;
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

	setPreset(Ppreset);
	cleanup();
}

YroDistortion::~YroDistortion() {
	/**
	 * cleanup allocated resources
	 */
	if(octoutl == 0) delete octoutl;
	if(octoutr == 0) delete octoutr;
}

/*
 * Cleanup the effect
 */
void YroDistortion::cleanup() {
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
void YroDistortion::applyFilters(int iPeriod, float fPeriod) {
	lpfl->filterout(iPeriod, fPeriod, efxoutl);
	hpfl->filterout(iPeriod, fPeriod, efxoutl);

	if (Pstereo != 0) { //stereo
		lpfr->filterout(iPeriod, fPeriod, efxoutr);
		hpfr->filterout(iPeriod, fPeriod, efxoutr);
	};

}

/*
 * Effect output
 * @param jack_nframes_t nframes, number of byte to render
 * @param float *smpsl left
 * @param float *smpsr right
 */
void YroDistortion::render(jack_nframes_t nframes, float *smpsl, float *smpsr) {
	/**
	 * TODO allocate if dynamicly and detect nframes adjustment on the fly
	 * allocate
	 */
	if(octoutl == 0) {
		octoutl = new jack_default_audio_sample_t[nframes];
		memset(octoutl,0,sizeof(octoutl));
	}
	if(octoutr == 0) {
		octoutr = new jack_default_audio_sample_t[nframes];
		memset(octoutr,0,sizeof(octoutr));
	}

	float l = 0., r = 0., lout = 0., rout = 0.;

	float inputvol = powf(5.0f, ((float) Pdrive - 32.0f) / 127.0f);
	if (Pnegate != 0)
		inputvol *= -1.0f;

	if (Pstereo != 0) { //Stereo
		for (jack_nframes_t i = 0; i < nframes; i++) {
			efxoutl[i] = smpsl[i] * inputvol * 2.0f;
			efxoutr[i] = smpsr[i] * inputvol * 2.0f;
		};
	} else {
		for (jack_nframes_t i = 0; i < nframes; i++) {
			efxoutl[i] = (smpsl[i] + smpsr[i]) * inputvol;
		};
	};

	if (Pprefiltering != 0) {
		applyFilters(int(nframes), float(nframes));
	}

	//no optimised, yet (no look table)

	dwshapel->waveshapesmps(nframes, efxoutl, Ptype, Pdrive, 1);
	if (Pstereo != 0)
		dwshaper->waveshapesmps(nframes, efxoutr, Ptype, Pdrive, 1);

	if (Pprefiltering == 0) {
		applyFilters(int(nframes), float(nframes));
	}

	if (Pstereo == 0)
		memcpy(efxoutr, efxoutl, nframes * sizeof(float));

	if (octmix > 0.01f) {
		for (jack_nframes_t i = 0; i < nframes; i++) {
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

		blockDCr->filterout(int(nframes), float(nframes), octoutr);
		blockDCl->filterout(int(nframes), float(nframes), octoutl);
	}

	float level = dB2rap (60.0f * (float)Plevel / 127.0f - 40.0f);

	for (jack_nframes_t i = 0; i < nframes; i++) {
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

	DCr->filterout(int(nframes), float(nframes), efxoutr);
	DCl->filterout(int(nframes), float(nframes), efxoutl);
}

void YroDistortion::setPlrcross(int value) {
	this->Plrcross = value;
	lrcross = (float) Plrcross / 127.0f * 1.0f;
}

int YroDistortion::getPdrive() const {
	return Pdrive;
}

void YroDistortion::setPdrive(int pdrive) {
	Pdrive = pdrive;
}

int YroDistortion::getPreset() const {
	return Ppreset;
}

void YroDistortion::setPreset(int npreset) {
	Ppreset = npreset;
	const int PRESET_SIZE = 11;
	const int NUM_PRESETS = 6;
	int presets[NUM_PRESETS][PRESET_SIZE] = {
	//Overdrive 1
			{ 84, 64, 35, 56, 40, 0, 0, 6703, 21, 0, 0 },
			//Overdrive 2
			{ 85, 64, 35, 29, 45, 1, 0, 25040, 21, 0, 0 },
			//Distorsion 1
			{ 0, 64, 0, 87, 14, 6, 0, 3134, 157, 0, 1 },
			//Distorsion 2
			{ 0, 64, 127, 87, 14, 0, 1, 3134, 102, 0, 0 },
			//Distorsion 3
			{ 0, 64, 127, 127, 12, 13, 0, 5078, 56, 0, 1 },
			//Guitar Amp
			{ 84, 64, 35, 63, 50, 2, 0, 824, 21, 0, 0 } };

	/**
	 * adjust preset
	 */
	setPvolume(presets[npreset][0]);
	setPpanning(presets[npreset][1]);
	setPlrcross(presets[npreset][2]);
	setPdrive(presets[npreset][3]);
	setPlevel(presets[npreset][4]);
	setPtype(presets[npreset][5]);
	setPnegate(presets[npreset][6]);
	setPlpf(presets[npreset][7]);
	setPhpf(presets[npreset][8]);
	setPstereo(presets[npreset][9]);
	setPprefiltering(presets[npreset][10]);
	setPoctave(presets[npreset][12]);
	cleanup();
}

int YroDistortion::getPhpf() const {
	return Phpf;
}

void YroDistortion::setPhpf(int value) {
	Phpf = value;
	float fr = (float) Phpf;

	hpfl->setfreq(fr);
	hpfr->setfreq(fr);
	//Prefiltering of 51 is approx 630 Hz. 50 - 60 generally good for OD pedal.
}

int YroDistortion::getPlevel() const {
	return Plevel;
}

void YroDistortion::setPlevel(int plevel) {
	Plevel = plevel;
}

int YroDistortion::getPlpf() const {
	return Plpf;
}

void YroDistortion::setPlpf(int value) {
	Plpf = value;
	float fr = (float) Plpf;
	lpfl->setfreq(fr);
	lpfr->setfreq(fr);
}

int YroDistortion::getPoctave() const {
	return Poctave;
}

void YroDistortion::setPoctave(int poctave) {
	this->Poctave = poctave;
	octmix = (float) (Poctave) / 127.0f;
}

int YroDistortion::getPpanning() const {
	return Ppanning;
}

void YroDistortion::setPpanning(int ppanning) {
	this->Ppanning = ppanning;
	panning = ((float) Ppanning + 0.5f) / 127.0f;
}

int YroDistortion::getPvolume() const {
	return Pvolume;
}

void YroDistortion::setPvolume(int pvolume) {
	this->Pvolume = pvolume;

	outvolume = (float) Pvolume / 127.0f;
	if (Pvolume == 0)
		cleanup();
}

int YroDistortion::getPlrcross() const {
	return Plrcross;
}

int YroDistortion::getPtype() const {
	return Ptype;
}

void YroDistortion::setPtype(int ptype) {
	Ptype = ptype;
}

int YroDistortion::getPnegate() const {
	return Pnegate;
}

void YroDistortion::setPnegate(int value) {
	if (value > 1)
		value = 1;
	Pnegate = value;
}

int YroDistortion::getPstereo() const {
	return Pstereo;
}

void YroDistortion::setPstereo(int value) {
	if (value > 1)
		value = 1;
	Pstereo = value;
}

int YroDistortion::getPprefiltering() const {
	return Pprefiltering;
}

void YroDistortion::setPprefiltering(int pprefiltering) {
	Pprefiltering = pprefiltering;
}

