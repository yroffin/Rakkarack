/*
 rakarrack - a guitar effects software

 pan.h  -  Auto/Pan - Extra stereo definitions
 Copyright (C) 2008-2010 Josep Andreu
 Author: Josep Andreu

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

#ifndef AUTOPAN_H
#define AUTOPAN_H

#include <plugins/YroRawEffectPlugin.h>
#include <plugins/misc/YroLowfrequencyOscillation.h>

namespace std {

class Pan: public YroRawEffectPlugin

{

public:
	Pan();
	~Pan();
	void render(jack_nframes_t nframes, float *smpsl, float *smpsr);
	void cleanup();
	const char *toXml();

	/**
	 * member declaration
	 */
enum functions {
		_preset,
		_volume,
		_panning,
		_lfofreq,
		_lforandomness,
		_lfotype,
		_lfostereo,
		_extra,
		_autopan,
		_extraon
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
		return getLfoFreq();
	}
	void set3(int value) {
		setLfoFreq(value);
	}
	int get4() {
		return getLfoRandomness();
	}
	void set4(int value) {
		setLfoRandomness(value);
	}
	int get5() {
		return getLfoType();
	}
	void set5(int value) {
		setLfoType(value);
	}
	int get6() {
		return getLfoStereo();
	}
	void set6(int value) {
		setLfoStereo(value);
	}
	int get7() {
		return getExtra();
	}
	void set7(int value) {
		setExtra(value);
	}
	int get8() {
		return getAutoPan();
	}
	void set8(int value) {
		setAutoPan(value);
	}
	int get9() {
		return getExtraOn();
	}
	void set9(int value) {
		setExtraOn(value);
	}

	/**
	 * setter and getter
	 */
	int getVolume();
	void setVolume(int value);
	int getPanning();
	void setPanning(int value);
	int getLfoFreq();
	void setLfoFreq(int value);
	int getLfoRandomness();
	void setLfoRandomness(int value);
	int getLfoType();
	void setLfoType(int value);
	int getLfoStereo();
	void setLfoStereo(int value);
	int getExtra();
	void setExtra(int value);
	int getAutoPan();
	void setAutoPan(int value);
	int getExtraOn();
	void setExtraOn(int value);

private:

	int Pvolume;
	int Ppanning;
	int Pextra;
	int PAutoPan;
	int PextraON;

	float dvalue;
	float panning, mul;
	float lfol, lfor;
	float ll, lr;

	YroLowfrequencyOscillation lfo;

};

}
#endif
