/*
  Arpie.h - Echo Effect w/ arpeggiated delay
  Copyright (C) 2002-2005 Nasca Octavian Paul
  Author: Nasca Octavian Paul

  Modified for rakarrack by Josep Andreu
  Arpeggiated Echo by Ryan Billing (a.k.a. Transmogrifox)
  Modified by Yannick Roffin
  
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

#ifndef ARPIE_H
#define ARPIE_H

#include <plugins/YroEffectPlugin.h>


namespace std {

class Arpie : public YroEffectPlugin

{
public:
  Arpie();
  ~Arpie();
  void render(jack_nframes_t nframes,float * smpsl, float * smpr);
  void cleanup ();

  /**
  * member declaration
  */
  enum functions {
  _volume
  , _panning
  , _delay
  , _lrdelay
  , _lrcross
  , _fb
  , _hidamp
  , _reverse
  , _harms
  , _pattern
  , _subdiv
  };

  /**
  * setter and getter map
  */
  int  get0() {return getVolume();}
  void set0(int value) {setVolume(value);}
  int  get1() {return getPanning();}
  void set1(int value) {setPanning(value);}
  int  get2() {return getDelay();}
  void set2(int value) {setDelay(value);}
  int  get3() {return getLrdelay();}
  void set3(int value) {setLrdelay(value);}
  int  get4() {return getLrcross();}
  void set4(int value) {setLrcross(value);}
  int  get5() {return getFb();}
  void set5(int value) {setFb(value);}
  int  get6() {return getHidamp();}
  void set6(int value) {setHidamp(value);}
  int  get7() {return getReverse();}
  void set7(int value) {setReverse(value);}
  int  get8() {return getHarms();}
  void set8(int value) {setHarms(value);}
  int  get9() {return getPattern();}
  void set9(int value) {setPattern(value);}
  int  get10() {return getSubdiv();}
  void set10(int value) {setSubdiv(value);}

  /**
  * setter and getter
  */
  int getHarms();
  void setHarms(int value);
  int getSubdiv();
  void setSubdiv(int value);
  int  getVolume();
  void setVolume(int value);
  int  getPanning();
  void setPanning(int value);
  int  getDelay();
  void setDelay(int value);
  int  getLrdelay();
  void setLrdelay(int value);
  int  getLrcross();
  void setLrcross(int value);
  int  getFb();
  void setFb(int value);
  int  getHidamp();
  void setHidamp(int value);
  int  getReverse();
  void setReverse(int value);
  int  getPattern();
  void setPattern(int value);

private:
  float outvolume;
  //Parametrii
  int Pvolume;	//Volumul or E/R
  int Ppanning;	//Panning
  int Pdelay;
  int Plrdelay;	// L/R delay difference
  int Plrcross;	// L/R Mixing
  int Pfb;		//Feed-back-ul
  int Phidamp;
  int Preverse;
  int Ppattern;
  int Pharms;
  int Psubdiv;


  void setvolume (int Pvolume);
  void setpanning (int Ppanning);
  void setdelay (int Pdelay);
  void setlrdelay (int Plrdelay);
  void setlrcross (int Plrcross);
  void setfb (int Pfb);
  void sethidamp (int Phidamp);
  void setreverse (int Preverse);
  void setpattern (int Ppattern);


  //Parametrii reali
  void initdelays ();

  int dl, dr, delay, lrdelay;
  int kl, kr, rvkl, rvkr, rvfl, rvfr, maxx_delay, fade, harmonic, envcnt, invattack;
  int subdiv;
  int *pattern;

  float panning, lrcross, fb, hidamp, reverse;
  float *ldelay, *rdelay;
  float oldl, oldr;		//pt. lpf
  float  Srate_Attack_Coeff, envattack, envswell;
};


}
#endif
