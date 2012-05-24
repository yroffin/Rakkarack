/*
 Rakarrack Audio FX
 
 DualFlanger.C - Super Flanger
 Copyright (C) 2010 Ryan Billing
 Authors:
 Ryan Billing (a.k.a Transmogrifox)  --  Signal Processing
 Copyright (C) 2010 Ryan Billing

 Nasca Octavian Paul -- Remnants of ZynAddSubFX Echo.h structure and utility routines common to ZynSubFX source
 Copyright (C) 2002-2005 Nasca Octavian Paul

 Higher intensity flanging accomplished by picking two points out of the delay line to create a wider notch filter.

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

#include <plugins/effect/DualFlanger.h>
using namespace std;

DualFlanger::DualFlanger() :
		YroEffectPlugin("DualFlanger",
				"Preset1: -32,0,0,110,800,10,-27,16000,1,0,24,64,1,10;"
				"Flange-Wha: 0,0,64,500,3000,50,-40,8000,1,0,196,96,0,0;"
				"FbFlange: 0,0,64,68,75,50,-50,8000,0,1,23,96,5,0;"
				"SoftFlange: -32,0,64,60,10,100,20,16000,0,0,16,90,4,0;"
				"Flanger: -32,0,64,170,1200,50,0,16000,1,0,68,127,0,0;"
				"Chorus1: -15,0,0,42,12,50,-10,1500,0,0,120,0,0,20;"
				"Chorus2: -40,0,0,35,9,67,12,4700,1,1,160,75,0,60;"
				"Preset8: 0,0,0,0,0,0,0,0,0,0,0,0,0,0;"
				"Preset9: 0,0,0,0,0,0,0,0,0,0,0,0,0,0;") {

	period_const = 1.0f / fPERIOD;

	//default values
	ldelay = NULL;
	rdelay = NULL;

	maxx_delay = (int) iSAMPLE_RATE * D_FLANGE_MAX_DELAY;
	ldelay = new float[maxx_delay];
	rdelay = new float[maxx_delay];
	zldelay = new float[maxx_delay];
	zrdelay = new float[maxx_delay];
	fsubtract = 0.5f;
	fhidamp = 1.0f;
	fwidth = 800;
	fdepth = 50;
	zcenter = (int) floorf(0.5f * (fdepth + fwidth));
	base = 7.0f; //sets curve of modulation to frequency relationship
	ibase = 1.0f / base;
	//default values
	rsA = 0.0f;
	rsB = 0.0f;
	lsA = 0.0f;
	lsB = 0.0f;
	setPreset(0);
	cleanup();
}
;

DualFlanger::~DualFlanger() {
}
;

/*
 * Cleanup the effect
 */
void DualFlanger::cleanup() {
	int i;
	for (i = 0; i < maxx_delay; i++) {
		ldelay[i] = 0.0;
		rdelay[i] = 0.0;
		zldelay[i] = 0.0;
		zrdelay[i] = 0.0;
	};

	//loop variables
	l = 0.0f;
	r = 0.0f;
	ldl = 0.0f;
	rdl = 0.0f;
	rflange0 = 0.0f;
	lflange0 = 0.0f;
	rflange1 = 0.0f;
	lflange1 = 0.0f;

}
;

/*
 * Effect output
 */
