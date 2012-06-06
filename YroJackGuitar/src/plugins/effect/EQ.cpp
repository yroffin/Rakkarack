/*
 ZynAddSubFX - a software synthesizer
 
 EQ.C - EQ effect
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

#include <plugins/effect/EQ.h>
using namespace std;

EQ::EQ() :
		YroEffectPlugin("EQ", "EQ 1: 67;"
				"EQ 2: 67;") {

	for (int i = 0; i < MAX_EQ_BANDS; i++) {
		filter[i].Ptype = 0;
		filter[i].Pfreq = 64;
		filter[i].Pgain = 64;
		filter[i].Pq = 64;
		filter[i].Pstages = 0;
		filter[i].l = new AnalogFilter(6, 1000.0f, 1.0f, 0);
		filter[i].r = new AnalogFilter(6, 1000.0f, 1.0f, 0);
	};
	//default values
	Pvolume = 50;
	setPreset(0);
	cleanup();
}

EQ::~EQ() {
}

/*
 * Cleanup the effect
 */
void EQ::cleanup() {
	for (int i = 0; i < MAX_EQ_BANDS; i++) {
		filter[i].l->cleanup();
		filter[i].r->cleanup();
	};
}

/*
 * Effect output
 */
void EQ::render(jack_nframes_t nframes, float * smpsl, float * smpsr) {
	int i;
	for (i = 0; i < MAX_EQ_BANDS; i++) {
		if (filter[i].Ptype == 0)
			continue;
		filter[i].l->filterout(iPERIOD, fPERIOD, efxoutl);
		filter[i].r->filterout(iPERIOD, fPERIOD, efxoutr);
	};

	for (i = 0; i < iPERIOD; i++) {
		efxoutl[i] = smpsl[i] * outvolume;
		efxoutr[i] = smpsr[i] * outvolume;
	};

}

/*
 * Parameter control
 */
void EQ::setVolume(int Pvolume) {
	this->Pvolume = Pvolume;
	outvolume = powf(0.005f, (1.0f - (float) Pvolume / 127.0f)) * 10.0f;
}

int EQ::getVolume() {
	return this->Pvolume;
}

void EQ::setEqType(int nb, int value) {
	if (nb >= MAX_EQ_BANDS)
		return;
	if (value > 9)
		value = 0; //has to be changed if more filters will be added
	filter[nb].Ptype = value;
	if (value != 0) {
		filter[nb].l->settype(value - 1);
		filter[nb].r->settype(value - 1);
	}
}
void EQ::setEqFreq(int nb, int value) {
	if (nb >= MAX_EQ_BANDS)
		return;
	float tmp = 0.;
	filter[nb].Pfreq = value;
	tmp = (float) value;
	filter[nb].l->setfreq(tmp);
	filter[nb].r->setfreq(tmp);
}
void EQ::setEqGain(int nb, int value) {
	if (nb >= MAX_EQ_BANDS)
		return;
	float tmp = 0.;
	filter[nb].Pgain = value;
	tmp = 30.0f * ((float) value - 64.0f) / 64.0f;
	filter[nb].l->setgain(tmp);
	filter[nb].r->setgain(tmp);
}
void EQ::setEqQ(int nb, int value) {
	if (nb >= MAX_EQ_BANDS)
		return;
	float tmp = 0.;
	filter[nb].Pq = value;
	tmp = powf(30.0f, ((float) value - 64.0f) / 64.0f);
	filter[nb].l->setq(tmp);
	filter[nb].r->setq(tmp);
}
void EQ::setEqStages(int nb, int value) {
	if (nb >= MAX_EQ_BANDS)
		return;
	if (value >= MAX_FILTER_STAGES)
		value = MAX_FILTER_STAGES - 1;
	filter[nb].Pstages = value;
	filter[nb].l->setstages(value);
	filter[nb].r->setstages(value);
}
int EQ::getEqType(int nb) {
	if (nb >= MAX_EQ_BANDS)
		return (0);
	return (filter[nb].Ptype);
}
int EQ::getEqFreq(int nb) {
	if (nb >= MAX_EQ_BANDS)
		return (0);
	return (filter[nb].Pfreq);
}
int EQ::getEqGain(int nb) {
	if (nb >= MAX_EQ_BANDS)
		return (0);
	return (filter[nb].Pgain);
}
int EQ::getEqQ(int nb) {
	if (nb >= MAX_EQ_BANDS)
		return (0);
	return (filter[nb].Pq);
}
int EQ::getEqStages(int nb) {
	if (nb >= MAX_EQ_BANDS)
		return (0);
	return (filter[nb].Pstages);
}

float EQ::getfreqresponse(float freq) {
	float resp = 1.0f;

	for (int i = 0; i < MAX_EQ_BANDS; i++) {
		if (filter[i].Ptype == 0)
			continue;
		resp *= filter[i].l->H(freq);
	};
	return (rap2dB (resp * outvolume));
}
/**
 * toXml member
*/
const char *EQ::toXml() {
        char _buffer[256];
        char _formatd[] = {"<attribute name=\"%s\" value=\"%d\" />"};
        char _formatdi[] = {"<attribute name=\"%s\" value=\"%d\" instance=\"%d\" />"};
        strcpy(_toXml,"<attributes>");
        for(int i=0;i<MAX_EQ_BANDS;i++) {
			sprintf(_buffer,_formatdi,"Ptype",filter[i].Ptype,i);
			strcat(_toXml,_buffer);
			sprintf(_buffer,_formatdi,"Pfreq",filter[i].Pfreq,i);
			strcat(_toXml,_buffer);
			sprintf(_buffer,_formatdi,"Pgain",filter[i].Pgain,i);
			strcat(_toXml,_buffer);
			sprintf(_buffer,_formatdi,"Pq",filter[i].Pq,i);
			strcat(_toXml,_buffer);
			sprintf(_buffer,_formatdi,"Pstages",filter[i].Pstages,i);
			strcat(_toXml,_buffer);
        }
        sprintf(_buffer,_formatd,"Pvolume",Pvolume);
        strcat(_toXml,_buffer);
        strcat(_toXml,"</attributes>");
        return _toXml;
}
