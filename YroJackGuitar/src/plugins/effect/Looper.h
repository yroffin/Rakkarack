/*
  ZynAddSubFX - a software synthesizer
 
  Looper.h - Looper Effect
  Copyright (C) 2002-2005 Nasca Octavian Paul
  Author: Nasca Octavian Paul

  Modified for rakarrack by Ryan Billing & Josep Andreu
  Reverse Echo by Transmogrifox
    
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

#ifndef LOOPER_H
#define LOOPER_H

#include <plugins/YroEffectPlugin.h>
#include <plugins/misc/metronome.h>

namespace std {

class Looper : public YroEffectPlugin

{
public:
  Looper(float size);
  ~Looper();
  void render(jack_nframes_t nframes,float * smpsl, float * smpr);
  void cleanuppt1 ();
  void cleanuppt2 ();
  void cleanup ();

  /**
   * member declaration
   */
  enum functions {
  	_preset,
  	_Volume,
  	_Misc1,
  	_Misc2,
  	_Misc3,
  	_Misc4,
  	_Reverse,
  	_Fade1,
  	_Misc5,
  	_Misc6,
  	_Autoplay,
  	_Fade2,
  	_Rec1,
  	_Rec2,
  	_Link,
  	_Tempo,
  	_Bar,
  	_Metro,
  	_Ms
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
  	return getMisc1();
  }
  void set2(int value) {
  	setMisc1(value);
  }
  int get3() {
  	return getMisc2();
  }
  void set3(int value) {
  	setMisc2(value);
  }
  int get4() {
  	return getMisc3();
  }
  void set4(int value) {
  	setMisc3(value);
  }
  int get5() {
  	return getMisc4();
  }
  void set5(int value) {
  	setMisc4(value);
  }
  int get6() {
  	return getReverse();
  }
  void set6(int value) {
  	setReverse(value);
  }
  int get7() {
  	return getFade1();
  }
  void set7(int value) {
  	setFade1(value);
  }
  int get8() {
  	return getMisc5();
  }
  void set8(int value) {
  	setMisc5(value);
  }
  int get9() {
  	return getMisc6();
  }
  void set9(int value) {
  	setMisc6(value);
  }
  int get10() {
  	return getAutoplay();
  }
  void set10(int value) {
  	setAutoplay(value);
  }
  int get11() {
  	return getFade2();
  }
  void set11(int value) {
  	setFade2(value);
  }
  int get12() {
  	return getRec1();
  }
  void set12(int value) {
  	setRec1(value);
  }
  int get13() {
  	return getRec2();
  }
  void set13(int value) {
  	setRec2(value);
  }
  int get14() {
  	return getLink();
  }
  void set14(int value) {
  	setLink(value);
  }
  int get15() {
  	return getTempo();
  }
  void set15(int value) {
  	setTempo(value);
  }
  int get16() {
  	return getBar();
  }
  void set16(int value) {
  	setBar(value);
  }
  int get17() {
  	return getMetro();
  }
  void set17(int value) {
  	setMetro(value);
  }
  int get18() {
  	return getMs();
  }
  void set18(int value) {
  	setMs(value);
  }

  /**
   * setter and getter
   */
  void setFade();
  int getVolume();
  void setVolume(int value);
  int getMisc1();
  void setMisc1(int value);
  int getMisc2();
  void setMisc2(int value);
  int getMisc3();
  void setMisc3(int value);
  int getMisc4();
  void setMisc4(int value);
  int getReverse();
  void setReverse(int value);
  int getFade1();
  void setFade1(int value);
  int getMisc5();
  void setMisc5(int value);
  int getMisc6();
  void setMisc6(int value);
  int getAutoplay();
  void setAutoplay(int value);
  int getFade2();
  void setFade2(int value);
  int getRec1();
  void setRec1(int value);
  int getRec2();
  void setRec2(int value);
  int getLink();
  void setLink(int value);
  int getTempo();
  void setTempo(int value);
  int getBar();
  void setBar(int value);
  int getMetro();
  void setMetro(int value);
  int getMs();
  void setMs(int value);
  int getMvol();
  void setMvol(int value);

private:
  int looper_bar;
  int looper_qua;
  int Ppreset;
  int progstate[6];
  
  void getstate ();

  float outvolume;

  float *efxoutl;
  float *efxoutr;

  int Pplay;	//set to 1
  int Pstop;	//set to 1


private:

  void clearAll();
  void initdelays ();
  void timeposition(int value);
  int set_len(int value);
  int cal_len(int value);
  


  //Parameters
  int Pvolume;	//Mix
  int Precord;	//set to 1
  int Pclear;   //set to 1
  int Preverse;	//0 or 1
  int Pfade1;    //0...127//  crossfade track1 & 2
  int Pfade2;
  int PT1;
  int PT2;
  int Pautoplay;
  int Prec1;
  int Prec2;
  int Plink;
  int Ptempo;
  int Pbar;
  int Pmetro;
  int Pms;
  
  int kl, kl2, rvkl, rvkl2, maxx_delay, fade, dl, dl2, first_time1, first_time2, rplaystate;
  int barlen, looper_ts;

  float *ldelay, *rdelay;
  float *t2ldelay, *t2rdelay;
  
  float oldl, oldr;		//pt. lpf

  float  Srate_Attack_Coeff, track1gain, track2gain, fade1, fade2, pregain1, pregain2;
  float mvol;
 
  
  metronome ticker;

};


}
#endif
