/*
 
  Shuffle.h - Stereo Shuffle

  Copyright (C) 2002-2005 Nasca Octavian Paul
  Author: Nasca Octavian Paul
  ZynAddSubFX - a software synthesizer

  Modified for rakarrack by Josep Andreu & Hernan Ordiales
  
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

#ifndef SHUFFLE_H
#define SHUFFLE_H

#include <plugins/YroEffectPlugin.h>
#include <plugins/filter/AnalogFilter.h>
#include <plugins/distortion/Distortion.h>

namespace std {

class Shuffle : public YroEffectPlugin

{
public:
  Shuffle();
  ~Shuffle();
  void render(jack_nframes_t nframes,float * smpsl, float * smpr);
  void cleanup ();

  /**
  * member declaration
  */
  enum functions {_preset
  , _volume
  , _GainL
  , _GainML
  , _GainMH
  , _GainH
  , _Cross1
  , _Cross2
  , _Cross3
  , _Cross4
  , _pq
  , _e
  };

  /**
  * setter and getter map
  */
  int  get0() {return getPreset();}
  void set0(int value) {setPreset(value);}
  int  get1() {return getVolume();}
  void set1(int value) {setVolume(value);}
  int  get2() {return getGainL();}
  void set2(int value) {setGainL(value);}
  int  get3() {return getGainML();}
  void set3(int value) {setGainML(value);}
  int  get4() {return getGainMH();}
  void set4(int value) {setGainMH(value);}
  int  get5() {return getGainH();}
  void set5(int value) {setGainH(value);}
  int  get6() {return getCross1();}
  void set6(int value) {setCross1(value);}
  int  get7() {return getCross2();}
  void set7(int value) {setCross2(value);}
  int  get8() {return getCross3();}
  void set8(int value) {setCross3(value);}
  int  get9() {return getCross4();}
  void set9(int value) {setCross4(value);}
  int  get10() {return getPq();}
  void set10(int value) {setPq(value);}
  int  get11() {return getE();}
  void set11(int value) {setE(value);}

  /**
  * setter and getter
  */
  int  getVolume();
  void setVolume(int value);
  int  getGainL();
  void setGainL(int value);
  int  getGainML();
  void setGainML(int value);
  int  getGainMH();
  void setGainMH(int value);
  int  getGainH();
  void setGainH(int value);
  int  getCross1();
  void setCross1(int value);
  int  getCross2();
  void setCross2(int value);
  int  getCross3();
  void setCross3(int value);
  int  getCross4();
  void setCross4(int value);
  int  getPq();
  void setPq(int value);
  int  getE();
  void setE(int value);

  

private:
  float *inputl;
  float *inputr;
  float outvolume;

  //Parametrii
  int Pvolume;	//Volumul or E/R
  int PvolL;
  int PvolML;
  int PvolMH;
  int PvolH;
  int PQ;
  int E;
    
  int Cross1;		//lowpass filter
  int Cross2;		//highpass filter
  int Cross3;
  int Cross4;
  

  //Parametrii reali
  float tmp;
  float volL,volML,volMH,volH;
  AnalogFilter  *lr, *hr;
  AnalogFilter  *mlr,*mhr;
  
  

};


}
#endif
