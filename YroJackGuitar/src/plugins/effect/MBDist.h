/*
 
  MBDist.h - Distorsion Effect

  Copyright (C) 2002-2005 Nasca Octavian Paul
  Author: Nasca Octavian Paul
  ZynAddSubFX - a software synthesizer

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

#ifndef MBDIST_H
#define MBDIST_H

#include <plugins/YroEffectPlugin.h>
#include <plugins/filter/AnalogFilter.h>
#include <plugins/filter/WaveShaper.h>


namespace std {

class MBDist : public YroEffectPlugin

{
public:
  MBDist();
  ~MBDist();
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
  , _drive
  , _level
  , _typel
  , _typem
  , _typeh
  , _voll
  , _volm
  , _volh
  , _negate
  , _cross1
  , _cross2
  , _stereo
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
  int  get4() {return getDrive();}
  void set4(int value) {setDrive(value);}
  int  get5() {return getLevel();}
  void set5(int value) {setLevel(value);}
  int  get6() {return getTypel();}
  void set6(int value) {setTypel(value);}
  int  get7() {return getTypem();}
  void set7(int value) {setTypem(value);}
  int  get8() {return getTypeh();}
  void set8(int value) {setTypeh(value);}
  int  get9() {return getVoll();}
  void set9(int value) {setVoll(value);}
  int  get10() {return getVolm();}
  void set10(int value) {setVolm(value);}
  int  get11() {return getVolh();}
  void set11(int value) {setVolh(value);}
  int  get12() {return getNegate();}
  void set12(int value) {setNegate(value);}
  int  get13() {return getCross1();}
  void set13(int value) {setCross1(value);}
  int  get14() {return getCross2();}
  void set14(int value) {setCross2(value);}
  int  get15() {return getStereo();}
  void set15(int value) {setStereo(value);}

  /**
  * setter and getter
  */
  int  getVolume();
  void setVolume(int value);
  int  getPanning();
  void setPanning(int value);
  int  getLrcross();
  void setLrcross(int value);
  int  getDrive();
  void setDrive(int value);
  int  getLevel();
  void setLevel(int value);
  int  getTypel();
  void setTypel(int value);
  int  getTypem();
  void setTypem(int value);
  int  getTypeh();
  void setTypeh(int value);
  int  getVoll();
  void setVoll(int value);
  int  getVolm();
  void setVolm(int value);
  int  getVolh();
  void setVolh(int value);
  int  getNegate();
  void setNegate(int value);
  int  getCross1();
  void setCross1(int value);
  int  getCross2();
  void setCross2(int value);
  int  getStereo();
  void setStereo(int value);

private:
  float *lowl;
  float *lowr;
  float *midl;
  float *midr;
  float *highl;
  float *highr;

  /**
   * parameter
   */
  int Pvolume;	//Volumul or E/R
  int Ppanning;	//Panning
  int Plrcross;	// L/R Mixing
  int Pdrive;		//the input amplification
  int Plevel;		//the ouput amplification
  int PtypeL;
  int PtypeM;
  int PtypeH;
  int PvolL;
  int PvolM;
  int PvolH;
  int PdriveL;
  int PdriveM;
  int PdriveH;
  		//MBDist type
  int Pnegate;	//if the input is negated 
  int Cross1;		//lowpass filter
  int Cross2;		//highpass filter
  int Pstereo;	//0=mono,1=stereo
  

  //Parametrii reali
  float panning, lrcross;
  float volL,volM,volH;
  AnalogFilter *lpf1l, *lpf1r, *hpf1l, *hpf1r;
  AnalogFilter *lpf2l, *lpf2r, *hpf2l, *hpf2r;
  AnalogFilter *DCl, *DCr;
  
  WaveShaper *mbwshape1l, *mbwshape2l, *mbwshape3l;
  WaveShaper *mbwshape1r, *mbwshape2r, *mbwshape3r;

  

};


}
#endif
