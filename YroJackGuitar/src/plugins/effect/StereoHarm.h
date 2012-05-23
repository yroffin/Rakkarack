/*
 rakarrack - a guitar effects software

 StereoHarm.h  -  Stereo Harmonizer definitions
 Copyright (C) 2008 Josep Andreu
 Author: Josep Andreu

 Using Stephan M. Bernsee smbPitchShifter engine.

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

#ifndef STEREOHARM_H
#define STEREOHARM_H

#include <plugins/YroEffectPlugin.h>
#include <plugins/misc/Resample.h>
#include <plugins/misc/PitchShifter.h>

namespace std {

class StereoHarm: public YroEffectPlugin {
public:
	StereoHarm(long int Quality, int DS, int uq, int dq);
	~StereoHarm();
	void render(jack_nframes_t nframes, float *smpsl, float *smpsr);
	void cleanup();
	void adjust(int DS);
	/**
	 * member declaration
	 */
	enum functions {
		_preset,
		_volume,
		_gain0,
		_interval0,
		_chrome0,
		_gain1,
		_interval1,
		_chrome1,
		_select,
		_note,
		_type,
		_MIDI,
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
		return getVolume();
	}
	void set1(int value) {
		setVolume(value);
	}
	int get2() {
		return getGain(0);
	}
	void set2(int value) {
		setGain(0, value);
	}
	int get3() {
		return getInterval(0);
	}
	void set3(int value) {
		setInterval(0, value);
	}
	int get4() {
		return getChrome(0);
	}
	void set4(int value) {
		setChrome(0, value);
	}
	int get5() {
		return getGain(1);
	}
	void set5(int value) {
		setGain(1, value);
	}
	int get6() {
		return getInterval(1);
	}
	void set6(int value) {
		setInterval(1, value);
	}
	int get7() {
		return getChrome(1);
	}
	void set7(int value) {
		setChrome(1, value);
	}
	int get8() {
		return getSelect();
	}
	void set8(int value) {
		setSelect(value);
	}
	int get9() {
		return getNote();
	}
	void set9(int value) {
		setNote(value);
	}
	int get10() {
		return getType();
	}
	void set10(int value) {
		setType(value);
	}
	int get11() {
		return getMIDI();
	}
	void set11(int value) {
		setMIDI(value);
	}
	int get12() {
		return getLrcross();
	}
	void set12(int value) {
		setLrcross(value);
	}

	/**
	 * setter and getter
	 */
	int getVolume();
	void setVolume(int value);
	int getGain(int sel);
	void setGain(int sel, int value);
	int getInterval(int sel);
	void setInterval(int sel, int value);
	int getChrome(int sel);
	void setChrome(int sel, int value);
	int getSelect();
	void setSelect(int value);
	int getNote();
	void setNote(int value);
	int getType();
	void setType(int value);
	int getMIDI();
	void setMIDI(int value);
	int getLrcross();
	void setLrcross(int value);

private:
	int Pintervall;
	int Pintervalr;

	int PMIDI;
	int PSELECT;
	int mira;
	int DS_state;
	int nPERIOD;
	int nSAMPLE_RATE;
	long window;

	long int hq;

	double u_up;
	double u_down;
	float nfSAMPLE_RATE;

	/**
	 * TODO ??? duplicate ???
	 */
	float *efxoutl;
	float *efxoutr;
	float *outil, *outir;
	float *outol, *outor;
	float *templ, *tempr;

	float outvolume;

private:

	int Pvolume;
	int Plrcross;
	int Pgainl;
	int Pgainr;
	int Pchromel;
	int Pchromer;
	int Pnote;
	int Ptype;

	float gainl, gainr;
	float intervall;
	float intervalr;
	float chromel;
	float chromer;
	float lrcross;

	Resample *U_Resample;
	Resample *D_Resample;

	PitchShifter *PSl, *PSr;

};

}
#endif
