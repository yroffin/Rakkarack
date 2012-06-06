/*
 ZynAddSubFX - a software synthesizer
 
 RyanWah.C - "WahWah" effect and others
 Copyright (C) 2002-2005 Nasca Octavian Paul
 Author: Nasca Octavian Paul

 Modified for rakarrack by Ryan Billing

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

#include <plugins/effect/RyanWah.h>
using namespace std;

RyanWah::RyanWah() :
		YroEffectPlugin("RyanWah",
				"WahPedal: 16,10,60,0,0,64,0,0,10,7,-16,40,-3,1,2000,450,1,1;"
						"Mutron: 0,15,138,0,0,64,0,50,0,30,32,0,5,1,2000,60,0,1;"
						"PhaseWah: 0,50,60,0,0,64,30,10,10,30,32,0,10,2,2000,350,1,1;"
						"SucculentPhaser: 64,8,35,10,0,64,50,-10,53,35,28,-16,32,4,2600,300,1,1;"
						"Quacky: 16,10,60,0,0,64,0,40,10,32,-16,40,-3,1,2000,400,1,1;") {
	filterl = NULL;
	filterr = NULL;

	base = 7.0f; //sets curve of modulation to frequency relationship
	ibase = 1.0f / base;

	Pampsns = 0;
	Pampsnsinv = 0;
	Pampsmooth = 0;
	maxfreq = 5000.0f;
	minfreq = 40.0f;
	frequency = 40.0f;
	q = 10.0f;
	Pqm = 1; //Set backward compatibility mode by default.
	hpmix = 0.0f;
	lpmix = 0.5f;
	bpmix = 2.0f;
	wahsmooth = 1.0f - expf(-1.0f / (0.02f * fSAMPLE_RATE)); //0.02 seconds

	Fstages = 1;
	Ftype = 1;
	filterl = new RBFilter(0, 80.0f, 70.0f, 1);
	filterr = new RBFilter(0, 80.0f, 70.0f, 1);
	setPreset(0);
	cleanup();
}

RyanWah::~RyanWah() {
}
;

/*
 * Apply the effect
 */
void RyanWah::render(jack_nframes_t nframes, float * smpsl, float * smpsr) {
	int i;

	float lfol, lfor;
	lfo.render(nframes, &lfol, &lfor);
	lfol *= depth * 5.0f;
	lfor *= depth * 5.0f;

	for (i = 0; i < iPERIOD; i++) {
		efxoutl[i] = smpsl[i];
		efxoutr[i] = smpsr[i];

		float x = (fabsf(smpsl[i]) + fabsf(smpsr[i])) * 0.5f;
		ms1 = ms1 * ampsmooth + x * (1.0f - ampsmooth) + 1e-10f;

		//oldfbias -= 0.001 * oldfbias2;
		oldfbias = oldfbias * (1.0f - wahsmooth) + fbias * wahsmooth + 1e-10f; //smooth MIDI control
		oldfbias1 = oldfbias1 * (1.0f - wahsmooth) + oldfbias * wahsmooth
				+ 1e-10f;
		oldfbias2 = oldfbias2 * (1.0f - wahsmooth) + oldfbias1 * wahsmooth
				+ 1e-10f;
	};

	float rms = ms1 * ampsns + oldfbias2;

	if (rms > 0.0f) //apply some smooth limiting
			{
		rms = 1.0f - 1.0f / (rms * rms + 1.0f);
	} else {
		rms = -1.0f + 1.0f / (rms * rms + 1.0f);
	}

	if (variq)
		q = powf(2.0f, (2.0f * (1.0f - rms) + 1.0f));

	float lmod = (lfol + rms);
	float rmod = (lfor + rms);
	if (lmod > 1.0f)
		lmod = 1.0f;
	if (lmod < 0.0f)
		lmod = 0.0f;
	if (rmod > 1.0f)
		rmod = 1.0f;
	if (rmod < 0.0f)
		rmod = 0.0f;

	//rms*=rms;
	float frl = minfreq + maxfreq * (powf(base, lmod) - 1.0f) * ibase;
	float frr = minfreq + maxfreq * (powf(base, rmod) - 1.0f) * ibase;

	centfreq = frl; //testing variable

	filterl->setfreq_and_q(frl, q);
	filterr->setfreq_and_q(frr, q);

	filterl->filterout(efxoutl);
	filterr->filterout(efxoutr);

}
;

/*
 * Cleanup the effect
 */
void RyanWah::cleanup() {
	reinitfilter();
	ms1 = 0.0;
	oldfbias = oldfbias1 = oldfbias2 = 0.0f;
	filterl->cleanup();
	filterr->cleanup();
}
;

/*
 * Parameter control
 */

void RyanWah::setWidth(int Pwidth) {
	this->Pwidth = Pwidth;
	depth = powf(((float) Pwidth / 127.0f), 2.0f);
}

void RyanWah::setVolume(int Pvolume) {
	this->Pvolume = Pvolume;
	outvolume = (float) Pvolume / 127.0f;
}

