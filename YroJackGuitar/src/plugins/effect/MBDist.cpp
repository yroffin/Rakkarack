/*
  MBDist.C - Distorsion effect
  
  ZynAddSubFX - a software synthesizer
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


#include <stdlib.h>

#include <plugins/effect/MBDist.h>
using namespace std;

/*
 * Waveshape (this is called by OscilGen::waveshape and Distorsion::process)
 */



MBDist::MBDist() : YroEffectPlugin("MBDist")
{
  
  

  lowl = (float *) malloc (sizeof (float) * iPERIOD);
  lowr = (float *) malloc (sizeof (float) * iPERIOD);
  midl = (float *) malloc (sizeof (float) * iPERIOD);
  midr = (float *) malloc (sizeof (float) * iPERIOD);
  highl = (float *) malloc (sizeof (float) * iPERIOD);
  highr = (float *) malloc (sizeof (float) * iPERIOD);


  lpf1l = new AnalogFilter (2, 500.0f, .7071f, 0);
  lpf1r = new AnalogFilter (2, 500.0f, .7071f, 0);
  hpf1l = new AnalogFilter (3, 500.0f, .7071f, 0);
  hpf1r = new AnalogFilter (3, 500.0f, .7071f, 0);
  lpf2l = new AnalogFilter (2, 2500.0f, .7071f, 0);
  lpf2r = new AnalogFilter (2, 2500.0f, .7071f, 0);
  hpf2l = new AnalogFilter (3, 2500.0f, .7071f, 0);
  hpf2r = new AnalogFilter (3, 2500.0f, .7071f, 0);
  DCl = new AnalogFilter (3, 30, 1, 0);
  DCr = new AnalogFilter (3, 30, 1, 0);
  DCl->setfreq (30.0f);
  DCr->setfreq (30.0f);


  mbwshape1l = new WaveShaper();
  mbwshape2l = new WaveShaper();
  mbwshape3l = new WaveShaper();

  mbwshape1r = new WaveShaper();
  mbwshape2r = new WaveShaper();
  mbwshape3r = new WaveShaper();
  
  //default values
  Ppreset = 0;
  Pvolume = 50;
  Plrcross = 40;
  Pdrive = 90;
  Plevel = 64;
  PtypeL = 0;
  PtypeM = 0;
  PtypeH = 0;
  PvolL = 0;
  PvolM = 0;
  PvolH = 0;
  Pnegate = 0;
  Pstereo = 0;

  setpreset (Ppreset);
  cleanup ();
};

MBDist::~MBDist ()
{
};

/*
 * Cleanup the effect
 */
void
MBDist::cleanup ()
{
  lpf1l->cleanup ();
  hpf1l->cleanup ();
  lpf1r->cleanup ();
  hpf1r->cleanup ();
  lpf2l->cleanup ();
  hpf2l->cleanup ();
  lpf2r->cleanup ();
  hpf2r->cleanup ();
  DCl->cleanup();
  DCr->cleanup();

};
/*
 * Effect output
 */
void
MBDist::render(jack_nframes_t nframes,float * smpsl, float * smpsr)
{
  int i;
  float l, r, lout, rout;

  float inputvol = powf (5.0f, ((float)Pdrive - 32.0f) / 127.0f);
  if (Pnegate != 0)
    inputvol *= -1.0f;


  if (Pstereo)
    {				
      for (i = 0; i < iPERIOD; i++)
	{
	  efxoutl[i] = smpsl[i] * inputvol * 2.0f;
	  efxoutr[i] = smpsr[i] * inputvol * 2.0f;        
	};
    }
 else
     {
      for (i = 0; i < iPERIOD; i++)
	{
	  efxoutl[i] =
	    (smpsl[i]  +  smpsr[i] ) * inputvol;
	};
    };


  memcpy(lowl,efxoutl,sizeof(float) * iPERIOD);
  memcpy(midl,efxoutl,sizeof(float) * iPERIOD);
  memcpy(highl,efxoutl,sizeof(float) * iPERIOD);

  lpf1l->filterout(iPERIOD,fPERIOD,lowl);
  hpf1l->filterout(iPERIOD,fPERIOD,midl);
  lpf2l->filterout(iPERIOD,fPERIOD,midl);
  hpf2l->filterout(iPERIOD,fPERIOD,highl);

if(volL> 0)  mbwshape1l->waveshapesmps (iPERIOD, lowl, PtypeL, PdriveL, 1);
if(volM> 0)  mbwshape2l->waveshapesmps (iPERIOD, midl, PtypeM, PdriveM, 1);
if(volH> 0)  mbwshape3l->waveshapesmps (iPERIOD, highl, PtypeH, PdriveH, 1);


if(Pstereo)
{
  memcpy(lowr,efxoutr,sizeof(float) * iPERIOD);
  memcpy(midr,efxoutr,sizeof(float) * iPERIOD);
  memcpy(highr,efxoutr,sizeof(float) * iPERIOD);

  lpf1r->filterout(iPERIOD,fPERIOD,lowr);
  hpf1r->filterout(iPERIOD,fPERIOD,midr);
  lpf2r->filterout(iPERIOD,fPERIOD,midr);
  hpf2r->filterout(iPERIOD,fPERIOD,highr);
  
if(volL> 0)  mbwshape1r->waveshapesmps (iPERIOD, lowr, PtypeL, PdriveL, 1);
if(volM> 0)  mbwshape2r->waveshapesmps (iPERIOD, midr, PtypeM, PdriveM, 1);
if(volH> 0)  mbwshape3r->waveshapesmps (iPERIOD, highr, PtypeH, PdriveH, 1);


}

  for (i = 0; i < iPERIOD; i++)
  {
    efxoutl[i]=lowl[i]*volL+midl[i]*volM+highl[i]*volH;
    if (Pstereo) efxoutr[i]=lowr[i]*volL+midr[i]*volM+highr[i]*volH;
  }      
    
  if (!Pstereo) memcpy(efxoutr, efxoutl, sizeof(float)* iPERIOD);    
 

  float level = dB2rap (60.0f * (float)Plevel / 127.0f - 40.0f);

  for (i = 0; i < iPERIOD; i++)
    {
      lout = efxoutl[i];
      rout = efxoutr[i];
      
      l = lout * (1.0f - lrcross) + rout * lrcross;
      r = rout * (1.0f - lrcross) + lout * lrcross;
       
      efxoutl[i] = l * 2.0f * level * panning;
      efxoutr[i] = r * 2.0f * level * (1.0f -panning);  

    };
    
    DCr->filterout (iPERIOD,fPERIOD,efxoutr);
    DCl->filterout (iPERIOD,fPERIOD,efxoutl);



};


