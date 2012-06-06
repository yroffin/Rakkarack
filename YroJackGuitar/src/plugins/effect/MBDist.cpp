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

MBDist::MBDist() : YroEffectPlugin("MBDist",
		"Saturation: 0,64,0,41,64,26,19,26,41,20,35,0,400,1200,0;"
		"Dist1: 0,64,64,20,64,0,14,13,38,49,40,0,288,1315,0;"
		"Soft: 0,64,0,32,64,6,13,6,50,70,50,0,400,1800,0;"
		"Modulated: 0,64,0,36,64,18,17,18,40,70,30,0,500,2200,0;"
		"Crunch: 0,64,0,24,64,19,14,19,30,80,30,0,800,1800,0;"
		"Dist2: 0,64,0,64,64,22,27,22,25,50,25,0,440,1500,0;"
		"Dist3: 0,64,0,64,64,27,22,27,50,69,50,0,800,1200,0;"
		"Dist4: 0,64,0,30,64,19,25,26,20,51,83,0,329,800,0;") {
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

  setPreset(0);
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
MBDist::setVolume (int value)
{
  Pvolume = value;
  outvolume = (float)Pvolume / 127.0f;
};

void
MBDist::setPanning (int Ppanning)
{
  this->Ppanning = Ppanning;
  panning = ((float)Ppanning + 0.5f) / 127.0f;
};


void
MBDist::setLrcross (int Plrcross)
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

void MBDist::setDrive(int value) {
Pdrive = value;
PdriveL = (int)((float)Pdrive*volL);
PdriveM = (int)((float)Pdrive*volM);
PdriveH = (int)((float)Pdrive*volH);
}
void MBDist::setLevel(int value) {
Plevel = value;
}
void MBDist::setTypel(int value) {
PtypeL = value;
}
void MBDist::setTypem(int value) {
PtypeM = value;
}
void MBDist::setTypeh(int value) {
PtypeH = value;
}
void MBDist::setVoll(int value) {
PvolL = value;
volL = (float) value /100.0;
PdriveL = (int)((float)Pdrive*volL);
}
void MBDist::setVolm(int value) {
PvolM = value;
volM = (float) value /100.0;
PdriveM = (int)((float)Pdrive*volM);
}
void MBDist::setVolh(int value) {
PvolH = value;
volH = (float) value /100.0;
PdriveH = (int)((float)Pdrive*volH);
}
void MBDist::setNegate(int value) {
Pnegate = value;
}
void MBDist::setStereo(int value) {
Pstereo = value;
}

int  MBDist::getVolume() {return Pvolume;}
int  MBDist::getPanning() {return Ppanning;}
int  MBDist::getLrcross() {return Plrcross;}
int  MBDist::getDrive() {return Pdrive;}
int  MBDist::getLevel() {return Plevel;}
int  MBDist::getTypel() {return PtypeL;}
int  MBDist::getTypem() {return PtypeM;}
int  MBDist::getTypeh() {return PtypeH;}
int  MBDist::getVoll() {return PvolL;}
int  MBDist::getVolm() {return PvolM;}
int  MBDist::getVolh() {return PvolH;}
int  MBDist::getNegate() {return Pnegate;}
int  MBDist::getCross1() {return Cross1;}
int  MBDist::getCross2() {return Cross2;}
int  MBDist::getStereo() {return Pstereo;}
/**
 * toXml member
*/
const char *MBDist::toXml() {
        char _buffer[256];
        char _formatd[] = {"<attribute name=\"%s\" value=\"%d\" />"};
        char _formatf[] = {"<attribute name=\"%s\" value=\"%9.40f\" />"};
        strcpy(_toXml,"<attributes>");
        sprintf(_buffer,_formatd,"Cross1",Cross1);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatd,"Cross2",Cross2);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatd,"Pdrive",Pdrive);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatd,"PdriveH",PdriveH);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatd,"PdriveL",PdriveL);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatd,"PdriveM",PdriveM);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatd,"Plevel",Plevel);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatd,"Plrcross",Plrcross);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatd,"Pnegate",Pnegate);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatd,"Ppanning",Ppanning);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatd,"Pstereo",Pstereo);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatd,"PtypeH",PtypeH);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatd,"PtypeL",PtypeL);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatd,"PtypeM",PtypeM);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatd,"PvolH",PvolH);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatd,"PvolL",PvolL);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatd,"PvolM",PvolM);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatd,"Pvolume",Pvolume);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatf,"panning",panning);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatf,"lrcross",lrcross);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatf,"volL",volL);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatf,"volM",volM);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatf,"volH",volH);
        strcat(_toXml,_buffer);
        strcat(_toXml,"</attributes>");
        return _toXml;
}
