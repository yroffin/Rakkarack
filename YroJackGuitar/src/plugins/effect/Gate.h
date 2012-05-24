// Based in gate_1410.c LADSPA Swh-plugins

/*
 rakarrack - a guitar effects software

 Gate.h  -  Noise Gate Effect definitions
 Based on Steve Harris LADSPA gate.
 
 Copyright (C) 2008 Josep Andreu
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

#ifndef NOISEGATE_H
#define NOISEGATE_H

#include <plugins/YroEffectPlugin.h>
#include <plugins/filter/AnalogFilter.h>

namespace std {

class Gate: public YroEffectPlugin {
public:
	Gate();
	~Gate();
	void render(jack_nframes_t nframes, float * smps_l, float * smps_r);
	void cleanup();

	/**
	 * member declaration
	 */
	enum functions {
		_preset, _threshold, _range, _attack, _decay, _lpf, _hpf, _hold
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
		return getThreshold();
	}
	void set1(int value) {
		setThreshold(value);
	}
	int get2() {
		return getRange();
	}
	void set2(int value) {
		setRange(value);
	}
	int get3() {
		return getAttack();
	}
	void set3(int value) {
		setAttack(value);
	}
	int get4() {
		return getDecay();
	}
	void set4(int value) {
		setDecay(value);
	}
	int get5() {
		return getLpf();
	}
	void set5(int value) {
		setLpf(value);
	}
	int get6() {
		return getHpf();
	}
	void set6(int value) {
		setHpf(value);
	}
	int get7() {
		return getHold();
	}
	void set7(int value) {
		setHold(value);
	}

	/**
	 * setter and getter
	 */
	int getThreshold();
	void setThreshold(int value);
	int getRange();
	void setRange(int value);
	int getAttack();
	void setAttack(int value);
	int getDecay();
	void setDecay(int value);
	int getLpf();
	void setLpf(int value);
	int getHpf();
	void setHpf(int value);
	int getHold();
	void setHold(int value);

private:
	int Pthreshold; // attack time  (ms)
	int Pattack; // release time (ms)
	int Ohold;
	int Pdecay;
	int Prange;
	int Plpf;
	int Phpf;
	int Phold;

	float *efxoutl;
	float *efxoutr;

private:
	int hold_count;
	int state;
	float range;
	float cut;
	float t_level;
	float a_rate;
	float d_rate;
	float env;
	float gate;
	float fs;
	float hold;

	AnalogFilter *lpfl, *lpfr, *hpfl, *hpfr;
};

}
#endif
