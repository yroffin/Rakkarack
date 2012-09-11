/*
 Rakarrack   Audio FX software
 CoilCrafter.h - Pick Up Emulation
 Using RBFilter
 Using Steve Harris LADSPA Plugin harmonic_gen
 Modified for rakarrack by Ryan Billing & Josep Andreu

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

#ifndef COILCRAFTER_H
#define COILCRAFTER_H

#include <plugins/YroRawEffectPlugin.h>
#include <plugins/filter/AnalogFilter.h>
#include <plugins/filter/HarmEnhancer.h>

namespace std {

class CoilCrafter: public YroRawEffectPlugin {
public:
	CoilCrafter();
	~CoilCrafter();
	void render(jack_nframes_t nframes, float * smpsl, float * smpr);
	void cleanup();
	const char *toXml();

	/**
	 * member declaration
	 */
	enum functions {
		_preset, _volume, _po, _pd, _freq1, _q1, _freq2, _q2, _tone, _mode
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
		return getPo();
	}
	void set2(int value) {
		setPo(value);
	}
	int get3() {
		return getPd();
	}
	void set3(int value) {
		setPd(value);
	}
	int get4() {
		return getFreq1();
	}
	void set4(int value) {
		setFreq1(value);
	}
	int get5() {
		return getQ1();
	}
	void set5(int value) {
		setQ1(value);
	}
	int get6() {
		return getFreq2();
	}
	void set6(int value) {
		setFreq2(value);
	}
	int get7() {
		return getQ2();
	}
	void set7(int value) {
		setQ2(value);
	}
	int get8() {
		return getTone();
	}
	void set8(int value) {
		setTone(value);
	}
	int get9() {
		return getMode();
	}
	void set9(int value) {
		setMode(value);
	}

	/**
	 * setter and getter
	 */
	int getVolume();
	void setVolume(int value);
	int getPo();
	void setPo(int value);
	int getPd();
	void setPd(int value);
	int getFreq1();
	void setFreq1(int value);
	int getQ1();
	void setQ1(int value);
	int getFreq2();
	void setFreq2(int value);
	int getQ2();
	void setQ2(int value);
	int getTone();
	void setTone(int value);
	int getMode();
	void setMode(int value);

private:

	void setHpf(int value);

	int Pvolume;
	int Ppo;
	int Ppd;
	int Ptone;
	int Pq1;
	int Pfreq1;
	int Pq2;
	int Pfreq2;
	int Pmode;

	float tfreqs[10];
	float tqs[10];
	float rm[10];
	float freq1, q1, freq2, q2;
	float att;

	HarmEnhancer *harm;
	AnalogFilter *RB1l, *RB1r;
	AnalogFilter *RB2l, *RB2r;

	class FPreset *Fpre;

};

}
#endif
