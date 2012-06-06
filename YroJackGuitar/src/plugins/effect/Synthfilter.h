/*
 ZynAddSubFX - a software synthesizer
 Synthfilter.h - Synthesizer filter effect
 Copyright (C) 2010 Ryan Billing
 Based on Analog Phaser  derived from
 Phaser.h/.C
 Copyright (C) 2002-2005 Nasca Octavian Paul

 Authors: Nasca Octavian Paul, Ryan Billing, Josep Andreu

 Modified for rakarrack by Josep Andreu

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

#ifndef SYNTHFILTER_H
#define SYNTHFILTER_H
#include <plugins/YroEffectPlugin.h>
#include <plugins/misc/YroLowfrequencyOscillation.h>

namespace std {

class Synthfilter: public YroEffectPlugin

{
public:
	Synthfilter();
	~Synthfilter();
	void render(jack_nframes_t nframes, float * smpsl, float * smpsr);
	void cleanup();
	const char *toXml();

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
		_pstage,
		_hpstages,
		_outsub,
		_depth,
		_envelope,
		_attack,
		_release,
		_bandwith
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
		return getPstage();
	}
	void set9(int value) {
		setPstage(value);
	}
	int get10() {
		return getHpstages();
	}
	void set10(int value) {
		setHpstages(value);
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
		return getEnvelope();
	}
	void set13(int value) {
		setEnvelope(value);
	}
	int get14() {
		return getAttack();
	}
	void set14(int value) {
		setAttack(value);
	}
	int get15() {
		return getRelease();
	}
	void set15(int value) {
		setRelease(value);
	}
	int get16() {
		return getBandwidth();
	}
	void set16(int value) {
		setBandwidth(value);
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
	int getPstage();
	void setPstage(int value);
	int getHpstages();
	void setHpstages(int value);
	int getOutsub();
	void setOutsub(int value);
	int getDepth();
	void setDepth(int value);
	int getEnvelope();
	void setEnvelope(int value);
	int getAttack();
	void setAttack(int value);
	int getRelease();
	void setRelease(int value);
	int getBandwidth();
	void setBandwidth(int value);

private:
	float outvolume;

//Phaser parameters
	int Pvolume; //0//Used in Process.C to set wet/dry mix
	int Pdistortion; //1//0...127//Model distortion
//2//Tempo//LFO frequency
//3//0...127//LFO Random
//4//0...max types//LFO Type
//5//0...127//LFO stereo offset
	int Pwidth; //6//0...127//Phaser width (LFO amplitude)
	int Pfb; //7//-64...64//feedback
	int Plpstages; //8//0...12//Number of first-order Low-Pass stages
	int Phpstages; //9//0...12//Number of first-order High-Pass stages
	int Poutsub; //10//0 or 1//subtract the output instead of the adding it
	int Pdepth; //11//0...127//Depth of phaser sweep
	int Penvelope; //12//-64...64//envelope sensitivity
	int Pattack; //13//0...1000ms//Attack Time
	int Prelease; //14//0...500ms//Release Time
	int Pbandwidth; //15//0...127//Separate high pass & low pass

//Internal Variables
	float distortion, fb, width, env, envdelta, sns, att, rls, fbl, fbr, depth,
			bandgain;
	float *lyn1, *ryn1, *lx1hp, *ly1hp, *rx1hp, *ry1hp;
	float oldlgain, oldrgain, inv_period;

	float delta;
	float Rmin; // 2N5457 typical on resistance at Vgs = 0
	float Rmax; // Resistor parallel to FET
	float C, Clp, Chp; // Capacitor
	YroLowfrequencyOscillation lfo; //Filter modulator

};

}
#endif
