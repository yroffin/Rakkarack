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

  void setpreset (int npreset);
  void changepar (int npar, int value);
  int getpar (int npar);
  void cleanup ();


  int Ppreset;
  float outvolume;

  float *efxoutl;
  float *efxoutr;


private:

  void setvolume (int Pvolume);
  void setwidth (int Pwidth);
  void setampsns (int Pampsns);
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
