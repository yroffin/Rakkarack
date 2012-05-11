/*
  rakarrack - a guitar effects software

 pan.C  -  Auto/Pan -  Extra Stereo
  Copyright (C) 2008-2010 Josep Andreu
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


#include <plugins/effect/Pan.h>
using namespace std;



Pan::Pan() : YroEffectPlugin("Pan")
{

  
  


  Ppreset = 0;
  setpreset (Ppreset);
  
  lfo.render(1,&lfol, &lfor);

  cleanup ();


};



Pan::~Pan ()
{
};

void
Pan::cleanup ()
{
};




void
Pan::render(jack_nframes_t nframes,float *smpsl, float *smpsr)
{

  int i;
  float avg, ldiff, rdiff, tmp;
  float pp;
  float coeff_PERIOD = 1.0 / fPERIOD; 
  float fi,P_i;

  if (PextraON)
    {
   
      for (i = 0; i < iPERIOD; i++)

	{

       	  avg = (smpsl[i] + smpsr[i]) * .5f;
	  ldiff = smpsl[i] - avg;
	  rdiff = smpsr[i] - avg;

	  tmp = avg + ldiff * mul;
	  smpsl[i] = tmp*cosf(dvalue);

	  tmp = avg + rdiff * mul;
	  smpsr[i] = tmp*sinf(dvalue);


	}

    }

  if (PAutoPan)
    {

      ll = lfol;
      lr = lfor;
      lfo.render(nframes,&lfol, &lfor);
      for (i = 0; i < iPERIOD; i++)
	{
	  fi = (float) i;
	  P_i = (float) (iPERIOD - i);
	  
	  pp = (ll * P_i + lfol * fi) * coeff_PERIOD;

	  smpsl[i] *= pp * panning;

	  pp =  (lr * P_i + lfor * fi) * coeff_PERIOD;

          smpsr[i] *= pp * (1.0f - panning);

	}

    }







};



void
Pan::setvolume (int Pvolume)
{
  this->Pvolume = Pvolume;
  outvolume = (float)Pvolume / 127.0f;
};



void
Pan::setpanning (int Ppanning)
{
  this->Ppanning = Ppanning;
  panning = ((float)Ppanning)/ 127.0f;
  dvalue= panning*M_PI_2;


};



void
Pan::setextra (int Pextra)
{
  this->Pextra = Pextra;
  mul = 4.0f * (float)Pextra / 127.0f;
};


void
Pan::setpreset (int npreset)
{
  const int PRESET_SIZE = 9;
  const int NUM_PRESETS = 2;
  int presets[NUM_PRESETS][PRESET_SIZE] = {
    //AutoPan
    {64, 64, 26, 0, 0, 0, 0, 1, 0},
    //Extra Stereo
    {64, 64, 80, 0, 0, 0, 10, 0, 1}
  };

  if(npreset<NUM_PRESETS)
   {

  for (int n = 0; n < PRESET_SIZE; n++)
    changepar (n, presets[npreset][n]);
   }

  Ppreset = npreset;


};



void
Pan::changepar (int npar, int value)
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
      lfo.setPfreq(value);
      
      break;
    case 3:
      lfo.setPrandomness(value);
      
      break;
    case 4:
      lfo.setPlfOtype(value);
      
      break;
    case 5:
      lfo.setPstereo(value);
      
      break;
    case 6:
      setextra (value);
      break;
    case 7:
      PAutoPan = value;
      break;
    case 8:
      PextraON = value;
      break;

    }


};


int
Pan::getpar (int npar)
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
      return lfo.getPfreq();
      break;
    case 3:
      return lfo.getPrandomness();
      break;
    case 4:
      return lfo.getPlfOtype();
      break;
    case 5:
      return lfo.getPstereo();
      break;
    case 6:
      return (Pextra);
      break;
    case 7:
      return (PAutoPan);
      break;
    case 8:
      return (PextraON);
      break;
    default:
      return (0);

    }

};