void DualFlanger::render(jack_nframes_t nframes, float * smpsl, float * smpsr) {
	int i;
	//deal with LFO's
	int tmp0, tmp1;

	float lfol, lfor, lmod, rmod, lmodfreq, rmodfreq, rx0, rx1, lx0, lx1;
	float ldif0, ldif1, rdif0, rdif1; //Difference between fractional delay and floor(fractional delay)
	float drA, drB, dlA, dlB; //LFO inside the loop.

	lfo.render(nframes, &lfol, &lfor);
	lmod = lfol;
	rmod = lfor;
//  lmod = (powf (2.0f, lmod*LOG_FMAX) - 1.0f) * LFO_CONSTANT;  //2^x type sweep for musical interpretation of moving delay line.
//  rmod = (powf (2.0f, rmod*LOG_FMAX) - 1.0f) * LFO_CONSTANT;

	lmodfreq = fdepth + fwidth * (powf(base, lmod) - 1.0f) * ibase; //sets frequency of lowest notch. // 20 <= fdepth <= 4000 // 20 <= width <= 16000 //
	rmodfreq = fdepth + fwidth * (powf(base, rmod) - 1.0f) * ibase;

	if (lmodfreq > 10000.0f)
		lmodfreq = 10000.0f;
	else if (lmodfreq < 10.0f)
		lmodfreq = 10.0f;
	if (rmodfreq > 10000.0)
		rmodfreq = 10000.0f;
	else if (rmodfreq < 10.0f)
		rmodfreq = 10.0f;

	rflange0 = fSAMPLE_RATE * 0.5f / rmodfreq; //Turn the notch frequency into a number for delay
	rflange1 = rflange0 * foffset; //Set relationship of second delay line
	lflange0 = fSAMPLE_RATE * 0.5f / lmodfreq;
	lflange1 = lflange0 * foffset;

	//now is a delay expressed in number of samples.  Number here
	//will be fractional, but will use linear interpolation inside the loop to make a decent guess at
	//the numbers between samples.

	rx0 = (rflange0 - oldrflange0) * period_const; //amount to add each time around the loop.  Less processing of linear LFO interp inside the loop.
	rx1 = (rflange1 - oldrflange1) * period_const;
	lx0 = (lflange0 - oldlflange0) * period_const;
	lx1 = (lflange1 - oldlflange1) * period_const;
	// Now there is a fractional amount to add

	drA = oldrflange0;
	drB = oldrflange1;
	dlA = oldlflange0;
	dlB = oldlflange1;
	// dr, dl variables are the LFO inside the loop.

	oldrflange0 = rflange0;
	oldrflange1 = rflange1;
	oldlflange0 = lflange0;
	oldlflange1 = lflange1;
	//lfo ready...

	for (i = 0; i < iPERIOD; i++) {

		//Delay line utility
		ldl = ldelay[kl];
		rdl = rdelay[kr];
		l = ldl * flrcross + rdl * frlcross;
		r = rdl * flrcross + ldl * frlcross;
		ldl = l;
		rdl = r;
		ldl = smpsl[i] * lpan - ldl * ffb;
		rdl = smpsr[i] * rpan - rdl * ffb;

		//LowPass Filter
		ldelay[kl] = ldl = ldl * (1.0f - fhidamp) + oldl * fhidamp;
		rdelay[kr] = rdl = rdl * (1.0f - fhidamp) + oldr * fhidamp;
		oldl = ldl + DENORMAL_GUARD;
		oldr = rdl + DENORMAL_GUARD;

		if (Pzero) {
			//Offset zero reference delay
			zdl = zldelay[zl];
			zdr = zrdelay[zr];
			zldelay[zl] = smpsl[i];
			zrdelay[zr] = smpsr[i];
			if (--zl < 0) //Cycle delay buffer in reverse so delay time can be indexed directly with addition
				zl = zcenter;
			if (--zr < 0)
				zr = zcenter;
		}

		//End delay line management, start flanging:

		//Right Channel, delay A
		rdif0 = drA - floor(drA);
		tmp0 = (kr + (int) floor(drA)) % maxx_delay;
		tmp1 = tmp0 + 1;
		if (tmp1 >= maxx_delay)
			tmp1 = 0;
		//rsA = rdelay[tmp0] + rdif0 * (rdelay[tmp1] - rdelay[tmp0] );	//here is the first right channel delay
		rsA = rdelay[tmp1] + rdif0 * (rdelay[tmp0] - rsA); //All-pass interpolator

		//Right Channel, delay B
		rdif1 = drB - floor(drB);
		tmp0 = (kr + (int) floor(drB)) % maxx_delay;
		tmp1 = tmp0 + 1;
		if (tmp1 >= maxx_delay)
			tmp1 = 0;
		//rsB = rdelay[tmp0] + rdif1 * (rdelay[tmp1] - rdelay[tmp0]);	//here is the second right channel delay
		rsB = rdelay[tmp1] + rdif1 * (rdelay[tmp0] - rsB);

		//Left Channel, delay A
		ldif0 = dlA - floor(dlA);
		tmp0 = (kl + (int) floor(dlA)) % maxx_delay;
		tmp1 = tmp0 + 1;
		if (tmp1 >= maxx_delay)
			tmp1 = 0;
		//lsA = ldelay[tmp0] + ldif0 * (ldelay[tmp1] - ldelay[tmp0]);	//here is the first left channel delay
		lsA = ldelay[tmp1] + ldif0 * (ldelay[tmp0] - lsA);

		//Left Channel, delay B
		ldif1 = dlB - floor(dlB);
		tmp0 = (kl + (int) floor(dlB)) % maxx_delay;
		tmp1 = tmp0 + 1;
		if (tmp1 >= maxx_delay)
			tmp1 = 0;
		//lsB = ldelay[tmp0] + ldif1 * (ldelay[tmp1] - ldelay[tmp0]);	//here is the second leftt channel delay
		lsB = ldelay[tmp1] + ldif1 * (ldelay[tmp0] - lsB);
		//End flanging, next process outputs

		if (Pzero) {
			efxoutl[i] = dry * smpsl[i]
					+ fsubtract * wet * (fsubtract * (lsA + lsB) + zdl); // Make final FX out mix
			efxoutr[i] = dry * smpsr[i]
					+ fsubtract * wet * (fsubtract * (rsA + rsB) + zdr);
		} else {
			efxoutl[i] = dry * smpsl[i] + wet * fsubtract * (lsA + lsB); // Make final FX out mix
			efxoutr[i] = dry * smpsr[i] + wet * fsubtract * (rsA + rsB);
		}

		if (--kl < 0) //Cycle delay buffer in reverse so delay time can be indexed directly with addition
			kl = maxx_delay;
		if (--kr < 0)
			kr = maxx_delay;

// Increment LFO
		drA += rx0;
		drB += rx1;
		dlA += lx0;
		dlB += lx1;

	};

}
;