/*
 * Parameter control
 */
void
MBDist::setvolume (int value)
{
  Pvolume = value;
  outvolume = (float)Pvolume / 127.0f;
};

void
MBDist::setpanning (int Ppanning)
{
  this->Ppanning = Ppanning;
  panning = ((float)Ppanning + 0.5f) / 127.0f;
};


void
MBDist::setlrcross (int Plrcross)
{
  this->Plrcross = Plrcross;
  lrcross = (float)Plrcross / 127.0f * 1.0f;
};

void
MBDist::setCross1 (int value)
{
  Cross1 = value;
  lpf1l->setfreq ((float)value);
  lpf1r->setfreq ((float)value);
  hpf1l->setfreq ((float)value);
  hpf1r->setfreq ((float)value);


};

void
MBDist::setCross2 (int value)
{
  Cross2 = value;
  hpf2l->setfreq ((float)value);
  hpf2r->setfreq ((float)value);
  lpf2l->setfreq ((float)value);
  lpf2r->setfreq ((float)value);

  
};


void
MBDist::setpreset (int npreset)
{
  const int PRESET_SIZE = 15;
  const int NUM_PRESETS = 8;
  int presets[NUM_PRESETS][PRESET_SIZE] = {
    //Saturation
    {0, 64, 0, 41, 64, 26, 19, 26, 41, 20, 35, 0, 400, 1200, 0}, 
    //Dist 1
    {0, 64, 64, 20, 64, 0, 14, 13, 38, 49, 40, 0, 288, 1315, 0},
    //Soft
    {0, 64, 0, 32, 64, 6, 13, 6, 50, 70, 50, 0, 400, 1800, 0},
    //Modulated
    {0, 64, 0, 36, 64, 18, 17, 18, 40, 70, 30, 0, 500, 2200, 0},
    //Crunch
    {0, 64, 0, 24, 64, 19, 14, 19, 30, 80, 30, 0, 800, 1800, 0},
    //Dist 2
    {0, 64, 0, 64, 64, 22, 27, 22, 25, 50, 25, 0, 440, 1500, 0},
    //Dist 3
    {0, 64, 0, 64, 64, 27, 22, 27, 50, 69, 50, 0, 800, 1200, 0},
    //Dist 4
    {0, 64, 0, 30, 64, 19, 25, 26, 20, 51, 83, 0, 329, 800, 0}
     
  };
  if(npreset<NUM_PRESETS)
  {     

  for (int n = 0; n < PRESET_SIZE; n++)
    changepar (n, presets[npreset][n]);
  }
  Ppreset = npreset;
  cleanup ();
};


void
MBDist::changepar (int npar, int value)
{
  switch (npar)
    {
    case 0:
      setvolume (value);
      break;
    case 1:
      setpanning (value);
      break;
    case 2:
      setlrcross (value);
      break;
    case 3:
      Pdrive = value;
      PdriveL = (int)((float)Pdrive*volL);
      PdriveM = (int)((float)Pdrive*volM);
      PdriveH = (int)((float)Pdrive*volH);
      break;
    case 4:
      Plevel = value;
      break;
    case 5:
      PtypeL = value;
      break;
    case 6:
      PtypeM = value;
      break;
    case 7:
      PtypeH = value;
      break;
    case 8:
      PvolL = value;
      volL = (float) value /100.0;
      PdriveL = (int)((float)Pdrive*volL);
      break;
    case 9:
      PvolM = value;
      volM = (float) value /100.0;
      PdriveM = (int)((float)Pdrive*volM);
      break;
    case 10:
      PvolH = value;
      volH = (float) value /100.0;
      PdriveH = (int)((float)Pdrive*volH);
      break;
    case 11:
      Pnegate = value;
      break;
    case 12:
      setCross1 (value);
      break;
    case 13:
      setCross2 (value);
      break;
    case 14:
      Pstereo = value;
      break;
    };
};

int
MBDist::getpar (int npar)
{
  switch (npar)
    {
    case 0:
      return (Pvolume);
      break;
    case 1:
      return (Ppanning);
      break;
    case 2:
      return (Plrcross);
      break;
    case 3:
      return (Pdrive);
      break;
    case 4:
      return (Plevel);
      break;
    case 5:
      return (PtypeL);
      break;
    case 6:
      return (PtypeM);
      break;
    case 7:
      return (PtypeH);
      break;
    case 8:
      return (PvolL);
      break;
    case 9:
      return (PvolM);
      break;
    case 10:
      return (PvolH);
      break;
    case 11:
      return (Pnegate);
      break;
    case 12:
      return (Cross1);
      break;
    case 13:
      return (Cross2);
      break;
    case 14:
      return (Pstereo);
      break;
    };
  return (0);			//in case of bogus parameter number
};

