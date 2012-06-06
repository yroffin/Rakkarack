/*
 ZynAddSubFX - a software synthesizer
 
 Echo.C - Echo effect
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

#include <plugins/effect/MusicDelay.h>
using namespace std;

MusicDelay::MusicDelay() :
		YroEffectPlugin("MusicDelay",
				"Echo1: 64,0,2,7,0,59,0,127,4,59,106,75,75;"
						"Echo2: 67,0,3,7,0,59,0,127,6,69,60,127,127;") {
	//default values
	Pvolume = 50;
	Ppanning1 = 64;
	Ppanning2 = 64;
	Pgain1 = 64;
	Pgain2 = 64;
	Pdelay1 = 60;
	Pdelay2 = 60;
	Plrdelay = 100;
	Plrcross = 100;
	Pfb1 = 40;
	Pfb2 = 40;
	Ptempo = 100;
	Phidamp = 60;

	maxx_delay = iSAMPLE_RATE * MAX_DELAY;
	ldelay1 = new float[maxx_delay];
	rdelay1 = new float[maxx_delay];
	ldelay2 = new float[maxx_delay];
	rdelay2 = new float[maxx_delay];

	dl1 = maxx_delay - 1;
	dl2 = maxx_delay - 1;
	dr1 = maxx_delay - 1;
	dr2 = maxx_delay - 1;

	lrdelay = 0;
	setPreset(0);
	cleanup();
}

MusicDelay::~MusicDelay() {
}

/*
 * Cleanup the effect
 */
void MusicDelay::cleanup() {
	int i;
	for (i = 0; i < dl1; i++)
		ldelay1[i] = 0.0;
	for (i = 0; i < dr1; i++)
		rdelay1[i] = 0.0;
	for (i = 0; i < dl2; i++)
		ldelay2[i] = 0.0;
	for (i = 0; i < dr2; i++)
		rdelay2[i] = 0.0;

	oldl1 = 0.0;
	oldr1 = 0.0;
	oldl2 = 0.0;
	oldr2 = 0.0;
}
;

/*
 * Initialize the delays
 */
void MusicDelay::initdelays() {
	int i;
	kl1 = 0;
	kr1 = 0;

	if (delay1 >= maxx_delay)
		delay1 = maxx_delay - 1;
	if (delay2 >= maxx_delay)
		delay2 = maxx_delay - 1;

	dl1 = delay1;
	if (dl1 < 1)
		dl1 = 1;
	dr1 = delay1;
	if (dr1 < 1)
		dr1 = 1;
	kl2 = 0;
	kr2 = 0;
	dl2 = delay2 + lrdelay;
	if (dl2 < 1)
		dl2 = 1;
	dr2 = delay2 + lrdelay;
	if (dr2 < 1)
		dr2 = 1;

	for (i = dl1; i < maxx_delay; i++)
		ldelay1[i] = 0.0;
	for (i = dl2; i < maxx_delay; i++)
		ldelay2[i] = 0.0;

	for (i = dr1; i < maxx_delay; i++)
		rdelay1[i] = 0.0;
	for (i = dr2; i < maxx_delay; i++)
		rdelay2[i] = 0.0;

	cleanup();
}
;

/*
 * Effect output
 */
