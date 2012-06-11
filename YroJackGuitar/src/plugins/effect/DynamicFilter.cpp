/*
 ZynAddSubFX - a software synthesizer
 
 DynamicFilter.C - "WahWah" effect and others
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

#include <plugins/effect/DynamicFilter.h>
using namespace std;

DynamicFilter::DynamicFilter() :
		YroEffectPlugin("DynamicFilter",
				"WahWah: 64, 64, 80, 0, 0, 64, 70, 90, 0, 60;"
						"AutoWah: 64, 64, 70, 0, 0, 80, 70, 0, 0, 60;"
						"Sweep: 64, 64, 30, 0, 0, 50, 80, 0, 0, 60;"
						"VocalMorph1: 64, 64, 80, 0, 0, 64, 70, 64, 0, 60;"
						"VocalMorph1: 64, 64, 50, 0, 0, 96, 64, 0, 0, 60;") {

	filterl = NULL;
	filterr = NULL;
	filterpars = new FilterParams(0, 64, 64);
	setPreset(0);
	cleanup();
}
;

DynamicFilter::~DynamicFilter() {
}
;

/*
 * Apply the effect
 */
void DynamicFilter::render(jack_nframes_t nframes, float * smpsl,
		float * smpsr) {
	int i;
	float lfol, lfor;

	if (filterpars->changed) {
		filterpars->changed = false;
		cleanup();
	};

	lfo.render(nframes, &lfol, &lfor);
	lfol *= depth * 5.0f;
	lfor *= depth * 5.0f;
	float freq = filterpars->getfreq();
	float q = filterpars->getq();

	for (i = 0; i < iPERIOD; i++) {
		efxoutl[i] = smpsl[i];
		efxoutr[i] = smpsr[i];

		float x = (fabsf(smpsl[i]) + fabsf(smpsr[i])) * 0.5f;
		ms1 = ms1 * (1.0f - ampsmooth) + x * ampsmooth + 1e-10f;
	};

	float ampsmooth2 = powf(ampsmooth, 0.2f) * 0.3f;
	ms2 = ms2 * (1.0f - ampsmooth2) + ms1 * ampsmooth2;
	ms3 = ms3 * (1.0f - ampsmooth2) + ms2 * ampsmooth2;
	ms4 = ms4 * (1.0f - ampsmooth2) + ms3 * ampsmooth2;
	float rms = (sqrtf(ms4)) * ampsns;

	float frl = filterl->getrealfreq(freq + lfol + rms);
	float frr = filterr->getrealfreq(freq + lfor + rms);

	filterl->setfreq_and_q(frl, q);
	filterr->setfreq_and_q(frr, q);

	filterl->filterout(efxoutl);
	filterr->filterout(efxoutr);

	//panning
	for (i = 0; i < iPERIOD; i++) {
		efxoutl[i] *= panning;
		efxoutr[i] *= (1.0f - panning);
	};

}
;

/*
 * Cleanup the effect
 */
void DynamicFilter::cleanup() {
	reinitfilter();
	ms1 = 0.0;
	ms2 = 0.0;
	ms3 = 0.0;
	ms4 = 0.0;
}
;

/*
 * Parameter control
 */
void DynamicFilter::reinitfilter() {
	if (filterl != NULL)
		delete (filterl);
	if (filterr != NULL)
		delete (filterr);
	filterl = new Filter(filterpars);
	filterr = new Filter(filterpars);
}
;

