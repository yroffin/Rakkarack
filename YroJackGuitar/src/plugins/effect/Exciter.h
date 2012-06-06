/*
 Rakarrack   Audio FX software
 Exciter.h - Harmonic Enhancer
 Based in Steve Harris LADSPA Plugin harmonic_gen
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

#ifndef EXCITER_H
#define EXCITER_H

#include <plugins/YroEffectPlugin.h>
#include <plugins/filter/HarmEnhancer.h>

namespace std {

class Exciter: public YroEffectPlugin {
public:
	Exciter();
	~Exciter();
	void render(jack_nframes_t nframes, float * smpsl, float * smpr);
	void cleanup();
	const char *toXml();

	/**
	 * member declaration
	 */
	enum functions {
		_preset, _volume, _lpf, _hpf
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
		return getLpf();
	}
	void set2(int value) {
		setLpf(value);
	}
	int get3() {
		return getHpf();
	}
	void set3(int value) {
		setHpf(value);
	}
	int get4() {
		return getHar(0);
	}
	void set4(int value) {
		setHar(0, value);
	}
	int get5() {
		return getHar(1);
	}
	void set5(int value) {
		setHar(1, value);
	}
	int get6() {
		return getHar(2);
	}
	void set6(int value) {
		setHar(2, value);
	}
	int get7() {
		return getHar(3);
	}
	void set7(int value) {
		setHar(3, value);
	}
	int get8() {
		return getHar(4);
	}
	void set8(int value) {
		setHar(4, value);
	}
	int get9() {
		return getHar(5);
	}
	void set9(int value) {
		setHar(5, value);
	}
	int get10() {
		return getHar(6);
	}
	void set10(int value) {
		setHar(6, value);
	}
	int get11() {
		return getHar(7);
	}
	void set11(int value) {
		setHar(7, value);
	}
	int get12() {
		return getHar(8);
	}
	void set12(int value) {
		setHar(8, value);
	}
	int get13() {
		return getHar(9);
	}
	void set13(int value) {
		setHar(9, value);
	}

	/**
	 * setter and getter
	 */
	int getVolume();
	void setVolume(int value);
	int getLpf();
	void setLpf(int value);
	int getHpf();
	void setHpf(int value);
	int getHar(int sel);
	void setHar(int sel, int value);

private:
	int Prm[10];
	int Pvolume;
	int lpffreq;
	int hpffreq;

	float rm[10];

	class HarmEnhancer *harm;

};

}
#endif
