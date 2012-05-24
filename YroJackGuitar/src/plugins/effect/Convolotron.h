/*
 Convolotron.h - Convolotron Effect

 Author: Ryam Billing & Jospe Andreu

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

#ifndef CONVOLOTRON_H
#define CONVOLOTRON_H

#include <sndfile.h>
#include <plugins/YroEffectPlugin.h>
#include <plugins/misc/Resample.h>

namespace std {

class Convolotron: public YroEffectPlugin {
public:
	Convolotron(int DS, int uq, int dq);
	~Convolotron();
	void render(jack_nframes_t nframes, float * smpsl, float * smpr);
	void cleanup();
	void adjust(int DS);
	void loaddefault();

	/**
	* member declaration
	*/
	enum functions {
	_preset
	, _volume
	, _panning
	, _safe
	, _length
	, _file
	, _hidamp
	, _level
	, _user
	, _fb
	};

	/**
	* setter and getter map
	*/
	int  get0() {return getPreset();}
	void set0(int value) {setPreset(value);}
	int  get1() {return getVolume();}
	void set1(int value) {setVolume(value);}
	int  get2() {return getPanning();}
	void set2(int value) {setPanning(value);}
	int  get3() {return getSafe();}
	void set3(int value) {setSafe(value);}
	int  get4() {return getLength();}
	void set4(int value) {setLength(value);}
	int  get5() {return getFile();}
	void set5(int value) {setFile(value);}
	int  get6() {return getHidamp();}
	void set6(int value) {setHidamp(value);}
	int  get7() {return getLevel();}
	void set7(int value) {setLevel(value);}
	int  get8() {return getUser();}
	void set8(int value) {setUser(value);}
	int  get9() {return getFb();}
	void set9(int value) {setFb(value);}

	/**
	* setter and getter
	*/
	int  getVolume();
	void setVolume(int value);
	int  getPanning();
	void setPanning(int value);
	int  getSafe();
	void setSafe(int value);
	int  getLength();
	void setLength(int value);
	int  getFile();
	void setFile(int value);
	int  getHidamp();
	void setHidamp(int value);
	int  getLevel();
	void setLevel(int value);
	int  getUser();
	void setUser(int value);
	int  getFb();
	void setFb(int value);

	char Filename[128];

private:
	//Parametrii
	int Pvolume; //This is master wet/dry mix like other FX...but I am finding it is not useful
	int Ppanning; //Panning
	int Plrcross; // L/R Mixing  // This is a mono effect, so lrcross and panning are pointless
	int Phidamp;
	int Plevel; //This should only adjust the level of the IR effect, and not wet/dry mix
	int Psafe;
	int Plength; //5...500 ms// Set maximum length of IR.
	int Puser; //-64...64//Feedback.
	int Filenum;
	int Pfb; //-64 ... 64// amount of feedback
	void setvolume(int Pvolume);
	void setpanning(int Ppanning);
	void sethidamp(int Phidamp);
	void process_rbuf();

	int offset;
	int maxx_size, maxx_read, real_len, length;
	int DS_state;
	int nPERIOD;
	int nSAMPLE_RATE;

	double u_up;
	double u_down;
	float nfSAMPLE_RATE;

	float lpanning, rpanning, hidamp, alpha_hidamp, convlength, oldl;
	float *rbuf, *buf, *lxn;
	float *templ, *tempr;

	float level, fb, feedback;
	float levpanl, levpanr;

	SNDFILE *infile;
	SF_INFO sfinfo;

	//Parametrii reali

	class Resample *M_Resample;
	class Resample *U_Resample;
	class Resample *D_Resample;
};

}
#endif
