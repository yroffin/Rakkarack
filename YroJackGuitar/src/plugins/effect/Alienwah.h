/*
 ZynAddSubFX - a software synthesizer

 Alienwah.h - "AlienWah" effect
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

#ifndef ALIENWAH_H
#define ALIENWAH_H
#include <plugins/YroEffectPlugin.h>
#include <plugins/misc/YroLowfrequencyOscillation.h>

namespace std {

class Alienwah: public YroEffectPlugin

{
public:
	Alienwah();
	~Alienwah();
	void render(jack_nframes_t nframes, float * smpsl, float * smpsr);

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
		_delay,
		_lrcross,
		_phase
	};

	/**
	 * setter and getter
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
	;
	void set2(int value) {
		setPanning(value);
	}
	;
	int get3() {
		return getLfoPfreq();
	}
	;
	void set3(int value) {
		setLfoPfreq(value);
	}
	;
	int get4() {
		return getLfoPrandomness();
	}
	;
	void set4(int value) {
		setLfoPrandomness(value);
	}
	;
	int get5() {
		return getLfoPlfOtype();
	}
	;
	void set5(int value) {
		setLfoPlfOtype(value);
	}
	;
	int get6() {
		return getLfoPstereo();
	}
	;
	void set6(int value) {
		setLfoPstereo(value);
	}
	;
	int get7() {
		return getDepth();
	}
	;
	void set7(int value) {
		setDepth(value);
	}
	;
	int get8() {
		return getFb();
	}
	;
	void set8(int value) {
		setFb(value);
	}
	;
	int get9() {
		return getDelay();
	}
	;
	void set9(int value) {
		setDelay(value);
	}
	;
	int get10() {
		return getLrcross();
	}
	;
	void set10(int value) {
		setLrcross(value);
	}
	;
	int get11() {
		return getPhase();
	}
	;
	void set11(int value) {
		setPhase(value);
	}
	;

	void cleanup();
	const char *toXml();

private:
	float outvolume;

	struct COMPLEXTYPE {
		float a, b;
	};

//Parametrii Alienwah
	YroLowfrequencyOscillation lfo; //lfo-ul Alienwah
	int Pvolume;
	int Ppanning;
	int Pdepth; //the depth of the Alienwah
	int Pfb; //feedback
	int Plrcross; //feedback
	int Pdelay;
	int Pphase;

	int oldk;
	int oldpdelay;

	/**
	 * control
	 */
	void setVolume(int Pvolume);
	void setPanning(int Ppanning);
	void setDepth(int Pdepth);
	void setFb(int Pfb);
	void setLrcross(int Plrcross);
	void setDelay(int Pdelay);
	void setPhase(int Pphase);
	int getVolume();
	int getPanning();
	int getDepth();
	int getFb();
	int getLrcross();
	int getDelay();
	int getPhase();

	int getLfoPstereo();
	int getLfoPlfOtype();
	int getLfoPrandomness();
	int getLfoPfreq();
	void setLfoPstereo(int);
	void setLfoPlfOtype(int);
	void setLfoPrandomness(int);
	void setLfoPfreq(int);

//Valorile interne
	float panning, fb, depth, lrcross, phase;
	struct COMPLEXTYPE oldl[MAX_ALIENWAH_DELAY], oldr[MAX_ALIENWAH_DELAY];
	COMPLEXTYPE oldclfol, oldclfor;
};

}
#endif
