/*
 
 CompBand.h - 4 Bands Compressor

 Using Compressor and AnalogFilters by other authors.

 Based on artscompressor.cc by Matthias Kretz <kretz@kde.org>
 Stefan Westerfeld <stefan@space.twc.de>
 Modified by Ryan Billing & Josep Andreu

 Copyright (C) 2002-2005 Nasca Octavian Paul
 Author: Nasca Octavian Paul
 ZynAddSubFX - a software synthesizer

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

#ifndef COMPBANDL_H
#define COMPBANDL_H

#include <plugins/YroEffectPlugin.h>
#include <plugins/filter/AnalogFilter.h>
#include <plugins/effect/Compressor.h>

namespace std {

class CompBand: public YroEffectPlugin

{
public:
	CompBand();
	~CompBand();
	void render(jack_nframes_t nframes, float * smpsl, float * smpr);
	void cleanup();

	/**
	* member declaration
	*/
	enum functions {
	_preset
	, _volume
	, _plratio
	, _pmlratio
	, _pmhratio
	, _phratio
	, _plthreshold
	, _pmlthreshold
	, _pmhthreshold
	, _phthreshold
	, _cross1
	, _cross2
	, _cross3
	, _level
	};

	/**
	* setter and getter map
	*/
	int  get0() {return getPreset();}
	void set0(int value) {setPreset(value);}
	int  get1() {return getVolume();}
	void set1(int value) {setVolume(value);}
	int  get2() {return getLratio();}
	void set2(int value) {setLratio(value);}
	int  get3() {return getMLratio();}
	void set3(int value) {setMLratio(value);}
	int  get4() {return getMHratio();}
	void set4(int value) {setMHratio(value);}
	int  get5() {return getHratio();}
	void set5(int value) {setHratio(value);}
	int  get6() {return getLthreshold();}
	void set6(int value) {setLthreshold(value);}
	int  get7() {return getMLthreshold();}
	void set7(int value) {setMLthreshold(value);}
	int  get8() {return getMHthreshold();}
	void set8(int value) {setMHthreshold(value);}
	int  get9() {return getHthreshold();}
	void set9(int value) {setHthreshold(value);}
	int  get10() {return getCross1();}
	void set10(int value) {setCross1(value);}
	int  get11() {return getCross2();}
	void set11(int value) {setCross2(value);}
	int  get12() {return getCross3();}
	void set12(int value) {setCross3(value);}
	int  get13() {return getLevel();}
	void set13(int value) {setLevel(value);}

	/**
	* setter and getter
	*/
	int  getVolume();
	void setVolume(int value);
	int  getLratio();
	void setLratio(int value);
	int  getMLratio();
	void setMLratio(int value);
	int  getMHratio();
	void setMHratio(int value);
	int  getHratio();
	void setHratio(int value);
	int  getLthreshold();
	void setLthreshold(int value);
	int  getMLthreshold();
	void setMLthreshold(int value);
	int  getMHthreshold();
	void setMHthreshold(int value);
	int  getHthreshold();
	void setHthreshold(int value);
	int  getCross1();
	void setCross1(int value);
	int  getCross2();
	void setCross2(int value);
	int  getCross3();
	void setCross3(int value);
	int  getLevel();
	void setLevel(int value);

private:
	void setRatio(int sel, int value);
	void setThreshold(int sel, int value);

	float level;
	float *lowl;
	float *lowr;
	float *midll;
	float *midlr;
	float *midhl;
	float *midhr;
	float *highl;
	float *highr;

	/**
	 * parameters
	 */
	int Pvolume; //Volumul or E/R
	int Plevel;
	int PLratio;
	int PMLratio;
	int PMHratio;
	int PHratio;
	int PLthres;
	int PMLthres;
	int PMHthres;
	int PHthres;

	int Cross1;
	int Cross2;
	int Cross3;

	//Parametrii reali

	AnalogFilter *lpf1l, *lpf1r, *hpf1l, *hpf1r;
	AnalogFilter *lpf2l, *lpf2r, *hpf2l, *hpf2r;
	AnalogFilter *lpf3l, *lpf3r, *hpf3l, *hpf3r;

	Compressor *CL, *CML, *CMH, *CH;
};

}
#endif
