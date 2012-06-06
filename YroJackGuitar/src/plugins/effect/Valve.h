/*
 ZynAddSubFX - a software synthesizer
 Copyright (C) 2002-2005 Nasca Octavian Paul
 Based on valve Steve Harris LADSPA plugin.
 Valve.h - Distorsion Effect

 Modified and adapted to rakarrack by Josep Andreu.

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

#ifndef VALVE_H
#define VALVE_H

#include <plugins/YroEffectPlugin.h>
#include <plugins/filter/AnalogFilter.h>
#include <plugins/filter/HarmEnhancer.h>

namespace std {

class Valve: public YroEffectPlugin

{
public:
	Valve();
	~Valve();
	void render(jack_nframes_t nframes, float * smpsl, float * smpr);
	float Wshape(float x);
	void cleanup();
	const char *toXml();
	void applyfilters(float * efxoutl, float * efxoutr);

	/**
	 * member declaration
	 */
enum functions {
		_preset,
		_volume,
		_panning,
		_lrcross,
		_drive,
		_level,
		_negate,
		_lpf,
		_hpf,
		_stereo,
		_prefiltering,
		_qq,
		_ed,
		_presence
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
		return getLrcross();
	}
	void set3(int value) {
		setLrcross(value);
	}
	int get4() {
		return getDrive();
	}
	void set4(int value) {
		setDrive(value);
	}
	int get5() {
		return getLevel();
	}
	void set5(int value) {
		setLevel(value);
	}
	int get6() {
		return getNegate();
	}
	void set6(int value) {
		setNegate(value);
	}
	int get7() {
		return getLpf();
	}
	void set7(int value) {
		setLpf(value);
	}
	int get8() {
		return getHpf();
	}
	void set8(int value) {
		setHpf(value);
	}
	int get9() {
		return getStereo();
	}
	void set9(int value) {
		setStereo(value);
	}
	int get10() {
		return getPrefiltering();
	}
	void set10(int value) {
		setPrefiltering(value);
	}
	int get11() {
		return getQq();
	}
	void set11(int value) {
		setQq(value);
	}
	int get12() {
		return getEd();
	}
	void set12(int value) {
		setEd(value);
	}
	int get13() {
		return getPresence();
	}
	void set13(int value) {
		setPresence(value);
	}

	/**
	 * setter and getter
	 */
	int getVolume();
	void setVolume(int value);
	int getPanning();
	void setPanning(int value);
	int getLrcross();
	void setLrcross(int value);
	int getDrive();
	void setDrive(int value);
	int getLevel();
	void setLevel(int value);
	int getNegate();
	void setNegate(int value);
	int getLpf();
	void setLpf(int value);
	int getHpf();
	void setHpf(int value);
	int getStereo();
	void setStereo(int value);
	int getPrefiltering();
	void setPrefiltering(int value);
	int getQq();
	void setQq(int value);
	int getEd();
	void setEd(int value);
	int getPresence();
	void setPresence(int value);

private:
	float outvolume;

	void init_coefs();

//Parametrii
	int Pvolume; //Volumul or E/R
	int Ppanning; //Panning
	int Plrcross; // L/R Mixing
	int Pdrive; //the input amplification
	int Plevel; //the ouput amplification
	int Pnegate; //if the input is negated
	int Plpf; //lowpass filter
	int Phpf; //highpass filter
	int Pstereo; //0=mono,1=stereo
	int Pprefiltering; //if you want to do the filtering before the distorsion
	int Q_q;
	int Ped;
	int Presence;

//Parametrii reali
	float panning, lrcross, q, dist, otml, otmr, itml, itmr, factor, atk;
	float rm[10];

	float coef;
	float qcoef;
	float fdist;
	float inputvol;

	AnalogFilter *lpfl, *lpfr, *hpfl, *hpfr;
	class HarmEnhancer *harm;

};

}
#endif
