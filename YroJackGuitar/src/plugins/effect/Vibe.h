/*
 rakarrack - a guitar effects software

 Vibe.h  -  Vibe Effect definitions

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

#ifndef Vibe_H
#define Vibe_H

#include <plugins/YroEffectPlugin.h>
#include <plugins/misc/YroLowfrequencyOscillation.h>

namespace std {

class Vibe: public YroEffectPlugin

{

public:

	Vibe();
	~Vibe();

	void render(jack_nframes_t nframes, float * smpsl, float * smpsr);
	void cleanup();

	/**
	 * member declaration
	 */
	enum functions {
		_preset,
		_width,
		_LfoPfreq,
		_LfoPrandomness,
		_LfoPlfOtype,
		_LfoPstereo,
		_panning,
		_volume,
		_fb,
		_depth,
		_lrcross
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
		return getWidth();
	}
	void set1(int value) {
		setWidth(value);
	}
	int get2() {
		return getLfoPfreq();
	}
	void set2(int value) {
		setLfoPfreq(value);
	}
	int get3() {
		return getLfoPrandomness();
	}
	void set3(int value) {
		setLfoPrandomness(value);
	}
	int get4() {
		return getLfoPlfOtype();
	}
	void set4(int value) {
		setLfoPlfOtype(value);
	}
	int get5() {
		return getLfoPstereo();
	}
	void set5(int value) {
		setLfoPstereo(value);
	}
	int get6() {
		return getPanning();
	}
	void set6(int value) {
		setPanning(value);
	}
	int get7() {
		return getVolume();
	}
	void set7(int value) {
		setVolume(value);
	}
	int get8() {
		return getFb();
	}
	void set8(int value) {
		setFb(value);
	}
	int get9() {
		return getDepth();
	}
	void set9(int value) {
		setDepth(value);
	}
	int get10() {
		return getLrcross();
	}
	void set10(int value) {
		setLrcross(value);
	}

	/**
	 * setter and getter
	 */
	int getWidth();
	void setWidth(int value);
	int getLfoPfreq();
	void setLfoPfreq(int value);
	int getLfoPrandomness();
	void setLfoPrandomness(int value);
	int getLfoPlfOtype();
	void setLfoPlfOtype(int value);
	int getLfoPstereo();
	void setLfoPstereo(int value);
	int getPanning();
	void setPanning(int value);
	int getVolume();
	void setVolume(int value);
	int getFb();
	void setFb(int value);
	int getDepth();
	void setDepth(int value);
	int getLrcross();
	void setLrcross(int value);

private:
	float outvolume;
	int Pwidth;
	int Pfb;
	int Plrcross;
	int Pdepth;
	int Ppanning;
	int Pvolume;

	float fwidth;
	float fdepth;
	float rpanning, lpanning;
	float flrcross, fcross;
	float fb;
	YroLowfrequencyOscillation lfo;

	float Ra, Rb, b, dTC, dRCl, dRCr, lampTC, ilampTC, minTC, alphal, alphar,
			stepl, stepr, oldstepl, oldstepr;
	float fbr, fbl;
	float dalphal, dalphar;
	float lstep, rstep;
	float cperiod;
	float gl, oldgl;
	float gr, oldgr;

	class fparams {
	public:
		float x1;
		float y1;
		//filter coefficients
		float n0;
		float n1;
		float d0;
		float d1;
	} vc[8], vcvo[8], ecvc[8], vevo[8], bootstrap[8];

	float vibefilter(float data, fparams *ftype, int stage);
	void init_vibes();
	void modulate(float ldrl, float ldrr);
	float bjt_shape(float data);

	float R1;
	float Rv;
	float C2;
	float C1[8];
	float beta; //transistor forward gain.
	float gain, k;
	float oldcvolt[8];
	float en1[8], en0[8], ed1[8], ed0[8];
	float cn1[8], cn0[8], cd1[8], cd0[8];
	float ecn1[8], ecn0[8], ecd1[8], ecd0[8];
	float on1[8], on0[8], od1[8], od0[8];

};

}
#endif
