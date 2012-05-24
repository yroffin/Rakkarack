/*
 ZynAddSubFX - a software synthesizer
 
 AnalogPhaser.h - Phaser effect
 Copyright (C) 2002-2005 Nasca Octavian Paul
 Author: Nasca Octavian Paul

 Modified for rakarrack by Josep Andreu and Ryan Billing
 Modified by Yannick Roffin

 Further modified for rakarrack by Ryan Billing (Transmogrifox) to model Analog Phaser behavior 2009

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

#ifndef APHASER_H
#define APHASER_H
#include <plugins/YroEffectPlugin.h>
#include <plugins/misc/YroLowfrequencyOscillation.h>

namespace std {

class AnalogPhaser: public YroEffectPlugin

{
public:
	AnalogPhaser();
	~AnalogPhaser();
	void render(jack_nframes_t nframes, float * smpsl, float * smpsr);
	void cleanup();

	/**
	 * member declaration
	 */
	enum functions {
		_preset,
		_volume,
		_distortion,
		_LfoPfreq,
		_LfoPrandomness,
		_LfoPlfOtype,
		_LfoPstereo,
		_width,
		_fb,
		_stages,
		_off,
		_outsub,
		_depth,
		_hyper
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
		return getDistortion();
	}

	void set2(int value) {
		setDistortion(value);
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
		return getWidth();
	}

	void set7(int value) {
		setWidth(value);
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
		return getOffset();
	}

	void set10(int value) {
		setOffset(value);
	}
	int get11() {
		return getOutsub();
	}
	void set11(int value) {
		setOutsub(value);
	}
	int get12() {
		return getDepth();
	}
	void set12(int value) {
		setDepth(value);
	}
	int get13() {
		return getHyper();
	}
	void set13(int value) {
		setHyper(value);
	}

	/**
	 * setter and getter
	 */
	int getVolume();
	void setVolume(int value);
	int getDistortion();
	void setDistortion(int value);
	int getLfoPfreq();
	void setLfoPfreq(int value);
	int getLfoPrandomness();
	void setLfoPrandomness(int value);
	int getLfoPlfOtype();
	void setLfoPlfOtype(int value);
	int getLfoPstereo();
	void setLfoPstereo(int value);
	int getWidth();
	void setWidth(int value);
	int getFb();
	void setFb(int value);
	int getStages();
	void setStages(int value);
	int getOffset();
	void setOffset(int value);
	int getOutsub();
	void setOutsub(int value);
	int getDepth();
	void setDepth(int value);
	int getHyper();
	void setHyper(int value);

private:
	float outvolume;
	//Phaser parameters
	YroLowfrequencyOscillation lfo; //Phaser modulator
	int Pvolume; //Used in Process.C to set wet/dry mix
	int Pdistortion; //Model distortion added by FET element
	int Pwidth; //Phaser width (LFO amplitude)
	int Pfb; //feedback
	int Poffset; //Model mismatch between variable resistors
	int Pstages; //Number of first-order All-Pass stages
	int Poutsub; //if I wish to subtract the output instead of the adding it
	int Phyper; //lfo^2 -- converts tri into hyper-sine
	int Pdepth; //Depth of phaser sweep
	int Pbarber; //Enable barber pole phasing

	//Internal Variables
	bool barber; //Barber pole phasing flag
	float distortion, fb, width, offsetpct, fbl, fbr, depth;
	float *lxn1, *lyn1, *rxn1, *ryn1, *offset;
	float oldlgain, oldrgain, rdiff, ldiff, invperiod;

	float mis;
	float Rmin; // 2N5457 typical on resistance at Vgs = 0
	float Rmax; // Resistor parallel to FET
	float Rmx; // Rmin/Rmax to avoid division in loop
	float Rconst; // Handle parallel resistor relationship
	float C; // Capacitor
	float CFs; // A constant derived from capacitor and resistor relationships

	class FPreset *Fpre;

};

}
#endif
