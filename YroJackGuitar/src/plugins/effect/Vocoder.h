/*
 Vocoder.h - Vocoder Effect

 Author: Ryam Billing & Josep Andreu

 Adapted effect structure of ZynAddSubFX - a software synthesizer
 Author: Nasca Octavian Paul

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

#ifndef VOCODER_H
#define VOCODER_H

#include <plugins/YroEffectPlugin.h>
#include <plugins/misc/Resample.h>
#include <plugins/filter/AnalogFilter.h>

namespace std {

class Vocoder: public YroEffectPlugin

{
public:
	Vocoder(float *auxresampled_, int bands, int DS, int uq, int dq);
	~Vocoder();
	void render(jack_nframes_t nframes, float * smpsl, float * smpr);
	void cleanup();
	const char *toXml();
	void adjust(int DS);

	/**
	 * member declaration
	 */
	enum functions {
		_preset, _volume, _panning, _Muffle, _Qq, _Input, _level, _Ring
	};

	/**
	 * setter and getter map
	 */
	int get0() {
		return getPreset();
	}
	void set0(int value) {
		setPreset(value);
	}
	int get1() {
		return getVolume();
	}
	void set1(int value) {
		setVolume(value);
	}
	int get2() {
		return getPanning();
	}
	void set2(int value) {
		setPanning(value);
	}
	int get3() {
		return getMuffle();
	}
	void set3(int value) {
		setMuffle(value);
	}
	int get4() {
		return getQq();
	}
	void set4(int value) {
		setQq(value);
	}
	int get5() {
		return getInput();
	}
	void set5(int value) {
		setInput(value);
	}
	int get6() {
		return getLevel();
	}
	void set6(int value) {
		setLevel(value);
	}
	int get7() {
		return getRing();
	}
	void set7(int value) {
		setRing(value);
	}

	/**
	 * setter and getter
	 */
	int getVolume();
	void setVolume(int value);
	int getPanning();
	void setPanning(int value);
	int getMuffle();
	void setMuffle(int value);
	int getQq();
	void setQq(int value);
	int getInput();
	void setInput(int value);
	int getLevel();
	void setLevel(int value);
	int getRing();
	void setRing(int value);

private:
	float outvolume;
	float vulevel;
	float *auxresampled;

	void init_filters();
	void adjustq(float q);
	void setbands(int numbands, float startfreq, float endfreq);
	int VOC_BANDS;
//Parametrii
	int Pvolume; //This is master wet/dry mix like other FX...but I am finding it is not useful
	int Ppanning; //Panning
	int Plrcross; // L/R Mixing  // This is a mono effect, so lrcross and panning are pointless
	int Plevel; //This should only adjust the level of the IR effect, and not wet/dry mix
	int Pinput;
	int Pband;
	int Pmuffle;
	int Pqq;
	int Pring;

	int DS_state;
	int nPERIOD;
	int nSAMPLE_RATE;

	float ncSAMPLE_RATE;
	float nfSAMPLE_RATE;

	double u_up;
	double u_down;

	float ringworm;
	float lpanning, rpanning, input, level;
	float alpha, beta, prls, gate;
	float compeak, compg, compenv, oldcompenv, calpha, cbeta, cthresh, cratio,
			cpthresh;
	float *tmpl, *tmpr;
	float *tsmpsl, *tsmpsr;
	float *tmpaux;
	struct fbank {
		float sfreq, sq, speak, gain, oldgain;
		AnalogFilter *l, *r, *aux;

	}*filterbank;

	AnalogFilter *vhp, *vlp;

	Resample *U_Resample;
	Resample *D_Resample;
	Resample *A_Resample;

};

}
#endif
