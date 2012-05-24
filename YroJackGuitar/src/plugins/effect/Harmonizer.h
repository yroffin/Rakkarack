/*
  rakarrack - a guitar effects software

  Harmonizer.h  -  Harmonizer definitions
  Copyright (C) 2008 Josep Andreu
  Author: Josep Andreu

  Using Stephan M. Bernsee smbPitchShifter engine.

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

#ifndef HARMONIZER_H
#define HARMONIZER_H

#include <plugins/YroEffectPlugin.h>
#include <plugins/misc/Resample.h>
#include <plugins/filter/AnalogFilter.h>
#include <plugins/misc/PitchShifter.h>

namespace std {

class Harmonizer : public YroEffectPlugin

{

public:
  Harmonizer(long int Quality, int DS, int uq, int dq);
   ~Harmonizer();
  void render(jack_nframes_t nframes,float *smpsl, float *smpsr);
  void cleanup ();
  void applyfilters (float * efxoutl);
  void adjust(int DS);

  /**
  * member declaration
  */
  enum functions {
  _preset
  , _volume
  , _panning
  , _gain
  , _interval
  , _ffreq
  , _select
  , _note
  , _type
  , _fgain
  , _fq
  , _midi
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
  int  get3() {return getGain();}
  void set3(int value) {setGain(value);}
  int  get4() {return getInterval();}
  void set4(int value) {setInterval(value);}
  int  get5() {return getFfreq();}
  void set5(int value) {setFfreq(value);}
  int  get6() {return getSelect();}
  void set6(int value) {setSelect(value);}
  int  get7() {return getNote();}
  void set7(int value) {setNote(value);}
  int  get8() {return getType();}
  void set8(int value) {setType(value);}
  int  get9() {return getFgain();}
  void set9(int value) {setFgain(value);}
  int  get10() {return getQ();}
  void set10(int value) {setQ(value);}
  int  get11() {return getMidi();}
  void set11(int value) {setMidi(value);}

  /**
  * setter and getter
  */
  int  getVolume();
  void setVolume(int value);
  int  getPanning();
  void setPanning(int value);
  int  getGain();
  void setGain(int value);
  int  getInterval();
  void setInterval(int value);
  int  getFfreq();
  void setFfreq(int value);
  int  getSelect();
  void setSelect(int value);
  int  getNote();
  void setNote(int value);
  int  getType();
  void setType(int value);
  int  getFgain();
  void setFgain(int value);
  int  getQ();
  void setQ(int value);
  int  getMidi();
  void setMidi(int value);

private:
  int Pinterval;
  int PMIDI;
  int PSELECT;
  int mira;
  int DS_state;
  int nPERIOD;
  int nSAMPLE_RATE;
  long window;

  long int hq;

  double u_up;
  double u_down;
  float nfSAMPLE_RATE;

  float *outi;
  float *outo;
  float *templ, *tempr;

private:
  int Pvolume;
  int Pgain;
  int Ppan;
  int Pnote;
  int Ptype;

  int fPfreq;
  int fPgain;
  int fPq;

  float panning;
  float gain;
  float interval;

  AnalogFilter *pl;

  Resample *U_Resample;
  Resample *D_Resample;

  PitchShifter *PS;
  

};

}
#endif
