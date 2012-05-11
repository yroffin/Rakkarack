/*
  rakarrack - a guitar effects software

 Opticaltrem.C  -  Optical tremolo effect

  Copyright (C) 2008-2010 Ryan Billing
  Author: Josep Andreu


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


#include <plugins/effect/Opticaltrem.h>
using namespace std;

Opticaltrem::Opticaltrem() : YroEffectPlugin("Opticaltrem")
{
  
  

R1 = 2700.0f;	   //tremolo circuit series resistance 
Ra = 1000000.0f;  //Cds cell dark resistance.
Ra = logf(Ra);		//this is done for clarity 
Rb = 300.0f;         //Cds cell full illumination
b = (float) exp((float) Ra/(float) logf(Rb)) - CNST_E;
dTC = 0.03f;
dRCl = dTC;
dRCr = dTC;   //Right & left channel dynamic time contsants
minTC = logf(0.005f/dTC);
alphal = 1.0f - cSAMPLE_RATE/(dRCl + cSAMPLE_RATE);
alphar = alphal;
lstep = 0.0f;
rstep = 0.0f;
Pdepth = 127;
Ppanning = 64;
lpanning = 1.0f;
rpanning = 1.0f;
fdepth = 1.0f;  
oldgl = 0.0f;
oldgr = 0.0f;
gl = 0.0f;
gr = 0.0f;
cperiod = 1.0f/fPERIOD;

}

Opticaltrem::~Opticaltrem ()
{
}


void
Opticaltrem::cleanup ()
{


};

void
Opticaltrem::render(jack_nframes_t nframes,float *smpsl, float *smpsr)
{

  int i;
  float lfol, lfor, xl, xr, fxl, fxr;
  float rdiff, ldiff;
  lfo.render(nframes,&lfol, &lfor);

  lfol = 1.0f - lfol*fdepth;
  lfor = 1.0f - lfor*fdepth;   
  
   if (lfol > 1.0f)
    lfol = 1.0f;
  else if (lfol < 0.0f)
    lfol = 0.0f;
  if (lfor > 1.0f)
    lfor = 1.0f;
  else if (lfor < 0.0f)
    lfor = 0.0f;  
    
    lfor = powf(lfor, 1.9f);
    lfol = powf(lfol, 1.9f);  //emulate lamp turn on/off characteristic
    
    //lfo interpolation
    rdiff = (lfor - oldgr)*cperiod;
    ldiff = (lfol - oldgl)*cperiod; 
    gr = lfor;
    gl = lfol;
    oldgr = lfor;
    oldgl = lfol;
    
  for (i = 0; i < iPERIOD; i++)
    {
    //Left Cds   
    stepl = gl*(1.0f - alphal) + alphal*oldstepl;
    oldstepl = stepl;
    dRCl = dTC*expf(stepl*minTC);
    alphal = 1.0f - cSAMPLE_RATE/(dRCl + cSAMPLE_RATE);  
    xl = CNST_E + stepl*b;
    fxl = expf(Ra/logf(xl));     
    fxl = R1/(fxl + R1);
    
    //Right Cds   
    stepr = gr*(1.0f - alphar) + alphar*oldstepr;
    oldstepr = stepr;
    dRCr = dTC*expf(stepr*minTC);
    alphar = 1.0f - cSAMPLE_RATE/(dRCr + cSAMPLE_RATE);        
    xr = CNST_E + stepr*b;
    fxr = expf(Ra/logf(xr));
    fxr = R1/(fxr + R1);  
    
    //Modulate input signal
    efxoutl[i] = lpanning*fxl*smpsl[i];
    efxoutr[i] = rpanning*fxr*smpsr[i];     
    
    gl += ldiff;
    gr += rdiff;  //linear interpolation of LFO 
    
    };

};

void
Opticaltrem::setpanning (int value)
{
  Ppanning = value;
  rpanning = ((float)Ppanning) / 64.0f;
  lpanning = 2.0f - rpanning;
  lpanning = 10.0f * powf(lpanning, 4);
  rpanning = 10.0f * powf(rpanning, 4);
  lpanning = 1.0f - 1.0f/(lpanning + 1.0f);
  rpanning = 1.0f - 1.0f/(rpanning + 1.0f); 
  lpanning *= 1.3f;
  rpanning *= 1.3f; 
};

void
Opticaltrem::setpreset (int npreset)
{
  const int PRESET_SIZE = 6;
  const int NUM_PRESETS = 6;
  int presets[NUM_PRESETS][PRESET_SIZE] = {
    //Fast
    {127, 260, 10, 0, 64, 64},
    //trem2
    {45, 140, 10, 0, 64, 64},
    //hard pan
    {127, 120, 10, 5, 0, 64},
    //soft pan
    {45, 240, 10, 1, 16, 64},    
    //ramp down
    {65, 200, 0, 3, 32, 64},
    //hard ramp
    {127, 480, 0, 3, 32, 64}  
    
  };

  if(npreset<NUM_PRESETS)
  {     
  for (int n = 0; n < PRESET_SIZE; n++)
  changepar (n, presets[npreset][n]);
  }

};

void
Opticaltrem::changepar (int npar, int value)
{

  switch (npar)
    {

    case 0:
      Pdepth = value;
      fdepth = 0.5f + ((float) Pdepth)/254.0f;    
      break;
    case 1:
      lfo.setPfreq(value);
      
      break;
    case 2:
      lfo.setPrandomness(value);
      
      break;
    case 3:
      lfo.setPlfOtype(value);
      
      break;
    case 4:
      lfo.setPstereo(value);
      
      break;
    case 5: // pan
    setpanning(value);
      break;  
    }
   
};

int
Opticaltrem::getpar (int npar)
{

  switch (npar)

    {
    case 0:
      return (Pdepth);
      break;
    case 1:
      return lfo.getPfreq();
      break;
    case 2:
      return lfo.getPrandomness();
      break;
    case 3:
      return lfo.getPlfOtype();
      break;
    case 4:
      return lfo.getPstereo();
      break;
    case 5:
      return (Ppanning); //pan

    }

  return (0);

};


