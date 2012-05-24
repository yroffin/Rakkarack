/*
  ZynAddSubFX - a software synthesizer
  Copyright (C) 2002-2005 Nasca Octavian Paul
  Based on Ring Modulator Steve Harris LADSPA plugin.
  Ring.h - Distorsion Effect

  Modified and adapted to rakarrack by Josep Andreu
  
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

#ifndef RING_H
#define RING_H

#include <plugins/YroEffectPlugin.h>



namespace std {

class Ring : public YroEffectPlugin

{
public:
  Ring();
  ~Ring();
  void render(jack_nframes_t nframes,float * smpsl, float * smpr);
  void cleanup ();

  /**
  * member declaration
  */
  enum functions {
  _preset
  , _volume
  , _panning
  , _lrcross
  , _level
  , _depthp
  , _freq
  , _stereo
  , _sin
  , _tri
  , _saw
  , _squ
  , _input
  , _afreq
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
  int  get3() {return getLrcross();}
  void set3(int value) {setLrcross(value);}
  int  get4() {return getLevel();}
  void set4(int value) {setLevel(value);}
  int  get5() {return getDepthp();}
  void set5(int value) {setDepthp(value);}
  int  get6() {return getFreq();}
  void set6(int value) {setFreq(value);}
  int  get7() {return getStereo();}
  void set7(int value) {setStereo(value);}
  int  get8() {return getSin();}
  void set8(int value) {setSin(value);}
  int  get9() {return getTri();}
  void set9(int value) {setTri(value);}
  int  get10() {return getSaw();}
  void set10(int value) {setSaw(value);}
  int  get11() {return getSqu();}
  void set11(int value) {setSqu(value);}
  int  get12() {return getInput();}
  void set12(int value) {setInput(value);}
  int  get13() {return getAfreq();}
  void set13(int value) {setAfreq(value);}

  /**
  * setter and getter
  */
  int  getVolume();
  void setVolume(int value);
  int  getPanning();
  void setPanning(int value);
  int  getLrcross();
  void setLrcross(int value);
  int  getLevel();
  void setLevel(int value);
  int  getDepthp();
  void setDepthp(int value);
  int  getFreq();
  void setFreq(int value);
  int  getStereo();
  void setStereo(int value);
  int  getSin();
  void setSin(int value);
  int  getTri();
  void setTri(int value);
  int  getSaw();
  void setSaw(int value);
  int  getSqu();
  void setSqu(int value);
  int  getInput();
  void setInput(int value);
  int  getAfreq();
  void setAfreq(int value);

private:
  void setScale();
  void Create_Tables();

  /**
   * parameters
   */
  int Pafreq;
  int Pfreq;
  int Pvolume;	//Volumul or E/R
  int Ppanning;	//Panning
  int Plrcross;	// L/R Mixing
  int Pinput;
  int Plevel;		//the ouput amplification
  int Pdepthp; 
  int Psin;		//highpass filter
  int Ptri;
  int Psaw;
  int Psqu;
  int Pstereo;	//0=mono,1=stereo
  

  //Parametrii reali
  int offset;
  float panning, lrcross; 
  float *sin_tbl, *tri_tbl, *saw_tbl, *squ_tbl;
  float sin,tri,saw,squ,scale,depth, idepth;

  
};


}
#endif