void RyanWah::setAmpsns(int Pp) {
	Pampsns = Pp;
	if (Pampsns > 0) {
		ampsns = expf(0.083f * (float) Pampsns);
	} else {
		ampsns = -expf(-0.083f * (float) Pampsns);
	}
	fbias = ((float) Pampsnsinv) / 127.0f;
	ampsmooth = expf(
			-1.0f / ((((float) Pampsmooth) / 127.0f + 0.01f) * fSAMPLE_RATE)); //goes up to 1 second

}
;

void RyanWah::reinitfilter() {
	//setmix (int mix, float lpmix, float bpmix, float hpmix)
	filterl->setmix(1, lpmix, bpmix, hpmix);
	filterr->setmix(1, lpmix, bpmix, hpmix);

}
;

void RyanWah::setQ(int value) {
	Pq = value;
	q = (float) Pq;
}
void RyanWah::setLfoFreq(int value) {
	lfo.setPfreq(value);
}
void RyanWah::setLfoRandomness(int value) {
	lfo.setPrandomness(0);
}
void RyanWah::setLfoType(int value) {
	lfo.setPlfOtype(value);
}
void RyanWah::setLfoStereo(int value) {
	lfo.setPstereo(value);
}
void RyanWah::setAmpsnsinv(int value) {
	Pampsnsinv = value;
	setAmpsns(Pampsns);
}
void RyanWah::setAmpsmooth(int value) {
	Pampsmooth = value;
	setAmpsns(Pampsns);
}
void RyanWah::setLp(int value) {
	Plp = value;
	lpmix = ((float) Plp) / 32.0f;
	reinitfilter();
}
void RyanWah::setBp(int value) {
	Pbp = value;
	bpmix = ((float) Pbp) / 32.0f;
	reinitfilter();
}
void RyanWah::setHp(int value) {
	Php = value;
	hpmix = ((float) Php) / 32.0f;
	reinitfilter();
}
void RyanWah::setStages(int value) {
	Pstages = (value - 1);
	filterl->setstages(Pstages);
	filterr->setstages(Pstages);
	cleanup();
}
void RyanWah::setRange(int value) {
	Prange = value;
	maxfreq = ((float) Prange);
}
void RyanWah::setMinfreq(int value) {
	Pminfreq = value;
	minfreq = (float) value;
}
void RyanWah::setVariq(int value) {
	variq = value;
}
void RyanWah::setQm(int value) {
	Pqm = value;
	filterl->setmode(Pqm);
	filterr->setmode(Pqm);
}

int RyanWah::getVolume() {
	return Pvolume;
}
int RyanWah::getQ() {
	return Pq;
}
int RyanWah::getLfoFreq() {
	return lfo.getPfreq();
}
int RyanWah::getLfoRandomness() {
	return lfo.getPrandomness();
}
int RyanWah::getLfoType() {
	return lfo.getPlfOtype();
}
int RyanWah::getLfoStereo() {
	return lfo.getPstereo();
}
int RyanWah::getWidth() {
	return Pwidth;
}
int RyanWah::getAmpsns() {
	return Pampsns;
}
int RyanWah::getAmpsnsinv() {
	return Pampsnsinv;
}
int RyanWah::getAmpsmooth() {
	return Pampsmooth;
}
int RyanWah::getLp() {
	return Plp;
}
int RyanWah::getBp() {
	return Pbp;
}
int RyanWah::getHp() {
	return Php;
}
int RyanWah::getStages() {
	return Pstages;
}
int RyanWah::getRange() {
	return Prange;
}
int RyanWah::getMinfreq() {
	return Pminfreq;
}
int RyanWah::getVariq() {
	return variq;
}
int RyanWah::getQm() {
	return Pqm;
}
/**
 * toXml member
*/
const char *RyanWah::toXml() {
        char _buffer[256];
        char _formatd[] = {"<attribute name=\"%s\" value=\"%d\" />"};
        char _formatf[] = {"<attribute name=\"%s\" value=\"%9.40f\" />"};
        strcpy(_toXml,"<attributes>");
        sprintf(_buffer,_formatd,"Fstages",Fstages);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatd,"Ftype",Ftype);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatd,"Pampsmooth",Pampsmooth);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatd,"Pampsns",Pampsns);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatd,"Pampsnsinv",Pampsnsinv);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatd,"Pbp",Pbp);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatd,"Php",Php);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatd,"Plp",Plp);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatd,"Pminfreq",Pminfreq);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatd,"Ppanning",Ppanning);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatd,"Pq",Pq);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatd,"Pqm",Pqm);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatd,"Prange",Prange);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatd,"Pstages",Pstages);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatd,"Pvolume",Pvolume);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatd,"Pwidth",Pwidth);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatd,"variq",variq);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatf,"centfreq",centfreq);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatf,"ms1",ms1);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatf,"lpmix",lpmix);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatf,"hpmix",hpmix);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatf,"bpmix",bpmix);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatf,"rpanning",rpanning);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatf,"lpanning",lpanning);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatf,"depth",depth);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatf,"ampsns",ampsns);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatf,"ampsmooth",ampsmooth);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatf,"wahsmooth",wahsmooth);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatf,"fbias",fbias);
        strcat(_toXml,_buffer);
        strcat(_toXml,"</attributes>");
        return _toXml;
}
