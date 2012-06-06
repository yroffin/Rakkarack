/*
 rakarrack - a guitar effects software

 Compressor.h  -  Compressor Effect definitions
 Based on artscompressor.cc by Matthias Kretz <kretz@kde.org>
 Stefan Westerfeld <stefan@space.twc.de>

 Copyright (C) 2008-2010 Josep Andreu
 Author: Josep Andreu & Ryan Billing

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

#ifndef COMPRESSOR_H
#define COMPRESSOR_H

#include <plugins/YroEffectPlugin.h>

namespace std {

class Compressor: public YroEffectPlugin {
public:
	Compressor();
	~Compressor();

	void render(jack_nframes_t nframes, float * smps_l, float * smps_r);
	void cleanup();
	const char *toXml();

	/**
	 * member declaration
	 */
enum functions {
		_preset,
		_threshold,
		_ratio,
		_output,
		_att,
		_rel,
		_out,
		_knee,
		_stereo,
		_peak
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
		return getThreshold();
	}
	void set1(int value) {
		setThreshold(value);
	}
	int get2() {
		return getRatio();
	}
	void set2(int value) {
		setRatio(value);
	}
	int get3() {
		return getOutput();
	}
	void set3(int value) {
		setOutput(value);
	}
	int get4() {
		return getAtt();
	}
	void set4(int value) {
		setAtt(value);
	}
	int get5() {
		return getRel();
	}
	void set5(int value) {
		setRel(value);
	}
	int get6() {
		return getOut();
	}
	void set6(int value) {
		setOut(value);
	}
	int get7() {
		return getKnee();
	}
	void set7(int value) {
		setKnee(value);
	}
	int get8() {
		return getStereo();
	}
	void set8(int value) {
		setStereo(value);
	}
	int get9() {
		return getPeak();
	}
	void set9(int value) {
		setPeak(value);
	}

	/**
	 * setter and getter
	 */
	int getThreshold();
	void setThreshold(int value);
	int getRatio();
	void setRatio(int value);
	int getOutput();
	void setOutput(int value);
	int getAtt();
	void setAtt(int value);
	int getRel();
	void setRel(int value);
	int getOut();
	void setOut(int value);
	int getKnee();
	void setKnee(int value);
	int getStereo();
	void setStereo(int value);
	int getPeak();
	void setPeak(int value);

	void compute();

private:
	int tatt; // attack time  (ms)
	int trel; // release time (ms)
	int tratio;
	int toutput;
	int tthreshold;
	int a_out;
	int stereo;
	int tknee;
	int peak;
	int clipping;
	int limit;

private:

	float rvolume;
	float lvolume;
	float rvolume_db;
	float lvolume_db;
	float thres_db; // threshold
	float knee;
	float thres_mx;
	float kpct;
	float ratio; // ratio
	float kratio; // ratio maximum for knee region
	float eratio; // dynamic ratio
	float makeup; // make-up gain
	float makeuplin;

	float outlevel;
	float att, attr, attl;
	float rel, relr, rell;
	float relcnst, attconst;
	int ltimer, rtimer, hold;

	float rgain;
	float rgain_old;

	float lgain;
	float lgain_old;

	float lgain_t;
	float rgain_t;

	float coeff_kratio;
	float coeff_ratio;
	float coeff_knee;
	float coeff_kk;
	float lpeak;
	float rpeak;
};

}
#endif
