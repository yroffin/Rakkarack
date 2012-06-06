/*
 Rakarrack Guitar FX

 Sustainer.C - Simple compressor/sustainer effect with easy interface, minimal controls
 Copyright (C) 2010 Ryan Billing
 Author: Ryan Billing
 
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

#include <stdlib.h>

#include <plugins/effect/Sustainer.h>
using namespace std;

Sustainer::Sustainer() :
		YroEffectPlugin("Sustainer", "Moderate: 79, 54;"
				"Extreme: 16, 127;"
				"Mild: 120, 15;") {
	Pvolume = 64;
	Psustain = 64;
	fsustain = 0.5f;
	level = 0.5f;

	float tmp = 0.01f; //10 ms decay time on peak detectors
	prls = 1.0f - (cSAMPLE_RATE / (cSAMPLE_RATE + tmp));

	tmp = 0.05f; //50 ms att/rel on compressor
	calpha = cSAMPLE_RATE / (cSAMPLE_RATE + tmp);
	cbeta = 1.0f - calpha;
	cthresh = 0.25f;
	cratio = 0.25f;

	timer = 0;
	hold = (int) (iSAMPLE_RATE * 0.0125); //12.5ms
	cleanup();
}
;

Sustainer::~Sustainer() {

}
;

/*
 * Cleanup the effect
 */
void Sustainer::cleanup() {
	compeak = 0.0f;
	compenv = 0.0f;
	oldcompenv = 0.0f;
	cpthresh = cthresh; //dynamic threshold
}
;

/*
 * Effect output
 */
void Sustainer::render(jack_nframes_t nframes, float * smpsl, float * smpsr) {
	int i;
	float auxtempl = 0.0f;
	float auxtempr = 0.0f;
	float auxcombi = 0.0f;

	for (i = 0; i < iPERIOD; i++) //apply compression to auxresampled
			{
		auxtempl = input * smpsl[i];
		auxtempr = input * smpsr[i];
		auxcombi = 0.5f * (auxtempl + auxtempr);
		if (fabs(auxcombi) > compeak) {
			compeak = fabs(auxcombi); //First do peak detection on the signal
			timer = 0;
		}
		if (timer > hold) {
			compeak *= prls;
			timer--;
		}
		timer++;
		compenv = cbeta * oldcompenv + calpha * compeak; //Next average into envelope follower
		oldcompenv = compenv;

		if (compenv > cpthresh) //if envelope of signal exceeds thresh, then compress
				{
			compg = cpthresh + cpthresh * (compenv - cpthresh) / compenv;
			cpthresh = cthresh + cratio * (compg - cpthresh); //cpthresh changes dynamically
			tmpgain = compg / compenv;
		} else {
			tmpgain = 1.0f;
		}

		if (compenv < cpthresh)
			cpthresh = compenv;
		if (cpthresh < cthresh)
			cpthresh = cthresh;

		smpsl[i] = auxtempl * tmpgain * level;
		smpsr[i] = auxtempr * tmpgain * level;
	};
	//End compression
}
;

/*
 * Parameter control
 */
void Sustainer::setVolume(int value) {
	Pvolume = value;
	level = dB2rap(-30.0f * (1.0f - ((float) Pvolume/127.0f)));
}

void Sustainer::setSustain(int value) {
	Psustain = value;
	fsustain = (float) Psustain / 127.0f;
	cratio = 1.25f - fsustain;
	input = dB2rap(42.0f * fsustain - 6.0f);
	cthresh = 0.25 + fsustain;
}

int Sustainer::getVolume() {
	return Pvolume;
}
int Sustainer::getSustain() {
	return Psustain;
}
/**
 * toXml member
*/
const char *Sustainer::toXml() {
        char _buffer[256];
        char _formatd[] = {"<attribute name=\"%s\" value=\"%d\" />"};
        char _formatf[] = {"<attribute name=\"%s\" value=\"%9.40f\" />"};
        strcpy(_toXml,"<attributes>");
        sprintf(_buffer,_formatd,"Psustain",Psustain);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatd,"Pvolume",Pvolume);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatd,"timer",timer);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatd,"hold",hold);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatf,"level",level);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatf,"fsustain",fsustain);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatf,"input",input);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatf,"tmpgain",tmpgain);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatf,"prls",prls);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatf,"compeak",compeak);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatf,"compg",compg);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatf,"compenv",compenv);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatf,"oldcompenv",oldcompenv);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatf,"calpha",calpha);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatf,"cbeta",cbeta);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatf,"cthresh",cthresh);
        strcat(_toXml,_buffer);
        strcat(_toXml,"</attributes>");
        return _toXml;
}
