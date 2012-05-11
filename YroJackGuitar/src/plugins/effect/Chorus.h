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

#include <plugins/YroEffectPlugin.h>
#include <plugins/misc/YroLowfrequencyOscillation.h>

namespace std {

class Chorus : public YroEffectPlugin {

public:
	Chorus();
	~Chorus();
	void render(jack_nframes_t nframes, float *outl, float *outr);

	void setPreset(int npreset);
	void cleanup();
	int getPflangemode() const;
	void setPflangemode(int pflangemode);
	int getPoutsub() const;
	void setPoutsub(int poutsub);

	void setPvolume(int Pvolume);
	void setPpanning(int Ppanning);
	void setPdepth(int Pdepth);
	void setPdelay(int Pdelay);
	void setPfb(int Pfb);
	void setPlrcross(int Plrcross);

private:
	//Parametrii Chorus
	YroLowfrequencyOscillation lfo; //lfo-ul chorus
	float outvolume; //this is the volume of effect and is public because need it in system effect. The out volume of s
	int Pvolume;
	int Ppanning;
	int Pdepth; //the depth of the Chorus(ms)
	int Pdelay; //the delay (ms)
	int Pfb; //feedback
	int Plrcross; //feedback
	int Pflangemode; //how the LFO is scaled, to result chorus or flange
	int Poutsub; //if I wish to substract the output instead of the adding it

	//Valorile interne
	int maxdelay;
	int dlk, drk, dlhi, dlhi2;

	float depth, delay, fb, lrcross, panning;
	float dl1, dl2, dr1, dr2, lfol, lfor;
	float *delayl, *delayr;
	float getdelay(float xlfo);
	float dllo, mdel;
};

}
#endif
