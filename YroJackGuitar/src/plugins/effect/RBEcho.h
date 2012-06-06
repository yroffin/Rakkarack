/*
 ZynAddSubFX - a software synthesizer

 Echo.h - Echo Effect
 Copyright (C) 2002-2005 Nasca Octavian Paul
 Author: Nasca Octavian Paul

 Modified for rakarrack by Josep Andreu

 Reverse Echo by Transmogrifox

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

#ifndef RBECHO_H
#define RBECHO_H

#include <plugins/YroEffectPlugin.h>

namespace std {

class RBEcho: public YroEffectPlugin

{
public:
	RBEcho();
	~RBEcho();
	void render(jack_nframes_t nframes, float * smpsl, float * smpr);
	void cleanup();
	const char *toXml();

	/**
	 * member declaration
	 */
enum functions {
		_preset,
		_volume,
		_panning,
		_delay,
		_lrdelay,
		_lrcross,
		_fb,
		_hidamp,
		_reverse,
		_subdiv,
		_es
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
		return getDelay();
	}
	void set3(int value) {
		setDelay(value);
	}
	int get4() {
		return getLrdelay();
	}
	void set4(int value) {
		setLrdelay(value);
	}
	int get5() {
		return getLrcross();
	}
	void set5(int value) {
		setLrcross(value);
	}
	int get6() {
		return getFb();
	}
	void set6(int value) {
		setFb(value);
	}
	int get7() {
		return getHidamp();
	}
	void set7(int value) {
		setHidamp(value);
	}
	int get8() {
		return getReverse();
	}
	void set8(int value) {
		setReverse(value);
	}
	int get9() {
		return getSubdiv();
	}
	void set9(int value) {
		setSubdiv(value);
	}
	int get10() {
		return getEs();
	}
	void set10(int value) {
		setEs(value);
	}

	/**
	 * setter and getter
	 */
	int getVolume();
	void setVolume(int value);
	int getPanning();
	void setPanning(int value);
	int getDelay();
	void setDelay(int value);
	int getLrdelay();
	void setLrdelay(int value);
	int getLrcross();
	void setLrcross(int value);
	int getFb();
	void setFb(int value);
	int getHidamp();
	void setHidamp(int value);
	int getReverse();
	void setReverse(int value);
	int getSubdiv();
	void setSubdiv(int value);
	int getEs();
	void setEs(int value);

private:

	void setvolume(int Pvolume);
	void setpanning(int Ppanning);
	void setdelay(int Pdelay);
	void setlrdelay(int Plrdelay);
	void setlrcross(int Plrcross);
	void setfb(int Pfb);
	void sethidamp(int Phidamp);
	void setreverse(int Preverse);
	void initdelays();

//Parametrii
	int Pvolume; //Volumul or E/R
	int Ppanning; //Panning
	int Pdelay;
	int Plrdelay; // L/R delay difference
	int Plrcross; // L/R Mixing
	int Pfb; //Feed-back-ul
	int Phidamp;
	int Preverse;
	int Psubdiv;
	int Pes;

//Parametrii reali
	int dl, dr, delay, lrdelay;
	int kl, kr, rkr, rkl, rvkl, rvkr, rvfl, rvfr, maxx_delay;

	float fdelay, subdiv, pes, pingpong;
	float rpanning, lpanning, lrcross, fb, hidamp, reverse, lfeedback,
			rfeedback;
	float *ldelay, *rdelay;
	float oldl, oldr; //pt. lpf
	float Srate_Attack_Coeff;

};

}
#endif
