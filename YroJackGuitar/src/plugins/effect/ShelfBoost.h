/*
  Rakarrack   Audio FX software 
  ShelfBoost.h - Tone Booster
  Modified for rakarrack by Ryan Billing & Josep Andreu
  
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

#ifndef SHELFBOOST_H
#define SHELFBOOST_H

#include <plugins/YroEffectPlugin.h>
#include <plugins/filter/AnalogFilter.h>



namespace std {

class ShelfBoost : public YroEffectPlugin

{
public:
  ShelfBoost();
  ~ShelfBoost();
  void render(jack_nframes_t nframes,float * smpsl, float * smpr);
  void cleanup ();

  /**
   * member declaration
   */
  enum functions {
  	_preset, _volume, _q1, _freq1, _stereo, _level
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
  	return getQ1();
  }
  void set2(int value) {
  	setQ1(value);
  }
  int get3() {
  	return getFreq1();
  }
  void set3(int value) {
  	setFreq1(value);
  }
  int get4() {
  	return getStereo();
  }
  void set4(int value) {
  	setStereo(value);
  }
  int get5() {
  	return getLevel();
  }
  void set5(int value) {
  	setLevel(value);
  }

  /**
   * setter and getter
   */
  int getVolume();
  void setVolume(int value);
  int getQ1();
  void setQ1(int value);
  int getFreq1();
  void setFreq1(int value);
  int getStereo();
  void setStereo(int value);
  int getLevel();
  void setLevel(int value);

private:
  int Pvolume;
  int Plevel;
  int Pq1;
  int Pfreq1;
  int Pstereo;
  
  float freq1,q1,gain,u_gain; 

  AnalogFilter *RB1l, *RB1r;

   

};


}
#endif
