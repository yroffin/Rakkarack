/*
 rakarrack - a guitar effects software

 PitchShifter.h  -  Shifterr definitions
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

#ifndef SHIFTER_H
#define SHIFTER_H

#include <plugins/YroEffectPlugin.h>
#include <plugins/misc/Resample.h>
#include <plugins/misc/PitchShifter.h>

#define IDLE 0
#define UP   1
#define WAIT 2
#define DOWN 3

namespace std {

class Shifter: public YroEffectPlugin

{

public:
	Shifter(long int Quality, int DS, int uq, int dq);
	~Shifter();
	void render(jack_nframes_t nframes, float *smpsl, float *smpsr);
	void cleanup();
	const char *toXml();
	void applyfilters(float * efxoutl);
	void adjust(int DS);

	/**
	 * member declaration
	 */
	enum functions {
		_preset,
		_volume,
		_panning,
		_gain,
		_attack,
		_decay,
		_threshold,
		_interval,
		_updown,
		_mode,
		_whammy
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
		return getGain();
	}
	void set3(int value) {
		setGain(value);
	}
	int get4() {
		return getAttack();
	}
	void set4(int value) {
		setAttack(value);
	}
	int get5() {
		return getDecay();
	}
	void set5(int value) {
		setDecay(value);
	}
	int get6() {
		return getThreshold();
	}
	void set6(int value) {
		setThreshold(value);
	}
	int get7() {
		return getInterval();
	}
	void set7(int value) {
		setInterval(value);
	}
	int get8() {
		return getUpdown();
	}
	void set8(int value) {
		setUpdown(value);
	}
	int get9() {
		return getMode();
	}
	void set9(int value) {
		setMode(value);
	}
	int get10() {
		return getWhammy();
	}
	void set10(int value) {
		setWhammy(value);
	}

	/**
	 * setter and getter
	 */
	int getVolume();
	void setVolume(int value);
	int getPanning();
	void setPanning(int value);
	int getGain();
	void setGain(int value);
	int getAttack();
	void setAttack(int value);
	int getDecay();
	void setDecay(int value);
	int getThreshold();
	void setThreshold(int value);
	int getInterval();
	void setInterval(int value);
	int getUpdown();
	void setUpdown(int value);
	int getMode();
	void setMode(int value);
	int getWhammy();
	void setWhammy(int value);

private:

	long int hq;
	float outvolume;
	float *outi;
	float *outo;

	void fixinterval(int Pinterval);

	int Pvolume;
	int Pgain;
	int Ppan;
	int Pinterval;
	int Pupdown;
	int Pmode;
	int Pattack;
	int Pdecay;
	int Pthreshold;
	int Pwhammy;
	int state;
	int DS_state;
	int nPERIOD;
	int nSAMPLE_RATE;
	long window;

	double u_up;
	double u_down;
	float nfSAMPLE_RATE;
	float env, t_level, td_level, tz_level;
	float a_rate, d_rate, tune, range, whammy;
	float panning;
	float gain;
	float interval;
	float *templ, *tempr;

	Resample *U_Resample;
	Resample *D_Resample;

	PitchShifter *PS;

};

}
#endif
