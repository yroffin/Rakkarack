/*
  rakarrack - a guitar effects software

 Opticaltrem.h  -  Opticaltrem Effect definitions

  Copyright (C) 2008-2010 Ryan Billing
  Author: Josep Andreu & Ryan Billing

 This program is free software; you can redistribute it and/or modify
 it under the terms of version 2 of the GNU General Public License
 as published by the Free Software Foundation.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License (version 2) for more details.

 You should have received a copy of the GNU General Public License
 (version2)  along with this program; if not, write to the Free Software
 Foundation,
 Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA

*/


#ifndef Opticaltrem_H
#define Opticaltrem_H

#include <plugins/YroEffectPlugin.h>
#include <plugins/misc/YroLowfrequencyOscillation.h>



namespace std {

class Opticaltrem : public YroEffectPlugin

{

public:

  Opticaltrem();
  ~Opticaltrem();

  void render(jack_nframes_t nframes,float * smpsl, float * smpsr);
  void setpanning(int value);
  void setpreset (int npreset);
  void changepar (int npar, int value);
  int getpar (int npar);
  void cleanup ();

  float *efxoutl;
  float *efxoutr;


private:
  int Pdepth;
  int Ppanning;
  
  float Ra, Rb, R1, b, dTC, dRCl, dRCr, minTC, alphal, alphar, stepl, stepr, oldstepl, oldstepr, fdepth;
  float lstep,rstep;
  float cperiod;
  float gl, oldgl;
  float gr, oldgr; 
  float rpanning, lpanning; 
  YroLowfrequencyOscillation lfo;

  

};

}
#endif
