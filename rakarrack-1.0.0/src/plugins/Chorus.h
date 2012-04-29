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
#include "global.h"
#include "EffectLFO.h"

#include "PluginEffect.h"

class Chorus : public PluginEffect {
public:
  Chorus (float * efxoutl_, float * efxoutr_);
  ~Chorus ();
  void out (float * smpsl, float * smpsr);
  void setpreset (int dgui, int npreset);
  void changepar (int npar, int value);
  int getpar (int npar);
  void cleanup ();

private:
  //Parametrii Chorus
    EffectLFO lfo;		//lfo-ul chorus
  int Pvolume;
  int Ppanning;
  int Pdepth;		//the depth of the Chorus(ms)
  int Pdelay;		//the delay (ms)
  int Pfb;		//feedback
  int Plrcross;	//feedback
  int Pflangemode;	//how the LFO is scaled, to result chorus or flange
  int Poutsub;	//if I wish to substract the output instead of the adding it


  //Control Parametrii
  void setvolume (int Pvolume);
  void setpanning (int Ppanning);
  void setdepth (int Pdepth);
  void setdelay (int Pdelay);
  void setfb (int Pfb);
  void setlrcross (int Plrcross);

  //Valorile interne
  int maxdelay;
  int dlk, drk, dlhi, dlhi2;

  float depth, delay, fb, lrcross, panning;
  float dl1, dl2, dr1, dr2, lfol, lfor;
  float *delayl, *delayr;
  float getdelay (float xlfo);
  float dllo, mdel;
  
  class FPreset *Fpre;
  
};

#endif
