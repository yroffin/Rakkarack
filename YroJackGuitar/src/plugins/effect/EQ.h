/*
 ZynAddSubFX - a software synthesizer
 
 EQ.h - EQ Effect
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

#ifndef EQ_H
#define EQ_H

#include <plugins/YroEffectPlugin.h>
#include <plugins/filter/AnalogFilter.h>

namespace std {

class EQ: public YroEffectPlugin

{
public:
	EQ();
	~EQ();
	void render(jack_nframes_t nframes, float * smpsl, float * smpr);
	void cleanup();

	/**
	 * member declaration
	 */
	enum functions {
		_preset,
		_volume,
		_type0,
		_freq0,
		_gain0,
		_q0,
		_stages0,
		_type1,
		_freq1,
		_gain1,
		_q1,
		_stages1
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
		return getEqType(0);
	}
	void set2(int value) {
		setEqType(0, value);
	}
	int get3() {
		return getEqFreq(0);
	}
	void set3(int value) {
		setEqFreq(0, value);
	}
	int get4() {
		return getEqGain(0);
	}
	void set4(int value) {
		setEqGain(0, value);
	}
	int get5() {
		return getEqQ(0);
	}
	void set5(int value) {
		setEqQ(0, value);
	}
	int get6() {
		return getEqStages(0);
	}
	void set6(int value) {
		setEqStages(0, value);
	}
	int get7() {
		return getEqType(0);
	}
	void set7(int value) {
		setEqType(0, value);
	}
	int get8() {
		return getEqFreq(0);
	}
	void set8(int value) {
		setEqFreq(0, value);
	}
	int get9() {
		return getEqGain(0);
	}
	void set9(int value) {
		setEqGain(0, value);
	}
	int get10() {
		return getEqQ(0);
	}
	void set10(int value) {
		setEqQ(0, value);
	}
	int get11() {
		return getEqStages(0);
	}
	void set11(int value) {
		setEqStages(0, value);
	}

	void setVolume(int Pvolume);
	int getVolume();

private:
	void setEqType(int nb, int value);
	void setEqFreq(int nb, int value);
	void setEqGain(int nb, int value);
	void setEqQ(int nb, int value);
	void setEqStages(int nb, int value);

	int getEqType(int nb);
	int getEqFreq(int nb);
	int getEqGain(int nb);
	int getEqQ(int nb);
	int getEqStages(int nb);

	float getfreqresponse(float freq);
	int setEq(int nb, int value);
	int getEq(int nb);

	int Pvolume; //Volumul
	struct {
		//parameters
		int Ptype, Pfreq, Pgain, Pq, Pstages;
		//internal values
		AnalogFilter *l, *r;
	} filter[MAX_EQ_BANDS];

};

}
#endif