void DynamicFilter::setPreset(int npreset) {
	YroEffectPlugin::setPreset(npreset);

	filterpars->defaults();
	switch (npreset) {
	case 0:
		filterpars->Pcategory = 0;
		filterpars->Ptype = 2;
		filterpars->Pfreq = 45;
		filterpars->Pq = 64;
		filterpars->Pstages = 1;
		filterpars->Pgain = 64;
		break;
	case 1:
		filterpars->Pcategory = 2;
		filterpars->Ptype = 0;
		filterpars->Pfreq = 72;
		filterpars->Pq = 64;
		filterpars->Pstages = 0;
		filterpars->Pgain = 64;
		break;
	case 2:
		filterpars->Pcategory = 0;
		filterpars->Ptype = 4;
		filterpars->Pfreq = 64;
		filterpars->Pq = 64;
		filterpars->Pstages = 2;
		filterpars->Pgain = 64;
		break;
	case 3:
		filterpars->Pcategory = 1;
		filterpars->Ptype = 0;
		filterpars->Pfreq = 50;
		filterpars->Pq = 70;
		filterpars->Pstages = 1;
		filterpars->Pgain = 64;

		filterpars->Psequencesize = 2;
		// "I"
		filterpars->Pvowels[0].formants[0].freq = 34;
		filterpars->Pvowels[0].formants[0].amp = 127;
		filterpars->Pvowels[0].formants[0].q = 64;
		filterpars->Pvowels[0].formants[1].freq = 99;
		filterpars->Pvowels[0].formants[1].amp = 122;
		filterpars->Pvowels[0].formants[1].q = 64;
		filterpars->Pvowels[0].formants[2].freq = 108;
		filterpars->Pvowels[0].formants[2].amp = 112;
		filterpars->Pvowels[0].formants[2].q = 64;
		// "A"
		filterpars->Pvowels[1].formants[0].freq = 61;
		filterpars->Pvowels[1].formants[0].amp = 127;
		filterpars->Pvowels[1].formants[0].q = 64;
		filterpars->Pvowels[1].formants[1].freq = 71;
		filterpars->Pvowels[1].formants[1].amp = 121;
		filterpars->Pvowels[1].formants[1].q = 64;
		filterpars->Pvowels[1].formants[2].freq = 99;
		filterpars->Pvowels[1].formants[2].amp = 117;
		filterpars->Pvowels[1].formants[2].q = 64;
		break;
	case 4:
		filterpars->Pcategory = 1;
		filterpars->Ptype = 0;
		filterpars->Pfreq = 64;
		filterpars->Pq = 70;
		filterpars->Pstages = 1;
		filterpars->Pgain = 64;

		filterpars->Psequencesize = 2;
		filterpars->Pnumformants = 2;
		filterpars->Pvowelclearness = 0;

		filterpars->Pvowels[0].formants[0].freq = 70;
		filterpars->Pvowels[0].formants[0].amp = 127;
		filterpars->Pvowels[0].formants[0].q = 64;
		filterpars->Pvowels[0].formants[1].freq = 80;
		filterpars->Pvowels[0].formants[1].amp = 122;
		filterpars->Pvowels[0].formants[1].q = 64;

		filterpars->Pvowels[1].formants[0].freq = 20;
		filterpars->Pvowels[1].formants[0].amp = 127;
		filterpars->Pvowels[1].formants[0].q = 64;
		filterpars->Pvowels[1].formants[1].freq = 100;
		filterpars->Pvowels[1].formants[1].amp = 121;
		filterpars->Pvowels[1].formants[1].q = 64;
		break;
	};

//          for (int i=0;i<5;i++){
//              printf("freq=%d  amp=%d  q=%d\n",filterpars->Pvowels[0].formants[i].freq,filterpars->Pvowels[0].formants[i].amp,filterpars->Pvowels[0].formants[i].q);
//          };

	reinitfilter();
}
;