void MusicDelay::render(jack_nframes_t nframes, float * smpsl, float * smpsr) {
	int i;
	float l1, r1, ldl1, rdl1, l2, r2, ldl2, rdl2;

	for (i = 0; i < iPERIOD; i++) {
		ldl1 = ldelay1[kl1];
		rdl1 = rdelay1[kr1];
		l1 = ldl1 * (1.0f - lrcross) + rdl1 * lrcross;
		r1 = rdl1 * (1.0f - lrcross) + ldl1 * lrcross;
		ldl1 = l1;
		rdl1 = r1;

		ldl2 = ldelay2[kl2];
		rdl2 = rdelay2[kr2];
		l2 = ldl2 * (1.0f - lrcross) + rdl2 * lrcross;
		r2 = rdl2 * (1.0f - lrcross) + ldl2 * lrcross;
		ldl2 = l2;
		rdl2 = r2;

		ldl1 = smpsl[i] * gain1 * panning1 - ldl1 * fb1;
		rdl1 = smpsr[i] * gain1 * (1.0f - panning1) - rdl1 * fb1;

		ldl2 = smpsl[i] * gain2 * panning2 - ldl2 * fb2;
		rdl2 = smpsr[i] * gain2 * (1.0f - panning2) - rdl2 * fb2;

		efxoutl[i] = (ldl1 + ldl2) * 2.0f;
		efxoutr[i] = (rdl1 + rdl2) * 2.0f;

		//LowPass Filter
		ldelay1[kl1] = ldl1 = ldl1 * hidamp + oldl1 * (1.0f - hidamp);
		rdelay1[kr1] = rdl1 = rdl1 * hidamp + oldr1 * (1.0f - hidamp);
		oldl1 = ldl1;
		oldr1 = rdl1;

		ldelay2[kl2] = ldl2 = ldl2 * hidamp + oldl2 * (1.0f - hidamp);
		rdelay2[kr2] = rdl2 = rdl2 * hidamp + oldr2 * (1.0f - hidamp);
		oldl2 = ldl2;
		oldr2 = rdl2;

		if (++kl1 >= dl1)
			kl1 = 0;
		if (++kr1 >= dr1)
			kr1 = 0;

		if (++kl2 >= dl2)
			kl2 = 0;
		if (++kr2 >= dr2)
			kr2 = 0;

	};

}
;

/*
 * Parameter control
 */
void MusicDelay::setVolume(int Pvolume) {
	this->Pvolume = Pvolume;
	outvolume = (float) Pvolume / 127.0f;

}

void MusicDelay::setPanning(int num, int Ppanning) {

	switch (num) {
	case 1:
		this->Ppanning1 = Ppanning;
		panning1 = ((float) Ppanning1 + 0.5f) / 127.0f;
		break;
	case 2:
		this->Ppanning2 = Ppanning;
		panning2 = ((float) Ppanning2 + 0.5f) / 127.0f;
		break;
	}

}
;

void MusicDelay::setDelay(int num, int Pdelay) {

	float ntem = 60.0f / (float) Ptempo;
	float coef;
	switch (num) {
	case 1:
		this->Pdelay1 = Pdelay;
		break;
	case 2:
		this->Pdelay2 = Pdelay;
		break;
	case 3:
		this->Plrdelay = Pdelay;
		break;
	}

	delay1 = lrintf((ntem / (float) Pdelay1) * fSAMPLE_RATE);

	if (Plrdelay != 0)
		coef = ntem / (float) Plrdelay;
	else
		coef = 0;

	delay2 = lrintf((coef + (ntem / (float) Pdelay2)) * fSAMPLE_RATE);

	initdelays();

}
;

void MusicDelay::setGain(int num, int PGain) {

	switch (num) {
	case 1:
		this->Pgain1 = PGain;
		gain1 = (float) Pgain1 / 127.0f;
		break;
	case 2:
		this->Pgain2 = PGain;
		gain2 = (float) Pgain2 / 127.0f;
		break;
	}

}
;

void MusicDelay::setLrcross(int Plrcross) {
	this->Plrcross = Plrcross;
	lrcross = (float) Plrcross / 127.0f * 1.0f;
}
;

void MusicDelay::setFb(int num, int Pfb) {
	switch (num) {
	case 1:
		this->Pfb1 = Pfb;
		fb1 = (float) Pfb1 / 127.0f;
		break;
	case 2:
		this->Pfb2 = Pfb;
		fb2 = (float) Pfb2 / 127.0f;
		break;
	}
}
;

void MusicDelay::setHidamp(int Phidamp) {
	this->Phidamp = Phidamp;
	hidamp = 1.0f - (float) Phidamp / 127.0f;
}
;

