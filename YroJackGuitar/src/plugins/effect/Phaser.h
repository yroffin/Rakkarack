/*
 ZynAddSubFX - a software synthesizer

 Phaser.h - Phaser effect
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

#ifndef PHASER_H
#define PHASER_H
#include <plugins/YroEffectPlugin.h>
#include <plugins/misc/YroLowfrequencyOscillation.h>

namespace std {

class Phaser: public YroEffectPlugin

{
public:
	Phaser();
	~Phaser();
	void render(jack_nframes_t nframes, float * smpsl, float * smpsr);
	void cleanup();
	const char *toXml();

	/**
	 * member declaration
	 */
enum functions {
		_preset,
		_volume,
		_panning,
		_LfoPfreq,
		_LfoPrandomness,
		_LfoPlfOtype,
		_LfoPstereo,
		_depth,
		_fb,
		_stages,
		_lrcross,
		_outsub,
		_phase
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
		return getLfoPfreq();
	}
	void set3(int value) {
		setLfoPfreq(value);
	}
	int get4() {
		return getLfoPrandomness();
	}
	void set4(int value) {
		setLfoPrandomness(value);
	}
	int get5() {
		return getLfoPlfOtype();
	}
	void set5(int value) {
		setLfoPlfOtype(value);
	}
	int get6() {
		return getLfoPstereo();
	}
	void set6(int value) {
		setLfoPstereo(value);
	}
	int get7() {
		return getDepth();
	}
	void set7(int value) {
		setDepth(value);
	}
	int get8() {
		return getFb();
	}
	void set8(int value) {
		setFb(value);
	}
	int get9() {
		return getStages();
	}
	void set9(int value) {
		setStages(value);
	}
	int get10() {
		return getLrcross();
	}
	void set10(int value) {
		setLrcross(value);
	}
	int get11() {
		return getOutsub();
	}
	void set11(int value) {
		setOutsub(value);
	}
	int get12() {
		return getPhase();
	}
	void set12(int value) {
		setPhase(value);
	}

	/**
	 * setter and getter
	 */
	int getVolume();
	void setVolume(int value);
	int getPanning();
	void setPanning(int value);
	int getLfoPfreq();
	void setLfoPfreq(int value);
	int getLfoPrandomness();
	void setLfoPrandomness(int value);
	int getLfoPlfOtype();
	void setLfoPlfOtype(int value);
	int getLfoPstereo();
	void setLfoPstereo(int value);
	int getDepth();
	void setDepth(int value);
	int getFb();
	void setFb(int value);
	int getStages();
	void setStages(int value);
	int getLrcross();
	void setLrcross(int value);
	int getOutsub();
	void setOutsub(int value);
	int getPhase();
	void setPhase(int value);

private:
	float outvolume;

//Parametrii Phaser
	int Pvolume;
	int Ppanning;
	int Pdepth; //the depth of the Phaser
	int Pfb; //feedback
	int Plrcross; //feedback
	int Pstages;
	int Poutsub; //if I wish to substract the output instead of the adding it
	int Pphase;

//Control Parametrii

//Valorile interne
	float panning, fb, depth, lrcross, fbl, fbr, phase;
	float *oldl, *oldr;
	float oldlgain, oldrgain;

	YroLowfrequencyOscillation lfo; //lfo-ul Phaser

};

}
#endif
