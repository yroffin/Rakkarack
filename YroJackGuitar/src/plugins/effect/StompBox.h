/*
 Rakarrack   Audio FX software
 Stompbox.h - stompbox modeler
 Using Steve Harris LADSPA Plugin harmonic_gen
 Modified for rakarrack by Ryan Billing & Josep Andreu

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

#ifndef STOMPBOX_H
#define STOMPBOX_H

#include <plugins/YroEffectPlugin.h>
#include <plugins/filter/AnalogFilter.h>
#include <plugins/filter/WaveShaper.h>

namespace std {

class StompBox: public YroEffectPlugin

{
public:
	StompBox();
	~StompBox();
	void render(jack_nframes_t nframes, float * smpsl, float * smpr);
	void cleanup();

	/**
	 * member declaration
	 */
	enum functions {
		_preset, _volume, _high, _mid, _low, _gain, _mode
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
		return getHigh();
	}
	void set2(int value) {
		setHigh(value);
	}
	int get3() {
		return getMid();
	}
	void set3(int value) {
		setMid(value);
	}
	int get4() {
		return getLow();
	}
	void set4(int value) {
		setLow(value);
	}
	int get5() {
		return getGain();
	}
	void set5(int value) {
		setGain(value);
	}
	int get6() {
		return getMode();
	}
	void set6(int value) {
		setMode(value);
	}

	/**
	 * setter and getter
	 */
	int getVolume();
	void setVolume(int value);
	int getHigh();
	void setHigh(int value);
	int getMid();
	void setMid(int value);
	int getLow();
	void setLow(int value);
	int getGain();
	void setGain(int value);
	int getMode();
	void setMode(int value);

private:
	void init_mode(int value);
	void init_tone();

	int Pvolume;
	int Pgain;
	int Phigh;
	int Pmid;
	int Plow;
	int Pmode;

	float gain, pre1gain, pre2gain, lowb, midb, highb, volume;
	float LG, MG, HG, RGP2, RGPST, pgain;

	AnalogFilter *linput, *lpre1, *lpre2, *lpost, *ltonehg, *ltonemd, *ltonelw;
	AnalogFilter *rinput, *rpre1, *rpre2, *rpost, *rtonehg, *rtonemd, *rtonelw;
	AnalogFilter *ranti, *lanti;
	WaveShaper *lwshape, *rwshape, *lwshape2, *rwshape2;
};

}
#endif
