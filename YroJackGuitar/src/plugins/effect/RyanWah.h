/*
  ZynAddSubFX - a software synthesizer
 
  RyanWah.h - "WahWah" effect and others
  Copyright (C) 2002-2005 Nasca Octavian Paul
  Author: Nasca Octavian Paul

  Modified for rakarrack by Ryan Billing

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

#ifndef RYANWAH_H
#define RYANWAH_H

#include <plugins/YroEffectPlugin.h>
#include <plugins/misc/YroLowfrequencyOscillation.h>
#include <plugins/filter/RBFilter.h>

namespace std {

class RyanWah : public YroEffectPlugin

{
public:
  RyanWah();
  ~RyanWah();
  void render(jack_nframes_t nframes,float * smpsl, float * smpsr);
  void cleanup ();

  /**
  * member declaration
  */
  enum functions {
  _preset
  , _volume
  , _q
  , _lfofreq
  , _lforandomness
  , _lfotype
  , _lfostereo
  , _width
  , _ampsns
  , _ampsnsinv
  , _ampsmooth
  , _lp
  , _bp
  , _hp
  , _stages
  , _range
  , _minfreq
  , _variq
  , _qm
  };

  /**
  * setter and getter map
  */
  int  get0() {return getPreset();}
  void set0(int value) {setPreset(value);}
  int  get1() {return getVolume();}
  void set1(int value) {setVolume(value);}
  int  get2() {return getQ();}
  void set2(int value) {setQ(value);}
  int  get3() {return getLfoFreq();}
  void set3(int value) {setLfoFreq(value);}
  int  get4() {return getLfoRandomness();}
  void set4(int value) {setLfoRandomness(value);}
  int  get5() {return getLfoType();}
  void set5(int value) {setLfoType(value);}
  int  get6() {return getLfoStereo();}
  void set6(int value) {setLfoStereo(value);}
  int  get7() {return getWidth();}
  void set7(int value) {setWidth(value);}
  int  get8() {return getAmpsns();}
  void set8(int value) {setAmpsns(value);}
  int  get9() {return getAmpsnsinv();}
  void set9(int value) {setAmpsnsinv(value);}
  int  get10() {return getAmpsmooth();}
  void set10(int value) {setAmpsmooth(value);}
  int  get11() {return getLp();}
  void set11(int value) {setLp(value);}
  int  get12() {return getBp();}
  void set12(int value) {setBp(value);}
  int  get13() {return getHp();}
  void set13(int value) {setHp(value);}
  int  get14() {return getStages();}
  void set14(int value) {setStages(value);}
  int  get15() {return getRange();}
  void set15(int value) {setRange(value);}
  int  get16() {return getMinfreq();}
  void set16(int value) {setMinfreq(value);}
  int  get17() {return getVariq();}
  void set17(int value) {setVariq(value);}
  int  get18() {return getQm();}
  void set18(int value) {setQm(value);}

  /**
  * setter and getter
  */
  int  getVolume();
  void setVolume(int value);
  int  getQ();
  void setQ(int value);
  int  getLfoFreq();
  void setLfoFreq(int value);
  int  getLfoRandomness();
  void setLfoRandomness(int value);
  int  getLfoType();
  void setLfoType(int value);
  int  getLfoStereo();
  void setLfoStereo(int value);
  int  getWidth();
  void setWidth(int value);
  int  getAmpsns();
  void setAmpsns(int value);
  int  getAmpsnsinv();
  void setAmpsnsinv(int value);
  int  getAmpsmooth();
  void setAmpsmooth(int value);
  int  getLp();
  void setLp(int value);
  int  getBp();
  void setBp(int value);
  int  getHp();
  void setHp(int value);
  int  getStages();
  void setStages(int value);
  int  getRange();
  void setRange(int value);
  int  getMinfreq();
  void setMinfreq(int value);
  int  getVariq();
  void setVariq(int value);
  int  getQm();
  void setQm(int value);


private:
  void reinitfilter ();
	  
  int Pvolume;		//For wet/dry mix
  int Ppanning;        //delete.  
  int Pwidth;		//0/127// The width of the lfo of the RyanWah
  int Pampsns;	        //0/127//how the filter varies according to the input amplitude
  int Pampsnsinv;	//-64/64//bias of the filter
  int Pampsmooth;	//0/127//how smooth the input amplitude changes the filter, also for Pampsinsinv smoothing for wah wah pedal
  int Prange;		//10/5000Hz//how far filter can deviate.
  int Php;		//-64/64//high pass filter mix
  int Plp;		//-64/64//low pass filter mix
  int Pbp;		//-64/64//Bandpass filter mix
  int Pq;		//0/127//filter resonance.
  int Pqm;	        //Qmode
  int Pstages;		//0-6// counter// Filter stages
  int Pminfreq;		//30/800//set lowest freq in range.
  int variq;		//on-off//checkbox//Selects whether Q is varied w/ frequency to emulate a wahwah
  

  //Control Parametrii

  int Ftype;
  int Fstages;

  //Valorile interne

  float rpanning, lpanning, depth, ampsns, ampsmooth, wahsmooth, fbias, oldfbias, oldfbias1, oldfbias2, q, frequency, maxfreq, base, ibase, minfreq;
  float ms1, lpmix, hpmix, bpmix;	//mean squares
  float centfreq; //testing
  YroLowfrequencyOscillation lfo;		//lfo-ul RyanWah
  class RBFilter *filterl, *filterr;

   


};

}
#endif
