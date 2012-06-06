/*

 MBDist.h - Distorsion Effect

 Copyright (C) 2002-2005 Nasca Octavian Paul
 Author: Nasca Octavian Paul
 ZynAddSubFX - a software synthesizer

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

#ifndef MBVVOL_H
#define MBVVOL_H

#include <plugins/YroEffectPlugin.h>
#include <plugins/filter/AnalogFilter.h>
#include <plugins/misc/YroLowfrequencyOscillation.h>

namespace std {

class MBVvol: public YroEffectPlugin

{
public:
	MBVvol();
	~MBVvol();
	void render(jack_nframes_t nframes, float * smpsl, float * smpr);
	void cleanup();
	const char *toXml();

	/**
	 * member declaration
	 */
enum functions {
		_preset,
		_volume,
		_lfopfreq1,
		_lfoplfotype1,
		_lfopstereo1,
		_lfopfreq2,
		_lfoplfotype2,
		_lfopstereo2,
		_cross1,
		_cross2,
		_cross3,
		_combi
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
		return getLfoFreq1();
	}
	void set2(int value) {
		setLfoFreq1(value);
	}
	int get3() {
		return getLfoType1();
	}
	void set3(int value) {
		setLfoType1(value);
	}
	int get4() {
		return getLfoStereo1();
	}
	void set4(int value) {
		setLfoStereo1(value);
	}
	int get5() {
		return getLfoFreq2();
	}
	void set5(int value) {
		setLfoFreq2(value);
	}
	int get6() {
		return getLfoType2();
	}
	void set6(int value) {
		setLfoType2(value);
	}
	int get7() {
		return getLfoStereo2();
	}
	void set7(int value) {
		setLfoStereo2(value);
	}
	int get8() {
		return getCross1();
	}
	void set8(int value) {
		setCross1(value);
	}
	int get9() {
		return getCross2();
	}
	void set9(int value) {
		setCross2(value);
	}
	int get10() {
		return getCross3();
	}
	void set10(int value) {
		setCross3(value);
	}
	int get11() {
		return getCombi();
	}
	void set11(int value) {
		setCombi(value);
	}

	/**
	 * setter and getter
	 */
	int getVolume();
	void setVolume(int value);
	int getLfoFreq1();
	void setLfoFreq1(int value);
	int getLfoType1();
	void setLfoType1(int value);
	int getLfoStereo1();
	void setLfoStereo1(int value);
	int getLfoFreq2();
	void setLfoFreq2(int value);
	int getLfoType2();
	void setLfoType2(int value);
	int getLfoStereo2();
	void setLfoStereo2(int value);
	int getCross1();
	void setCross1(int value);
	int getCross2();
	void setCross2(int value);
	int getCross3();
	void setCross3(int value);
	int getCombi();
	void setCombi(int value);

private:
	float *lowl;
	float *lowr;
	float *midll;
	float *midlr;
	float *midhl;
	float *midhr;
	float *highl;
	float *highr;

	/**
	 * parameters
	 */
	int Pvolume; //Volumul or E/R
	int Pcombi;
	int Cross1;
	int Cross2;
	int Cross3;

//Parametrii reali

	float coeff;
	float lfo1l, lfo1r, lfo2l, lfo2r;
	float v1l, v1r, v2l, v2r;
	float d1, d2, d3, d4;
	float volL, volML, volMH, volH;
	float volLr, volMLr, volMHr, volHr;
	AnalogFilter *lpf1l, *lpf1r, *hpf1l, *hpf1r;
	AnalogFilter *lpf2l, *lpf2r, *hpf2l, *hpf2r;
	AnalogFilter *lpf3l, *lpf3r, *hpf3l, *hpf3r;

	YroLowfrequencyOscillation lfo1, lfo2;

};

}
#endif
