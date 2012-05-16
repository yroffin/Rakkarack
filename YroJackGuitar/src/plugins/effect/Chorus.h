/*
 ZynAddSubFX - a software synthesizer
 
 Chorus.h - Chorus and Flange effects
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

#ifndef CHORUS_H
#define CHORUS_H

#include <plugins/YroEffectPlugin.h>
#include <plugins/misc/YroLowfrequencyOscillation.h>

namespace std {

class Chorus : public YroEffectPlugin {

public:
	Chorus();
	~Chorus();
	void render(jack_nframes_t nframes, float *outl, float *outr);
	void cleanup();

	enum functions {
		_preset,
		_volume,
		_panning,
		_freq,
		_randomness,
		_lfOtype,
		_stereo,
		_depth,
		_delay,
		_fb,
		_lrcross,
		_flangemode,
		_outsub
	};

	int  get0() {return getPreset();};
	void set0(int value) {setPreset(value);};
	int  get1() {return getPvolume();};
	void set1(int value) {setPvolume(value);};
	int  get2() {return getPpanning();};
	void set2(int value) {setPpanning(value);};
	int  get3() {return lfo.getPfreq();};
	void set3(int value) {lfo.setPfreq(value);onChange(_freq);};
	int  get4() {return lfo.getPrandomness();};
	void set4(int value) {lfo.setPrandomness(value);onChange(_randomness);};
	int  get5() {return lfo.getPlfOtype();};
	void set5(int value) {lfo.setPlfOtype(value);onChange(_lfOtype);};
	int  get6() {return lfo.getPstereo();};
	void set6(int value) {lfo.setPstereo(value);onChange(_stereo);};
	int  get7() {return getPdepth();};
	void set7(int value) {setPdepth(value);};
	int  get8() {return getPdelay();};
	void set8(int value) {setPdelay(value);};
	int  get9() {return getPfb();};
	void set9(int value) {setPfb(value);};
	int  get10() {return getPlrcross();};
	void set10(int value) {setPlrcross(value);};
	int  get11() {return getPflangemode();};
	void set11(int value) {setPflangemode(value);};
	int  get12() {return getPoutsub();};
	void set12(int value) {setPoutsub(value);};

	int getPflangemode() const;
	void setPflangemode(int pflangemode);
	int getPoutsub() const;
	void setPoutsub(int poutsub);

	int getPvolume();
	int getPpanning();
	int getPdepth();
	int getPdelay();
	int getPfb();
	int getPlrcross();

	void setPvolume(int Pvolume);
	void setPpanning(int Ppanning);
	void setPdepth(int Pdepth);
	void setPdelay(int Pdelay);
	void setPfb(int Pfb);
	void setPlrcross(int Plrcross);

private:
	//Parametrii Chorus
	YroLowfrequencyOscillation lfo; //lfo-ul chorus
	float outvolume; //this is the volume of effect and is public because need it in system effect. The out volume of s
	int Pvolume;
	int Ppanning;
	int Pdepth; //the depth of the Chorus(ms)
	int Pdelay; //the delay (ms)
	int Pfb; //feedback
	int Plrcross; //feedback
	int Pflangemode; //how the LFO is scaled, to result chorus or flange
	int Poutsub; //if I wish to substract the output instead of the adding it

	//Valorile interne
	int maxdelay;
	int dlk, drk, dlhi, dlhi2;

	float depth, delay, fb, lrcross, panning;
	float dl1, dl2, dr1, dr2, lfol, lfor;
	float *delayl, *delayr;
	float getdelay(float xlfo);
	float dllo, mdel;
};

}
#endif
