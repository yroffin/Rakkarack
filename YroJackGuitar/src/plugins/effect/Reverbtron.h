/*
 Reverbtron.h - Reverbtron Effect

 Author: Ryan Billing & Josep Andreu

 Adapted effect structure of ZynAddSubFX - a software synthesizer
 Author: Nasca Octavian Paul

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

#ifndef REVERBTRON_H
#define REVERBTRON_H

#include <plugins/YroEffectPlugin.h>
#include <plugins/misc/Resample.h>
#include <plugins/filter/AnalogFilter.h>

namespace std {

class Reverbtron: public YroEffectPlugin

{
public:
	Reverbtron(int DS, int uq, int dq);
	~Reverbtron();
	void render(jack_nframes_t nframes, float * smpsl, float * smpr);
	void cleanup();
	const char *toXml();
	int setfile(int value);
	void adjust(int DS);

	/**
	 * member declaration
	 */
enum functions {
		_preset,
		_volume,
		_fade,
		_safe,
		_length,
		_user,
		_idelay,
		_hidamp,
		_level,
		_file
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
		return getFade();
	}
	void set2(int value) {
		setFade(value);
	}
	int get3() {
		return getSafe();
	}
	void set3(int value) {
		setSafe(value);
	}
	int get4() {
		return getLength();
	}
	void set4(int value) {
		setLength(value);
	}
	int get5() {
		return getUser();
	}
	void set5(int value) {
		setUser(value);
	}
	int get6() {
		return getIdelay();
	}
	void set6(int value) {
		setIdelay(value);
	}
	int get7() {
		return getHidamp();
	}
	void set7(int value) {
		setHidamp(value);
	}
	int get8() {
		return getLevel();
	}
	void set8(int value) {
		setLevel(value);
	}
	int get9() {
		return getFile();
	}
	void set9(int value) {
		setFile(value);
	}

	/**
	 * setter and getter
	 */
	int getVolume();
	void setVolume(int value);
	int getFade();
	void setFade(int value);
	int getSafe();
	void setSafe(int value);
	int getLength();
	void setLength(int value);
	int getUser();
	void setUser(int value);
	int getIdelay();
	void setIdelay(int value);
	int getHidamp();
	void setHidamp(int value);
	int getLevel();
	void setLevel(int value);
	int getFile();
	void setFile(int value);

private:
	char Filename[128];

	void setStretch(int value);
	void setEs(int value);
	void setRv(int value);
	void setDiff(int value);
	void setFb(int value);
	void setLpf(int value);
	void setPanning(int value);

	void convert_time();
	void loaddefault();

//Parametrii
	int Pvolume; //This is master wet/dry mix like other FX...but I am finding it is not useful
	int Ppanning; //Panning
	int Plrcross; // L/R Mixing  // This is a mono effect, so lrcross and panning are pointless
	int Phidamp;
	int Plevel; //This should only adjust the level of the IR effect, and not wet/dry mix
	int Plength; //20... 1500// Set maximum number of IR points to calculate.
	int Puser; //-64...64//Feedback.
	int Pstretch; //-64 ... 64//For stretching reverb responses
	int Pidelay; //0 ... 500ms// initial delay time
	int Filenum;
	int Psafe;
	int Pfb; //-64 ... 64// amount of feedback
	int Pfade;
	int Pes; //0 or 1// Add stereo spatialization
	int Prv; //Shuffle
	int Plpf;
	int Pdiff;

	int imctr;
	int imax;
	int offset;
	int hoffset;
	int maxx_size;
	int data_length;
	int avgtime;
	int hrtf_size;
	int hlength;
	int DS_state;
	int nPERIOD;
	int nSAMPLE_RATE;

	int *time, *rndtime;
	double u_up;
	double u_down;
	float nfSAMPLE_RATE;

	float fstretch, idelay, ffade, maxtime, maxdata, decay, diffusion;
	float lpanning, rpanning, hidamp, alpha_hidamp, convlength, oldl;
	float *data, *lxn, *imdelay, *ftime, *tdata, *rnddata, *hrtf;
	float *templ, *tempr;
	float level, fb, feedback, levpanl, levpanr;
	float roomsize;

	Resample *U_Resample;
	Resample *D_Resample;

	AnalogFilter *lpfl, *lpfr; //filters

};

}
#endif
