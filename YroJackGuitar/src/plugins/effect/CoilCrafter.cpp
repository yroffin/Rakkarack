/*
 Rakarrack   Audio FX software
 CoilCrafter.C - Harmonic Enhancer
 Based in Steve Harris LADSPA Plugin harmonic_gen
 Modified for rakarrack by Ryan Billing and Josep Andreu


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

#include <plugins/effect/CoilCrafter.h>

using namespace std;

CoilCrafter::CoilCrafter() :
		YroEffectPlugin("CoilCrafter",
				"HtoS: 32,6,1,3300,16,4400,42,20,0;"
				"StoH: 32,1,6,4400,42,3300,16,20,0;") {

	//default values
	Pvolume = 50;
	Ptone = 20;
	att = 16.0f * powf(PI, fSAMPLE_RATE / 44100.0f);

	tfreqs[0] = 4000.0f;
	tfreqs[1] = 4400.0f;
	tfreqs[2] = 4200.0f;
	tfreqs[3] = 2900.0f;
	tfreqs[4] = 3000.0f;
	tfreqs[5] = 2700.0f;
	tfreqs[6] = 3300.0f;
	tfreqs[7] = 3300.0f;
	tfreqs[8] = 2800.0f;

	tqs[0] = 4.2f;
	tqs[1] = 4.2f;
	tqs[2] = 2.3f;
	tqs[3] = 1.8f;
	tqs[4] = 2.2f;
	tqs[5] = 2.1f;
	tqs[6] = 1.7f;
	tqs[7] = 1.7f;
	tqs[8] = 1.8f;

	for (int i = 0; i < 10; i++) {
		rm[i] = 0.0f;
	}
	rm[0] = 1.0;
	rm[2] = -1.0;
	rm[4] = 1.0;
	rm[6] = -1.0;
	rm[8] = 1.0;

	harm = new HarmEnhancer(rm, 2500.0f, 4400.0f, 1.0f);

	RB1l = new AnalogFilter(2, 2000.0f, 1.0f, 0);
	RB1r = new AnalogFilter(2, 2000.0f, 1.0f, 0);
	RB2l = new AnalogFilter(2, 2000.0f, 1.0f, 0);
	RB2r = new AnalogFilter(2, 2000.0f, 1.0f, 0);

	cleanup();
	setPreset(0);
}

CoilCrafter::~CoilCrafter() {
}
;

/*
 * Cleanup the effect
 */
void CoilCrafter::cleanup() {
	harm->cleanup();
	harm->calcula_mag(rm);
	harm->set_vol(1, 1.0f);

	RB1l->cleanup();
	RB1r->cleanup();
	RB2l->cleanup();
	RB2r->cleanup();

}
;

/*
 * Effect output
 */
void CoilCrafter::render(jack_nframes_t nframes, float * smpsl, float * smpsr) {
	if (Ppo > 0) {
		RB1l->filterout(iPERIOD, fPERIOD, smpsl);
		RB1r->filterout(iPERIOD, fPERIOD, smpsr);

		for (jack_nframes_t i = 0; i < nframes; i++) {
			smpsl[i] *= att;
			smpsr[i] *= att;
		}

	}
	if (Ppd > 0) {
		RB2l->filterout(iPERIOD, fPERIOD, smpsl);
		RB2r->filterout(iPERIOD, fPERIOD, smpsr);
	}

	if (Pmode)
		harm->harm_out(nframes, smpsl, smpsr);

	for (jack_nframes_t i = 0; i < nframes; i++) {
		smpsl[i] *= outvolume;
		smpsr[i] *= outvolume;

		if (Pmode) {
			smpsl[i] *= .5f;
			smpsr[i] *= .5f;
		}
	}

}
;

/*
 * Parameter control
 */
void CoilCrafter::setVolume(int value) {
	Pvolume = value;
	outvolume = (1.0f + (float) Pvolume) / 127.0f;

}

void CoilCrafter::setFreq1(int value) {
	Pfreq1 = value;
	freq1 = (float) value;
	RB1l->setfreq(freq1);
	RB1l->reversecoeffs();
	RB1r->setfreq(freq1);
	RB1r->reversecoeffs();
}

void CoilCrafter::setQ1(int value) {
	Pq1 = value;
	q1 = (float) value / 10.0f;
	RB1l->setq(q1);
	RB1l->reversecoeffs();
	RB1r->setq(q1);
	RB1r->reversecoeffs();
}

void CoilCrafter::setFreq2(int value) {
	Pfreq2 = value;
	freq2 = (float) value;
	RB2l->setfreq(freq2);
	RB2r->setfreq(freq2);
}

void CoilCrafter::setQ2(int value) {
	Pq2 = value;
	q2 = (float) value / 10.0f;
	RB2l->setq(q2);
	RB2r->setq(q2);
}

void CoilCrafter::setHpf(int value) {
	harm->set_freqh(0, (float) Ptone);
}

void CoilCrafter::setPo(int value) {
	Ppo = value;
	if (Ppo > 0) {
		freq1 = tfreqs[value];
		setFreq1((int) freq1);
		q1 = tqs[value];
		setQ1((int) (q1 * 10.0f));
	}
}
void CoilCrafter::setPd(int value) {
	Ppd = value;
	if (Ppd > 0) {
		freq2 = tfreqs[value];
		setFreq2((int) freq2);
		q2 = tqs[value];
		setQ2((int) (q2 * 10.0f));
	}
}
void CoilCrafter::setTone(int value) {
	Ptone = value;
	setHpf(value);
}
void CoilCrafter::setMode(int value) {
	Pmode = value;
}

int CoilCrafter::getVolume() {
	return Pvolume;
}
int CoilCrafter::getPo() {
	return Ppo;
}
int CoilCrafter::getPd() {
	return Ppd;
}
int CoilCrafter::getFreq1() {
	return Pfreq1;
}
int CoilCrafter::getQ1() {
	return Pq1;
}
int CoilCrafter::getFreq2() {
	return Pfreq2;
}
int CoilCrafter::getQ2() {
	return Pq2;
}
int CoilCrafter::getTone() {
	return Ptone;
}
int CoilCrafter::getMode() {
	return Pmode;
}
/**
 * toXml member
*/
const char *CoilCrafter::toXml() {
        char _buffer[256];
        char _formatd[] = {"<attribute name=\"%s\" value=\"%d\" />"};
        char _formatf[] = {"<attribute name=\"%s\" value=\"%9.40f\" />"};
        strcpy(_toXml,"<attributes>");
        sprintf(_buffer,_formatd,"Pfreq1",Pfreq1);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatd,"Pfreq2",Pfreq2);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatd,"Pmode",Pmode);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatd,"Ppd",Ppd);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatd,"Ppo",Ppo);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatd,"Pq1",Pq1);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatd,"Pq2",Pq2);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatd,"Ptone",Ptone);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatd,"Pvolume",Pvolume);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatf,"att",att);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatf,"freq1",freq1);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatf,"q1",q1);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatf,"freq2",freq2);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatf,"q2",q2);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatf,"rm[10]",rm[10]);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatf,"tfreqs[10]",tfreqs[10]);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatf,"tqs[10]",tqs[10]);
        strcat(_toXml,_buffer);
        strcat(_toXml,"</attributes>");
        return _toXml;
}
