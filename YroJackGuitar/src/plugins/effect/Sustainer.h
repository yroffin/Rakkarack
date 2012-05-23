/*
 Rakarrack Guitar FX

 Sustainer.h - Simple compressor/sustainer effect with easy interface, minimal controls
 Copyright (C) 2010 Ryan Billing
 Author: Ryan Billing
 
 This program is free software; you can redistribute it and/or modify
 it under the terms of version 3 of the GNU General Public License
 as published by the Free Software Foundation.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License (version 2) for more details.

 You should have received a copy of the GNU General Public License (version 2)
 along with this program; if not, write to the Free Software Foundation,
 Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA

 */

#ifndef SUSTAINER_H
#define SUSTAINER_H

#include <plugins/YroEffectPlugin.h>

namespace std {

class Sustainer: public YroEffectPlugin

{
public:
	Sustainer();
	~Sustainer();
	void cleanup();

	void render(jack_nframes_t nframes, float * smpsl, float * smpr);

	/**
	 * member declaration
	 */
	enum functions {
		_preset, _volume, _sustain
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
		return getSustain();
	}
	void set2(int value) {
		setSustain(value);
	}

	/**
	 * setter and getter
	 */
	int getVolume();
	void setVolume(int value);
	int getSustain();
	void setSustain(int value);
private:
	//Parametrii
	int Pvolume; //Output Level
	int Psustain; //Compression amount

	int timer, hold;
	float level, fsustain, input, tmpgain;
	float prls, compeak, compg, compenv, oldcompenv, calpha, cbeta, cthresh,
			cratio, cpthresh;

};

}
#endif
