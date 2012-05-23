/*
 ZynAddSubFX - a software synthesizer
 
 DynamicFilter.h - "WahWah" effect and others
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

#ifndef DYNAMICFILTER_H
#define DYNAMICFILTER_H

#include <plugins/YroEffectPlugin.h>
#include <plugins/misc/YroLowfrequencyOscillation.h>
#include <plugins/filter/Filter.h>

namespace std {

class DynamicFilter: public YroEffectPlugin

{
public:
	DynamicFilter();
	~DynamicFilter();
	void render(jack_nframes_t nframes, float * smpsl, float * smpsr);
	void cleanup();

	/**
	 * member declaration
	 */
	enum functions {
		_volume,
		_panning,
		_LfoPfreq,
		_LfoPrandomness,
		_LfoPlfOtype,
		_LfoPstereo,
		_depth,
		_ampsns,
		_ampsnsinv,
		_ampsmooth
	};

	/**
	 * setter and getter map
	 */
	int get0() {
		return getVolume();
	}
	void set0(int value) {
		setVolume(value);
	}
	int get1() {
		return getPanning();
	}
	void set1(int value) {
		setPanning(value);
	}
	int get2() {
		return getLfoPfreq();
	}
	void set2(int value) {
		setLfoPfreq(value);
	}
	int get3() {
		return getLfoPrandomness();
	}
	void set3(int value) {
		setLfoPrandomness(value);
	}
	int get4() {
		return getLfoPlfOtype();
	}
	void set4(int value) {
		setLfoPlfOtype(value);
	}
	int get5() {
		return getLfoPstereo();
	}
	void set5(int value) {
		setLfoPstereo(value);
	}
	int get6() {
		return getDepth();
	}
	void set6(int value) {
		setDepth(value);
	}
	int get7() {
		return getAmpsns();
	}
	void set7(int value) {
		setAmpsns(value);
	}
	int get8() {
		return getAmpsnsinv();
	}
	void set8(int value) {
		setAmpsnsinv(value);
	}
	int get9() {
		return getAmpsmooth();
	}
	void set9(int value) {
		setAmpsmooth(value);
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
	int getAmpsns();
	void setAmpsns(int value);
	int getAmpsnsinv();
	void setAmpsnsinv(int value);
	int getAmpsmooth();
	void setAmpsmooth(int value);

private:
	float outvolume;

	//Parametrii DynamicFilter
	YroLowfrequencyOscillation lfo; //lfo-ul DynamicFilter
	int Pvolume;
	int Ppanning;
	int Pdepth; //the depth of the lfo of the DynamicFilter
	int Pampsns; //how the filter varies according to the input amplitude
	int Pampsnsinv; //if the filter freq is lowered if the input amplitude rises
	int Pampsmooth; //how smooth the input amplitude changes the filter

	void setPreset(int _preset);
	void reinitfilter();

	//Valorile interne

	float panning, depth, ampsns, ampsmooth;
	float ms1, ms2, ms3, ms4; //mean squares

	Filter *filterl, *filterr;
	FilterParams *filterpars;
};

}
#endif
