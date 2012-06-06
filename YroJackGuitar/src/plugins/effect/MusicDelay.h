/*
 ZynAddSubFX - a software synthesizer

 Echo.h - Echo Effect
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

#ifndef MUSDELAY_H
#define MUSDELAY_H

#include <plugins/YroEffectPlugin.h>

namespace std {

class MusicDelay: public YroEffectPlugin

{
public:
	MusicDelay();
	~MusicDelay();
	void render(jack_nframes_t nframes, float * smpsl, float * smpr);
	void cleanup();
	const char *toXml();

	/**
	 * member declaration
	 */
enum functions {
		_preset,
		_volume,
		_panning1,
		_delay1,
		_delay3,
		_lrcross,
		_fb1,
		_hidamp,
		_panning2,
		_delay2,
		_fb2,
		_tempo,
		_gain1,
		_gain2
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
		return getPanning1();
	}
	void set2(int value) {
		setPanning1(value);
	}
	int get3() {
		return getDelay1();
	}
	void set3(int value) {
		setDelay1(value);
	}
	int get4() {
		return getDelay3();
	}
	void set4(int value) {
		setDelay3(value);
	}
	int get5() {
		return getLrcross();
	}
	void set5(int value) {
		setLrcross(value);
	}
	int get6() {
		return getFb1();
	}
	void set6(int value) {
		setFb1(value);
	}
	int get7() {
		return getHidamp();
	}
	void set7(int value) {
		setHidamp(value);
	}
	int get8() {
		return getPanning2();
	}
	void set8(int value) {
		setPanning2(value);
	}
	int get9() {
		return getDelay2();
	}
	void set9(int value) {
		setDelay2(value);
	}
	int get10() {
		return getFb2();
	}
	void set10(int value) {
		setFb2(value);
	}
	int get11() {
		return getTempo();
	}
	void set11(int value) {
		setTempo(value);
	}
	int get12() {
		return getGain1();
	}
	void set12(int value) {
		setGain1(value);
	}
	int get13() {
		return getGain2();
	}
	void set13(int value) {
		setGain2(value);
	}

	/**
	 * setter and getter
	 */
	int getVolume();
	void setVolume(int value);
	int getPanning1();
	void setPanning1(int value);
	int getDelay1();
	void setDelay1(int value);
	int getDelay3();
	void setDelay3(int value);
	int getLrcross();
	void setLrcross(int value);
	int getFb1();
	void setFb1(int value);
	int getHidamp();
	void setHidamp(int value);
	int getPanning2();
	void setPanning2(int value);
	int getDelay2();
	void setDelay2(int value);
	int getFb2();
	void setFb2(int value);
	int getTempo();
	void setTempo(int value);
	int getGain1();
	void setGain1(int value);
	int getGain2();
	void setGain2(int value);

private:

	void setPanning(int num, int Ppanning);
	void setDelay(int num, int Pdelay);
	void setGain(int num, int Pgain);
	void setFb(int num, int Pfb);
	void initdelays();

	/**
	 * parameters
	 */
	int Pvolume; //Volumul or E/R
	int Ppanning1; //Panning
	int Ppanning2;
	int Pgain1;
	int Pgain2;
	int Pdelay1;
	int Pdelay2;
	int Plrdelay; // L/R delay difference
	int Plrcross; // L/R Mixing
	int Pfb1; //Feed-back-ul
	int Pfb2;
	int Phidamp;
	int Ptempo;

//Parametrii reali
	int dl1, dr1, dl2, dr2, delay1, delay2, lrdelay;
	int kl1, kr1, kl2, kr2;
	int maxx_delay;
	float panning1, panning2, lrcross, fb1, fb2, hidamp;
	float gain1, gain2;
	float *ldelay1, *rdelay1, *ldelay2, *rdelay2;
	float oldl1, oldr1, oldl2, oldr2; //pt. lpf

};

}
#endif
