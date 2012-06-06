/*
 Rakarrack   Audio FX software
 Exciter.C - Harmonic Enhancer
 Based in Steve Harris LADSPA Plugin harmonic_gen
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

#include <plugins/effect/Exciter.h>

using namespace std;

Exciter::Exciter() :
		YroEffectPlugin("Exciter", "Plain: 64,0,0,0,0,0,0,0,0,0,0,20000,20;"
				"Loudness: 64,0,0,0,0,24,0,-24,0,24,0,80,20;"
				"Exciter1: 64,64,0,0,0,64,0,0,0,64,0,20000,20;"
				"Exciter2: 64,0,0,0,-32,0,-32,-64,0,48,0,14100,5660;"
				"Exciter3: 64,64,0,-64,0,64,0,-64,0,64,0,20000,20;") {
	//default values
	Pvolume = 50;
	lpffreq = 8000;
	hpffreq = 2500;

	for (int i = 0; i < 10; i++) {
		Prm[i] = 0;
		rm[i] = 0.0f;
	}

	harm = new HarmEnhancer(rm, 2500.0f, 8000.0, 1.0f);

	cleanup();
	setPreset(0);
}

Exciter::~Exciter() {
}
;

/*
 * Cleanup the effect
 */
void Exciter::cleanup() {
	harm->cleanup();
}
;

/*
 * Effect output
 */
void Exciter::render(jack_nframes_t nframes, float * smpsl, float * smpsr) {
	harm->harm_out(nframes, smpsl, smpsr);

}
;

/*
 * Parameter control
 */
void Exciter::setVolume(int value) {
	Pvolume = value;
	outvolume = (float) Pvolume / 127.0f;
	harm->set_vol(0, outvolume);
}
;

void Exciter::setLpf(int value) {
	lpffreq = value;
	harm->set_freql(0, (float) value);
}
;

void Exciter::setHpf(int value) {
	hpffreq = value;
	harm->set_freqh(0, (float) value);
}
;

void Exciter::setHar(int num, int value) {
	float har = 32.0f * ((float) num + 1.0f);
	if (num % 2 == 1)
		har = 11200.0f - 64.0f * ((float) num + 1.0f);
	Prm[num] = value;
	rm[num] = (float) value / har;
	harm->calcula_mag(rm);
}

int Exciter::getVolume() {
	return Pvolume;
}
int Exciter::getLpf() {
	return lpffreq;
}
int Exciter::getHpf() {
	return hpffreq;
}

int Exciter::getHar(int sel) {
	if (sel < 0 || sel > 9)
		return 0;
	return Prm[sel];
}
/**
 * toXml member
*/
const char *Exciter::toXml() {
        char _buffer[256];
        char _formatd[] = {"<attribute name=\"%s\" value=\"%d\" />"};
        char _formatf[] = {"<attribute name=\"%s\" value=\"%9.40f\" />"};
        strcpy(_toXml,"<attributes>");
        sprintf(_buffer,_formatd,"hpffreq",hpffreq);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatd,"lpffreq",lpffreq);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatd,"Prm[10]",Prm[10]);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatd,"Pvolume",Pvolume);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatf,"rm[10]",rm[10]);
        strcat(_toXml,_buffer);
        strcat(_toXml,"</attributes>");
        return _toXml;
}
