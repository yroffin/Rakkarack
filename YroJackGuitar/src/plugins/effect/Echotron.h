/*
 Echotron.h - Convolution-based Echo Effect

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

#ifndef ECHOTRON_H
#define ECHOTRON_H

#include <plugins/YroEffectPlugin.h>
#include <plugins/filter/AnalogFilter.h>
#include <plugins/filter/RBFilter.h>
#include <plugins/misc/YroLowfrequencyOscillation.h>

#define  ECHOTRON_F_SIZE   128       //Allow up to 150 points in the file
#define  ECHOTRON_MAXFILTERS  32      //filters available
namespace std {

class Echotron: public YroEffectPlugin {
public:
	Echotron();
	~Echotron();
	void render(jack_nframes_t nframes, float * smpsl, float * smpr);
	void cleanup();
	const char *toXml();

	/**
	 * member declaration
	 */
enum functions {
		_preset,
		_volume,
		_depth,
		_width,
		_length,
		_user,
		_tempo,
		_hidamp,
		_lrcross,
		_file,
		_lfostereo,
		_fb,
		_panning,
		_moddly,
		_modfilts,
		_lfotype,
		_filters
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
		return getDepth();
	}
	void set2(int value) {
		setDepth(value);
	}
	int get3() {
		return getWidth();
	}
	void set3(int value) {
		setWidth(value);
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
		return getTempo();
	}
	void set6(int value) {
		setTempo(value);
	}
	int get7() {
		return getHidamp();
	}
	void set7(int value) {
		setHidamp(value);
	}
	int get8() {
		return getLrcross();
	}
	void set8(int value) {
		setLrcross(value);
	}
	int get9() {
		return getFile();
	}
	void set9(int value) {
		setFile(value);
	}
	int get10() {
		return getLfoStereo();
	}
	void set10(int value) {
		setLfoStereo(value);
	}
	int get11() {
		return getFb();
	}
	void set11(int value) {
		setFb(value);
	}
	int get12() {
		return getPanning();
	}
	void set12(int value) {
		setPanning(value);
	}
	int get13() {
		return getModdly();
	}
	void set13(int value) {
		setModdly(value);
	}
	int get14() {
		return getModfilts();
	}
	void set14(int value) {
		setModfilts(value);
	}
	int get15() {
		return getLfoType();
	}
	void set15(int value) {
		setLfoType(value);
	}
	int get16() {
		return getFilters();
	}
	void set16(int value) {
		setFilters(value);
	}

	/**
	 * setter and getter
	 */
	int getVolume();
	void setVolume(int value);
	int getDepth();
	void setDepth(int value);
	int getWidth();
	void setWidth(int value);
	int getLength();
	void setLength(int value);
	int getUser();
	void setUser(int value);
	int getTempo();
	void setTempo(int value);
	int getHidamp();
	void setHidamp(int value);
	int getLrcross();
	void setLrcross(int value);
	int getFile();
	void setFile(int value);
	int getLfoStereo();
	void setLfoStereo(int value);
	int getFb();
	void setFb(int value);
	int getPanning();
	void setPanning(int value);
	int getModdly();
	void setModdly(int value);
	int getModfilts();
	void setModfilts(int value);
	int getLfoType();
	void setLfoType(int value);
	int getFilters();
	void setFilters(int value);

private:
	int Pchange;
	char Filename[128];
	void init_params();
	void modulate_delay();
	void modulate_filters();
	void loaddefault();

//User input parameters
	YroLowfrequencyOscillation lfo;
	YroLowfrequencyOscillation dlfo;
	int Pvolume; //This is master wet/dry mix like other FX...but I am finding it is not useful
	int Ppanning; //Panning
	int Plrcross; // L/R Mixing  //
	int Phidamp;
	int Puser; //0,1//
	int Ptempo; //Tempo, BPM//For stretching reverb responses
	int Filenum;
	int Pfb; //-64 ... 64// amount of feedback
	int Pdepth;
	int Pwidth;
	int Pfilters; //0 or 1// use or don't use filters in delay line
	int Pmodfilts; //0 or 1// apply LFO to filter freqs
	int Pmoddly; //0 or 1// apply LFO to delay time
	int Pstdiff;
	int Plength;

	int offset;
	int maxx_size;

//arrays of parameters from text file:
	float fPan[ECHOTRON_F_SIZE]; //1+Pan from text file
	float fTime[ECHOTRON_F_SIZE];
	float fLevel[ECHOTRON_F_SIZE];
	float fLP[ECHOTRON_F_SIZE];
	float fBP[ECHOTRON_F_SIZE];
	float fHP[ECHOTRON_F_SIZE];
	float fFreq[ECHOTRON_F_SIZE];
	float fQ[ECHOTRON_F_SIZE];
	int iStages[ECHOTRON_F_SIZE];
	float subdiv_dmod;
	float subdiv_fmod;
	int f_qmode;

	int rtime[ECHOTRON_F_SIZE];
	int ltime[ECHOTRON_F_SIZE];
	float ldata[ECHOTRON_F_SIZE];
	float rdata[ECHOTRON_F_SIZE];

//end text configurable parameters

	int initparams;

	float ldmod, rdmod, oldldmod, oldrdmod, interpl, interpr;
	float dlyrange;

	float width, depth;
	float lpanning, rpanning, hidamp, alpha_hidamp, convlength;

	float *lxn, *rxn;

	float level, fb, rfeedback, lfeedback, levpanl, levpanr, lrcross, ilrcross;
	float tempo_coeff;

	AnalogFilter *lpfl, *lpfr; //filters

	struct {
		float sfreq, sq, sLP, sBP, sHP, sStg;
		RBFilter *l, *r;

	} filterbank[ECHOTRON_MAXFILTERS];

};

}
#endif
