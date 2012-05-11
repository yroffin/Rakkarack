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

class Echotron: public YroEffectPlugin

{
public:
	Echotron();
	~Echotron();
	void render(jack_nframes_t nframes, float * smpsl, float * smpr);
	void setpreset(int npreset);
	void changepar(int npar, int value);
	int getpar(int npar);
	void cleanup();
	int setfile(int value);
	int Ppreset;

	int Pchange;

	float *efxoutl;
	float *efxoutr;
	float outvolume;

	char Filename[128];

private:

	void setvolume(int Pvolume);
	void setpanning(int Ppanning);
	void sethidamp(int Phidamp);
	void setlpf(int Plpf);
	void setfb(int value);
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
