/*
 ZynAddSubFX - a software synthesizer
 
 Distorsion.h - Distorsion Effect
 Copyright (C) 2002-2005 Nasca Octavian Paul
 Author: Nasca Octavian Paul

 Modified for rakarrack by Josep Andreu  & Ryan Billing

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

#ifndef NEWDIST_H
#define NEWDIST_H

#include <plugins/YroEffectPlugin.h>
#include <plugins/filter/AnalogFilter.h>
#include <plugins/filter/WaveShaper.h>
#include <plugins/filter/Filter.h>
#include <plugins/filter/FilterParams.h>

namespace std {

class NewDist: public YroEffectPlugin {
public:
	NewDist();
	~NewDist();
	void render(jack_nframes_t nframes, float * smpsl, float * smpr);
	void cleanup();
	void applyfilters(float * efxoutl, float * efxoutr);

	/**
	* member declaration
	*/
	enum functions {
	_preset
	, _volume
	, _panning
	, _lrcross
	, _drive
	, _level
	, _type
	, _negate
	, _lpf
	, _hpf
	, _rfreq
	, _prefiltering
	, _octave
	};

	/**
	* setter and getter map
	*/
	int  get0() {return getPreset();}
	void set0(int value) {setPreset(value);}
	int  get1() {return getVolume();}
	void set1(int value) {setVolume(value);}
	int  get2() {return getPanning();}
	void set2(int value) {setPanning(value);}
	int  get3() {return getLrcross();}
	void set3(int value) {setLrcross(value);}
	int  get4() {return getDrive();}
	void set4(int value) {setDrive(value);}
	int  get5() {return getLevel();}
	void set5(int value) {setLevel(value);}
	int  get6() {return getType();}
	void set6(int value) {setType(value);}
	int  get7() {return getNegate();}
	void set7(int value) {setNegate(value);}
	int  get8() {return getLpf();}
	void set8(int value) {setLpf(value);}
	int  get9() {return getHpf();}
	void set9(int value) {setHpf(value);}
	int  get10() {return getRfreq();}
	void set10(int value) {setRfreq(value);}
	int  get11() {return getPrefiltering();}
	void set11(int value) {setPrefiltering(value);}
	int  get12() {return getOctave();}
	void set12(int value) {setOctave(value);}

	/**
	* setter and getter
	*/
	int  getVolume();
	void setVolume(int value);
	int  getPanning();
	void setPanning(int value);
	int  getLrcross();
	void setLrcross(int value);
	int  getDrive();
	void setDrive(int value);
	int  getLevel();
	void setLevel(int value);
	int  getType();
	void setType(int value);
	int  getNegate();
	void setNegate(int value);
	int  getLpf();
	void setLpf(int value);
	int  getHpf();
	void setHpf(int value);
	int  getRfreq();
	void setRfreq(int value);
	int  getPrefiltering();
	void setPrefiltering(int value);
	int  getOctave();
	void setOctave(int value);

private:
	float inpll[4096];
	float inplr[4096];

	/**
	 * parameters
	 */
	int Pvolume; //Volumul or E/R
	int Ppanning; //Panning
	int Plrcross; // L/R Mixing
	int Pdrive; //the input amplification
	int Plevel; //the ouput amplification
	int Ptype; //Distorsion type
	int Pnegate; //if the input is negated
	int Plpf; //lowpass filter
	int Phpf; //highpass filter
	int Prfreq;
	int Pprefiltering; //if you want to do the filtering before the distorsion
	int Poctave; //mix sub octave

	float rfreq;
	float panning, lrcross, octave_memoryl, togglel, octave_memoryr, toggler,
			octmix;
	float *octoutl, *octoutr;

	//Parametrii reali
	AnalogFilter *lpfl, *lpfr, *hpfl, *hpfr, *blockDCl, *blockDCr, *DCl, *DCr;
	WaveShaper *wshapel, *wshaper;

	Filter *filterl, *filterr;
	FilterParams *filterpars;
};

}
#endif
