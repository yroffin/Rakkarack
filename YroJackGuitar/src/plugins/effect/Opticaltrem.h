/*
 rakarrack - a guitar effects software

 Opticaltrem.h  -  Opticaltrem Effect definitions

 Copyright (C) 2008-2010 Ryan Billing
 Author: Josep Andreu & Ryan Billing

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

#ifndef Opticaltrem_H
#define Opticaltrem_H

#include <plugins/YroEffectPlugin.h>
#include <plugins/misc/YroLowfrequencyOscillation.h>

namespace std {

class Opticaltrem: public YroEffectPlugin {
public:
	Opticaltrem();
	~Opticaltrem();

	void render(jack_nframes_t nframes, float * smpsl, float * smpsr);
	void cleanup();
	const char *toXml();

	/**
	 * member declaration
	 */
	enum functions {
		_preset,
		_depth,
		_lfofreq,
		_lforandomness,
		_lfotype,
		_lfostereo,
		_panning
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
		return getDepth();
	}
	void set1(int value) {
		setDepth(value);
	}
	int get2() {
		return getLfoFreq();
	}
	void set2(int value) {
		setLfoFreq(value);
	}
	int get3() {
		return getLfoRandomness();
	}
	void set3(int value) {
		setLfoRandomness(value);
	}
	int get4() {
		return getLfoType();
	}
	void set4(int value) {
		setLfoType(value);
	}
	int get5() {
		return getLfoStereo();
	}
	void set5(int value) {
		setLfoStereo(value);
	}
	int get6() {
		return getPanning();
	}
	void set6(int value) {
		setPanning(value);
	}

	/**
	 * setter and getter
	 */
	int getDepth();
	void setDepth(int value);
	int getLfoFreq();
	void setLfoFreq(int value);
	int getLfoRandomness();
	void setLfoRandomness(int value);
	int getLfoType();
	void setLfoType(int value);
	int getLfoStereo();
	void setLfoStereo(int value);
	int getPanning();
	void setPanning(int value);

private:
	int Pdepth;
	int Ppanning;

	float Ra, Rb, R1, b, dTC, dRCl, dRCr, minTC, alphal, alphar, stepl, stepr,
			oldstepl, oldstepr, fdepth;
	float lstep, rstep;
	float cperiod;
	float gl, oldgl;
	float gr, oldgr;
	float rpanning, lpanning;
	YroLowfrequencyOscillation lfo;
};

}
#endif
