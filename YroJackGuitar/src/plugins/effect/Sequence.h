/*
  Rakarrack Guitar FX
 
  Sequence.h - Simple compressor/Sequence effect with easy interface, minimal controls
  Copyright (C) 2010 Ryan Billing
  Author: Ryan Billing & Josep Andreu
  
  This program is free software; you can redistribute it and/or modify
  it under the terms of version 3 of the GNU General Public License 
  as published by the Free Software Foundation.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License (version 2) for more details.

  You should have received a copy of the GNU General Public License (version 2)
  along with this program; if not, write to the Free Software Foundation,
  Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA

*/

#ifndef SEQUENCE_H
#define SEQUENCE_H

#include <plugins/YroEffectPlugin.h>
#include <plugins/misc/Resample.h>
#include <plugins/misc/PitchShifter.h>
#include <plugins/filter/RBFilter.h>

namespace std {

class Sequence : public YroEffectPlugin

{
public:
  Sequence(long int Quality, int DS, int uq, int dq);
  ~Sequence();
   void cleanup (); 
  void render(jack_nframes_t nframes,float * smpsl, float * smpr);
  void changepar (int npar, int value);
  int getpar (int npar);
  void setpreset (int npreset);
  void setranges(int value);
  void settempo(int value);
  void adjust(int DS);
    
  int Ppreset;

  float outvolume;
  
  float *efxoutl;
  float *efxoutr;

private:
  //Parametrii
  int Pvolume;	       //Output Level
  int Psequence[8];	//Sequence.  Each value is 0...127
  int Ptempo;           // Rate
  int Pq;                //Filter Q
  int Pamplitude;     // 0 or 1.  Checkbox on or off...
  int Pstdiff;       // 1 2 3 4 5 6 7 8
  int Pmode;  
  int Prange;
  int tcount, scount, dscount, intperiod; 
  int subdiv; 
  int rndflag;
  int DS_state;
  int nPERIOD;
  int nSAMPLE_RATE;
  
  double u_up;
  double u_down;

  long int hq;
  long window;

  float nfSAMPLE_RATE;
  float MINFREQ,MAXFREQ;
  float fsequence[8];
  float fq;
  float panning;
  float ifperiod,fperiod, seqpower;

  float *outi;
  float *outo;
  float *templ, *tempr;
  
 
  class RBFilter *filterl, *filterr, *modfilterl, *modfilterr;

  Resample *U_Resample;
  Resample *D_Resample;

  PitchShifter *PS;

    

};


}
#endif