void DualFlanger::setWetdry(int value) {
Pwetdry = value;
dry = (float) (Pwetdry + 64) / 128.0f;
wet = 1.0f - dry;
}
void DualFlanger::setPanning(int value) {
Ppanning = value;
if (value < 0) {
rpan = 1.0f + (float) Ppanning / 64.0;
lpan = 1.0f;
} else {
lpan = 1.0f - (float) Ppanning / 64.0;
rpan = 1.0f;
}
;
}
void DualFlanger::setLrcross(int value) {
Plrcross = value;
flrcross = (float) Plrcross / 127.0;
frlcross = 1.0f - flrcross; //keep this out of the DSP loop
}
void DualFlanger::setDepth(int value) {
Pdepth = value;
fdepth = (float) Pdepth;
zcenter = (int) floor(0.5f * (fdepth + fwidth));
}
void DualFlanger::setWidth(int value) {
Pwidth = value;
fwidth = (float) Pwidth;
zcenter = (int) floor(0.5f * (fdepth + fwidth));
}
void DualFlanger::setOff(int value) {
Poffset = value;
foffset = 0.5f + (float) Poffset / 255.0;
}
void DualFlanger::setFb(int value) {
Pfb = value;
ffb = (float) Pfb / 64.5f;
}
void DualFlanger::setHidamp(int value) {
Phidamp = value;
fhidamp = expf(-D_PI * (float) Phidamp / fSAMPLE_RATE);
}
void DualFlanger::setSubtract(int value) {
Psubtract = value;
fsubtract = 0.5f;
if (Psubtract)
fsubtract = -0.5f; //In loop a mult by 0.5f is necessary, so this kills 2 birds with 1...
}
void DualFlanger::setZero(int value) {
Pzero = value;
if (Pzero)
fzero = 1.0f;
}
void DualFlanger::setLfoFreq(int value) {
lfo.setPfreq(value);
}
void DualFlanger::setLfoStereo(int value) {
lfo.setPstereo(value);
}
void DualFlanger::setLfoType(int value) {
lfo.setPlfOtype(value);
}
void DualFlanger::setLfoRandomness(int value) {
lfo.setPrandomness(value);
}

int  DualFlanger::getWetdry() {return Pwetdry;}
int  DualFlanger::getPanning() {return Ppanning;}
int  DualFlanger::getLrcross() {return Plrcross;}
int  DualFlanger::getDepth() {return Pdepth;}
int  DualFlanger::getWidth() {return Pwidth;}
int  DualFlanger::getOff() {return Poffset;}
int  DualFlanger::getFb() {return Pfb;}
int  DualFlanger::getHidamp() {return Phidamp;}
int  DualFlanger::getSubtract() {return Psubtract;}
int  DualFlanger::getZero() {return Pzero;}
int  DualFlanger::getLfoFreq() {return lfo.getPfreq();}
int  DualFlanger::getLfoStereo() {return lfo.getPstereo();}
int  DualFlanger::getLfoType() {return lfo.getPlfOtype();}
int  DualFlanger::getLfoRandomness() {return lfo.getPrandomness();}