void MusicDelay::setTempo(int Ptempo) {

	float coef = 0.0;

	this->Ptempo = Ptempo;
	float ntem = 60.0f / (float) Ptempo;

	delay1 = lrintf((ntem / (float) Pdelay1) * fSAMPLE_RATE);
	if (Plrdelay != 0)
		coef = ntem / (float) Plrdelay;
	else
		coef = 0;
	delay2 = lrintf((coef + (ntem / (float) Pdelay2)) * fSAMPLE_RATE);

	initdelays();
}

void MusicDelay::setPanning1(int value) {
	setPanning(1, value);
}
void MusicDelay::setPanning2(int value) {
	setPanning(2, value);
}
void MusicDelay::setDelay1(int value) {
	setDelay(1, value);
}
void MusicDelay::setDelay2(int value) {
	setDelay(2, value);
}
void MusicDelay::setDelay3(int value) {
	setDelay(3, value);
}
void MusicDelay::setFb1(int value) {
	setFb(1, value);
}
void MusicDelay::setFb2(int value) {
	setFb(2, value);
}
void MusicDelay::setGain1(int value) {
	setGain(1, value);
}
void MusicDelay::setGain2(int value) {
	setGain(2, value);
}

int MusicDelay::getVolume() {
	return Pvolume;
}
int MusicDelay::getPanning1() {
	return Ppanning1;
}
int MusicDelay::getDelay1() {
	return Pdelay1;
}
int MusicDelay::getDelay3() {
	return Plrdelay;
}
int MusicDelay::getLrcross() {
	return Plrcross;
}
int MusicDelay::getFb1() {
	return Pfb1;
}
int MusicDelay::getHidamp() {
	return Phidamp;
}
int MusicDelay::getPanning2() {
	return Ppanning2;
}
int MusicDelay::getDelay2() {
	return Pdelay2;
}
int MusicDelay::getFb2() {
	return Pfb2;
}
int MusicDelay::getTempo() {
	return Ptempo;
}
int MusicDelay::getGain1() {
	return Pgain1;
}
int MusicDelay::getGain2() {
	return Pgain2;
}
/**
 * toXml member
*/
const char *MusicDelay::toXml() {
        char _buffer[256];
        char _formatd[] = {"<attribute name=\"%s\" value=\"%d\" />"};
        char _formatf[] = {"<attribute name=\"%s\" value=\"%9.40f\" />"};
        strcpy(_toXml,"<attributes>");
        sprintf(_buffer,_formatd,"dl1",dl1);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatd,"dr1",dr1);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatd,"dl2",dl2);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatd,"dr2",dr2);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatd,"delay1",delay1);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatd,"delay2",delay2);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatd,"lrdelay",lrdelay);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatd,"kl1",kl1);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatd,"kr1",kr1);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatd,"kl2",kl2);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatd,"kr2",kr2);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatd,"maxx_delay",maxx_delay);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatd,"Pdelay1",Pdelay1);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatd,"Pdelay2",Pdelay2);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatd,"Pfb1",Pfb1);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatd,"Pfb2",Pfb2);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatd,"Pgain1",Pgain1);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatd,"Pgain2",Pgain2);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatd,"Phidamp",Phidamp);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatd,"Plrcross",Plrcross);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatd,"Plrdelay",Plrdelay);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatd,"Ppanning1",Ppanning1);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatd,"Ppanning2",Ppanning2);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatd,"Ptempo",Ptempo);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatd,"Pvolume",Pvolume);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatf,"gain1",gain1);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatf,"gain2",gain2);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatf,"oldl1",oldl1);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatf,"oldr1",oldr1);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatf,"oldl2",oldl2);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatf,"oldr2",oldr2);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatf,"panning1",panning1);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatf,"panning2",panning2);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatf,"lrcross",lrcross);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatf,"fb1",fb1);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatf,"fb2",fb2);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatf,"hidamp",hidamp);
        strcat(_toXml,_buffer);
        strcat(_toXml,"</attributes>");
        return _toXml;
}