void DynamicFilter::setVolume(int Pvolume) {
	this->Pvolume = Pvolume;
	outvolume = (float) Pvolume / 127.0f;

}
void DynamicFilter::setPanning(int Ppanning) {
	this->Ppanning = Ppanning;
	panning = ((float) Ppanning + .5f) / 127.0f;
}
void DynamicFilter::setLfoPfreq(int value) {
	lfo.setPfreq(value);
}
void DynamicFilter::setLfoPrandomness(int value) {
	lfo.setPrandomness(value);
}
void DynamicFilter::setLfoPlfOtype(int value) {
	lfo.setPlfOtype(value);
}
void DynamicFilter::setLfoPstereo(int value) {
	lfo.setPstereo(value);
}
void DynamicFilter::setDepth(int Pdepth) {
	this->Pdepth = Pdepth;
	depth = powf(((float) Pdepth / 127.0f), 2.0f);
}
void DynamicFilter::setAmpsns(int Pampsns) {
	ampsns = powf((float) Pampsns / 127.0f, 2.5f) * 10.0f;
	if (Pampsnsinv != 0)
		ampsns = -ampsns;
	ampsmooth = expf((float) -Pampsmooth / 127.0f * 10.0f) * 0.99f;
	this->Pampsns = Pampsns;
}
void DynamicFilter::setAmpsnsinv(int value) {
	Pampsnsinv = value;
	setAmpsns(Pampsns);
}
void DynamicFilter::setAmpsmooth(int value) {
	Pampsmooth = value;
	setAmpsns(Pampsns);
}

int DynamicFilter::getVolume() {
	return Pvolume;
}
int DynamicFilter::getPanning() {
	return Ppanning;
}
int DynamicFilter::getLfoPfreq() {
	return lfo.getPfreq();
}
int DynamicFilter::getLfoPrandomness() {
	return lfo.getPrandomness();
}
int DynamicFilter::getLfoPlfOtype() {
	return lfo.getPlfOtype();
}
int DynamicFilter::getLfoPstereo() {
	return lfo.getPstereo();
}
int DynamicFilter::getDepth() {
	return Pdepth;
}
int DynamicFilter::getAmpsns() {
	return Pampsns;
}
int DynamicFilter::getAmpsnsinv() {
	return Pampsnsinv;
}
int DynamicFilter::getAmpsmooth() {
	return Pampsmooth;
}
/**
 * toXml member
 */
const char *DynamicFilter::toXml() {
	char _buffer[256];
	char _formatd[] = { "<attribute name=\"%s\" value=\"%d\" />" };
	char _formatf[] = { "<attribute name=\"%s\" value=\"%9.40f\" />" };
	strcpy(_toXml, "<attributes>");
	sprintf(_buffer, _formatd, "Pampsmooth", Pampsmooth);
	strcat(_toXml, _buffer);
	sprintf(_buffer, _formatd, "Pampsns", Pampsns);
	strcat(_toXml, _buffer);
	sprintf(_buffer, _formatd, "Pampsnsinv", Pampsnsinv);
	strcat(_toXml, _buffer);
	sprintf(_buffer, _formatd, "Pdepth", Pdepth);
	strcat(_toXml, _buffer);
	sprintf(_buffer, _formatd, "Ppanning", Ppanning);
	strcat(_toXml, _buffer);
	sprintf(_buffer, _formatd, "Pvolume", Pvolume);
	strcat(_toXml, _buffer);
	sprintf(_buffer, _formatf, "ms1", ms1);
	strcat(_toXml, _buffer);
	sprintf(_buffer, _formatf, "ms2", ms2);
	strcat(_toXml, _buffer);
	sprintf(_buffer, _formatf, "ms3", ms3);
	strcat(_toXml, _buffer);
	sprintf(_buffer, _formatf, "ms4", ms4);
	strcat(_toXml, _buffer);
	sprintf(_buffer, _formatf, "outvolume", outvolume);
	strcat(_toXml, _buffer);
	sprintf(_buffer, _formatf, "panning", panning);
	strcat(_toXml, _buffer);
	sprintf(_buffer, _formatf, "depth", depth);
	strcat(_toXml, _buffer);
	sprintf(_buffer, _formatf, "ampsns", ampsns);
	strcat(_toXml, _buffer);
	sprintf(_buffer, _formatf, "ampsmooth", ampsmooth);
	strcat(_toXml, _buffer);
	strcat(_toXml, "</attributes>");
	return _toXml;
}
