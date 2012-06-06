/*
 Rakarrack Audio FX

 DualFlanger.h - Super Flanger
 Copyright (C) 2010 Ryan Billing
 Authors:
 Ryan Billing (a.k.a Transmogrifox)  --  Signal Processing
 Copyright (C) 2010 Ryan Billing

 Nasca Octavian Paul -- Remnants of ZynAddSubFX Echo.h structure and utility routines common to ZynSubFX source
 Copyright (C) 2002-2005 Nasca Octavian Paul

 Higher intensity flanging accomplished by picking two points out of the delay line to create a wider notch filter.

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

#ifndef DUAL_FLANGE_H
#define DUAL_FLANGE_H

#include <plugins/YroEffectPlugin.h>
#include <plugins/misc/YroLowfrequencyOscillation.h>

namespace std {

class DualFlanger: public YroEffectPlugin {
public:
	DualFlanger();
	~DualFlanger();
	void render(jack_nframes_t nframes, float * smpsl, float * smpr);
	void cleanup();
	const char *toXml();

	/**
	 * member declaration
	 */
enum functions {
		_preset,
		_wetdry,
		_panning,
		_lrcross,
		_depth,
		_width,
		_off,
		_fb,
		_hidamp,
		_subtract,
		_zero,
		_lfofreq,
		_lfostereo,
		_lfotype,
		_lforandomness
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
		return getWetdry();
	}
	void set1(int value) {
		setWetdry(value);
	}
	int get2() {
		return getPanning();
	}
	void set2(int value) {
		setPanning(value);
	}
	int get3() {
		return getLrcross();
	}
	void set3(int value) {
		setLrcross(value);
	}
	int get4() {
		return getDepth();
	}
	void set4(int value) {
		setDepth(value);
	}
	int get5() {
		return getWidth();
	}
	void set5(int value) {
		setWidth(value);
	}
	int get6() {
		return getOff();
	}
	void set6(int value) {
		setOff(value);
	}
	int get7() {
		return getFb();
	}
	void set7(int value) {
		setFb(value);
	}
	int get8() {
		return getHidamp();
	}
	void set8(int value) {
		setHidamp(value);
	}
	int get9() {
		return getSubtract();
	}
	void set9(int value) {
		setSubtract(value);
	}
	int get10() {
		return getZero();
	}
	void set10(int value) {
		setZero(value);
	}
	int get11() {
		return getLfoFreq();
	}
	void set11(int value) {
		setLfoFreq(value);
	}
	int get12() {
		return getLfoStereo();
	}
	void set12(int value) {
		setLfoStereo(value);
	}
	int get13() {
		return getLfoType();
	}
	void set13(int value) {
		setLfoType(value);
	}
	int get14() {
		return getLfoRandomness();
	}
	void set14(int value) {
		setLfoRandomness(value);
	}

	/**
	 * setter and getter
	 */
	int getWetdry();
	void setWetdry(int value);
	int getPanning();
	void setPanning(int value);
	int getLrcross();
	void setLrcross(int value);
	int getDepth();
	void setDepth(int value);
	int getWidth();
	void setWidth(int value);
	int getOff();
	void setOff(int value);
	int getFb();
	void setFb(int value);
	int getHidamp();
	void setHidamp(int value);
	int getSubtract();
	void setSubtract(int value);
	int getZero();
	void setZero(int value);
	int getLfoFreq();
	void setLfoFreq(int value);
	int getLfoStereo();
	void setLfoStereo(int value);
	int getLfoType();
	void setLfoType(int value);
	int getLfoRandomness();
	void setLfoRandomness(int value);

private:
//Parameters
	int Pwetdry; // 0 //Wet/Dry mix.  Range -64 to 64
	int Ppanning; // 1 //Panning.  Range -64 to 64
	int Plrcross; // 2 //L/R Mixing.  Range 0 to 127
	int Pdepth; // 3 //Max delay deviation expressed as frequency of lowest frequency notch.  Min = 20, Max = 4000
	int Pwidth; // 4 //LFO amplitude.  Range 0 to 16000 (Hz)
	int Poffset; // 5 //Offset of notch 1 to notch 2.  Range 0 to 100 (percent)
	int Pfb; // 6 //Feedback parameter.  Range -64 to 64
	int Phidamp; // 7 //Lowpass filter delay line.  Range 20 to 20000 (Hz)
	int Psubtract; // 8 //Subtract wet/dry instead of add.  Nonzero is true
	int Pzero; // 9 //Enable through-zero flanging,   Nonzero is true
// 10 //LFO Speed
// 11 //LFO stereo diff
// 12 //LFO type
// 13 //LFO Randomness

	float wet, dry; //Wet/Dry mix.
	float lpan, rpan; //Panning.
	float flrcross, frlcross; // L/R Mixing.
	float fdepth; //Max delay deviation expressed as frequency of lowest frequency notch.  Min = 20, Max = 15000
	float fwidth; //LFO amplitude.
	float foffset; // Offset of notch 1 to notch 2.  Range 0 to 1.0
	float ffb; //Feedback parameter.  Range -0.99 to 0.99
	float fhidamp; //Lowpass filter delay line.  Range 20 to 20000 (Hz)
	float fsubtract; //Subtract wet/dry instead of add.  Nonzero is true
	float fzero; //Enable through-zero flanging
	YroLowfrequencyOscillation lfo; //lfo Flanger

//Internally used variables
	int maxx_delay;
	int kl, kr, zl, zr;
	int zcenter;

	float l, r, ldl, rdl, zdr, zdl;
	float rflange0, rflange1, lflange0, lflange1, oldrflange0, oldrflange1,
			oldlflange0, oldlflange1;
	float period_const, base, ibase;
	float *ldelay, *rdelay, *zldelay, *zrdelay;
	float oldl, oldr; //pt. lpf
	float rsA, rsB, lsA, lsB; //Audio sample at given delay

};

}
#endif
