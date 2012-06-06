/*
 
 Vocoder.C - Vocoder effect
 Author: Ryam Billing & Josep Andreu

 Adapted effect structure of ZynAddSubFX - a software synthesizer
 Author: Nasca Octavian Paul

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

#include <plugins/effect/Vocoder.h>
using namespace std;

Vocoder::Vocoder(float *auxresampled_, int bands, int DS, int uq, int dq) :
		YroEffectPlugin("Vocoder", "Vocoder 1: 0, 64, 10, 70, 70, 40, 0;"
				"Vocoder 2: 0, 64, 14, 80, 70, 40, 32;"
				"Vocoder 3: 0, 64, 20, 90, 70, 40, 64;"
				"Vocoder 4: 0, 64, 30, 100, 70, 40, 127;") {

	adjust(DS);

	VOC_BANDS = bands;

	auxresampled = auxresampled_;
	//default values
	Pvolume = 50;
	Plevel = 0;
	Pinput = 0;
	Ppanning = 64;
	Plrcross = 100;

	filterbank = (fbank *) malloc(sizeof(fbank) * VOC_BANDS);
	tmpl = (float *) malloc(sizeof(float) * nPERIOD);
	tmpr = (float *) malloc(sizeof(float) * nPERIOD);
	tsmpsl = (float *) malloc(sizeof(float) * nPERIOD);
	tsmpsr = (float *) malloc(sizeof(float) * nPERIOD);
	tmpaux = (float *) malloc(sizeof(float) * nPERIOD);

	Pmuffle = 10;
	float tmp = 0.01f; //10 ms decay time on peak detectors
	alpha = ncSAMPLE_RATE / (ncSAMPLE_RATE + tmp);
	beta = 1.0f - alpha;
	prls = beta;
	gate = 0.005f;

	tmp = 0.05f; //50 ms att/rel on compressor
	calpha = ncSAMPLE_RATE / (ncSAMPLE_RATE + tmp);
	cbeta = 1.0f - calpha;
	cthresh = 0.25f;
	cpthresh = cthresh; //dynamic threshold
	cratio = 0.25f;

	float center;
	float qq;

	A_Resample = new Resample(dq);
	U_Resample = new Resample(dq);
	D_Resample = new Resample(uq);

	for (int i = 0; i < VOC_BANDS; i++) {
		center = (float) i * 20000.0f / ((float) VOC_BANDS);
		qq = 60.0f;

		filterbank[i].l = new AnalogFilter(4, center, qq, 0);
		filterbank[i].l->setSR(nSAMPLE_RATE);
		filterbank[i].r = new AnalogFilter(4, center, qq, 0);
		filterbank[i].r->setSR(nSAMPLE_RATE);
		filterbank[i].aux = new AnalogFilter(4, center, qq, 0);
		filterbank[i].aux->setSR(nSAMPLE_RATE);
	};

	vlp = new AnalogFilter(2, 4000.0f, 1.0f, 1);
	vhp = new AnalogFilter(3, 200.0f, 0.707f, 1);

	vlp->setSR(nSAMPLE_RATE);
	vhp->setSR(nSAMPLE_RATE);

	setbands(VOC_BANDS, 200.0f, 4000.0f);
	setPreset(0);
}

Vocoder::~Vocoder() {
}

/*
 * Cleanup the effect
 */
void Vocoder::cleanup() {
	for (int k = 0; k < VOC_BANDS; k++) {
		filterbank[k].l->cleanup();
		filterbank[k].r->cleanup();
		filterbank[k].aux->cleanup();
		filterbank[k].speak = 0.0f;
		filterbank[k].gain = 0.0f;
		filterbank[k].oldgain = 0.0f;

	}
	vhp->cleanup();
	vlp->cleanup();

	compeak = compg = compenv = oldcompenv = 0.0f;

}

void Vocoder::adjust(int DS) {

	DS_state = DS;

	switch (DS) {

	case 0:
		nPERIOD = iPERIOD;
		nSAMPLE_RATE = iSAMPLE_RATE;
		nfSAMPLE_RATE = fSAMPLE_RATE;
		break;

	case 1:
		nPERIOD = lrintf(fPERIOD * 96000.0f / fSAMPLE_RATE);
		nSAMPLE_RATE = 96000;
		nfSAMPLE_RATE = 96000.0f;
		break;

	case 2:
		nPERIOD = lrintf(fPERIOD * 48000.0f / fSAMPLE_RATE);
		nSAMPLE_RATE = 48000;
		nfSAMPLE_RATE = 48000.0f;
		break;

	case 3:
		nPERIOD = lrintf(fPERIOD * 44100.0f / fSAMPLE_RATE);
		nSAMPLE_RATE = 44100;
		nfSAMPLE_RATE = 44100.0f;
		break;

	case 4:
		nPERIOD = lrintf(fPERIOD * 32000.0f / fSAMPLE_RATE);
		nSAMPLE_RATE = 32000;
		nfSAMPLE_RATE = 32000.0f;
		break;

	case 5:
		nPERIOD = lrintf(fPERIOD * 22050.0f / fSAMPLE_RATE);
		nSAMPLE_RATE = 22050;
		nfSAMPLE_RATE = 22050.0f;
		break;

	case 6:
		nPERIOD = lrintf(fPERIOD * 16000.0f / fSAMPLE_RATE);
		nSAMPLE_RATE = 16000;
		nfSAMPLE_RATE = 16000.0f;
		break;

	case 7:
		nPERIOD = lrintf(fPERIOD * 12000.0f / fSAMPLE_RATE);
		nSAMPLE_RATE = 12000;
		nfSAMPLE_RATE = 12000.0f;
		break;

	case 8:
		nPERIOD = lrintf(fPERIOD * 8000.0f / fSAMPLE_RATE);
		nSAMPLE_RATE = 8000;
		nfSAMPLE_RATE = 8000.0f;
		break;

	case 9:
		nPERIOD = lrintf(fPERIOD * 4000.0f / fSAMPLE_RATE);
		nSAMPLE_RATE = 4000;
		nfSAMPLE_RATE = 4000.0f;
		break;

	}

	ncSAMPLE_RATE = 1.0f / nfSAMPLE_RATE;

	u_up = (double) nPERIOD / (double) iPERIOD;
	u_down = (double) iPERIOD / (double) nPERIOD;

}

/*
 * Effect output
 */
void Vocoder::render(jack_nframes_t nframes, float * smpsl, float * smpsr) {
	int i, j;

	float tempgain;
	float maxgain = 0.0f;
	float auxtemp, tmpgain;

	if (DS_state != 0) {
		A_Resample->mono_out(auxresampled, tmpaux, iPERIOD, u_up, nPERIOD);
	} else
		memcpy(tmpaux, auxresampled, sizeof(float) * nPERIOD);

	for (i = 0; i < nPERIOD; i++) //apply compression to auxresampled
			{
		auxtemp = input * tmpaux[i];
		if (fabs(auxtemp > compeak))
			compeak = fabs(auxtemp); //First do peak detection on the signal
		compeak *= prls;
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

		tmpaux[i] = auxtemp * tmpgain;

		tmpaux[i] = vlp->filterout_s(tmpaux[i]);
		tmpaux[i] = vhp->filterout_s(tmpaux[i]);

	};

	//End compression

	auxtemp = 0.0f;

	if (DS_state != 0) {
		/**
		 * TODO on to make it ?
		 U_Resample->render(nframes, smpsl, smpsr,tsmpsl,tsmpsr,PERIOD,u_up);
		 */
	} else {
		memcpy(tsmpsl, smpsl, sizeof(float) * nPERIOD);
		memcpy(tsmpsr, smpsr, sizeof(float) * nPERIOD);
	}

	memset(tmpl, 0, sizeof(float) * nPERIOD);
	memset(tmpr, 0, sizeof(float) * nPERIOD);

	for (j = 0; j < VOC_BANDS; j++) {

		for (i = 0; i < nPERIOD; i++) {
			auxtemp = tmpaux[i];

			if (filterbank[j].speak < gate)
				filterbank[j].speak = 0.0f; //gate
			if (auxtemp > maxgain)
				maxgain = auxtemp; //vu meter level.

			auxtemp = filterbank[j].aux->filterout_s(auxtemp);
			if (fabs(auxtemp) > filterbank[j].speak)
				filterbank[j].speak = fabs(auxtemp); //Leaky Peak detector

			filterbank[j].speak *= prls;

			filterbank[j].gain = beta * filterbank[j].oldgain
					+ alpha * filterbank[j].speak;
			filterbank[j].oldgain = filterbank[j].gain;

			tempgain = (1.0f - ringworm) * filterbank[j].oldgain
					+ ringworm * auxtemp;

			tmpl[i] += filterbank[j].l->filterout_s(tsmpsl[i]) * tempgain;
			tmpr[i] += filterbank[j].r->filterout_s(tsmpsr[i]) * tempgain;

		};

	};

	for (i = 0; i < nPERIOD; i++) {
		tmpl[i] *= lpanning * level;
		tmpr[i] *= rpanning * level;
	};

	if (DS_state != 0) {
		D_Resample->out(tmpl, tmpr, efxoutl, efxoutr, nPERIOD, u_down);
	} else {
		memcpy(efxoutl, tmpl, sizeof(float) * nPERIOD);
		memcpy(efxoutr, tmpr, sizeof(float) * nPERIOD);
	}

	vulevel = (float) CLAMP(rap2dB(maxgain), -48.0, 15.0);

}
;

void Vocoder::setbands(int numbands, float startfreq, float endfreq) {
	float start = startfreq; //useful variables
	float endband = endfreq;
	float fnumbands = (float) numbands;
	float output[VOC_BANDS + 1];
	int k;

	//calculate intermediate values
	float pwer = logf(endband / start) / log(2.0f);

	for (k = 0; k <= VOC_BANDS; k++)
		output[k] = start * powf(2.0f, ((float) k) * pwer / fnumbands);
	for (k = 0; k < VOC_BANDS; k++) {
		filterbank[k].sfreq = output[k] + (output[k + 1] - output[k]) * 0.5f;
		filterbank[k].sq = filterbank[k].sfreq / (output[k + 1] - output[k]);

		filterbank[k].l->setfreq_and_q(filterbank[k].sfreq, filterbank[k].sq);
		filterbank[k].r->setfreq_and_q(filterbank[k].sfreq, filterbank[k].sq);
		filterbank[k].aux->setfreq_and_q(filterbank[k].sfreq, filterbank[k].sq);
	}
	cleanup();

}

/*
 * Parameter control
 */
void Vocoder::setVolume(int Pvolume) {
	this->Pvolume = Pvolume;
	outvolume = (float) Pvolume / 127.0f;
	if (Pvolume == 0)
		cleanup();

}
;

void Vocoder::setPanning(int Ppanning) {
	this->Ppanning = Ppanning;
	lpanning = ((float) Ppanning + 0.5f) / 127.0f;
	rpanning = 1.0f - lpanning;
}

void Vocoder::init_filters() {
	float ff, qq;

	for (int ii = 0; ii < VOC_BANDS; ii++) {
		ff = filterbank[ii].sfreq;
		qq = filterbank[ii].sq;
		filterbank[ii].l->setfreq_and_q(ff, qq);
		filterbank[ii].r->setfreq_and_q(ff, qq);
		filterbank[ii].aux->setfreq_and_q(ff, qq);
	};

}

void Vocoder::adjustq(float q) {

	for (int ii = 0; ii < VOC_BANDS; ii++) {
		filterbank[ii].l->setq(q);
		filterbank[ii].r->setq(q);
		filterbank[ii].aux->setq(q);
	};

}

void Vocoder::setMuffle(int value) {
	Pmuffle = value;
	float tmp = (float) Pmuffle;
	tmp *= 0.0001f + tmp / 64000;
	alpha = ncSAMPLE_RATE / (ncSAMPLE_RATE + tmp);
	beta = 1.0f - alpha;
}
void Vocoder::setQq(int value) {
	Pqq = value;
	float tmp = (float) value;
	adjustq(tmp);
}
void Vocoder::setInput(int value) {
	Pinput = value;
	input = dB2rap (75.0f * (float)Pinput / 127.0f - 40.0f);
}
void Vocoder::setLevel(int value) {
	Plevel = value;
	level = dB2rap (60.0f * (float)Plevel / 127.0f - 40.0f);
}
void Vocoder::setRing(int value) {
	Pring = value;
	ringworm = (float) Pring / 127.0f;
}

int Vocoder::getVolume() {
	return Pvolume;
}
int Vocoder::getPanning() {
	return Ppanning;
}
int Vocoder::getMuffle() {
	return Pmuffle;
}
int Vocoder::getQq() {
	return Pqq;
}
int Vocoder::getInput() {
	return Pinput;
}
int Vocoder::getLevel() {
	return Plevel;
}
int Vocoder::getRing() {
	return Pring;
}

/**
 * toXml member
*/
const char *Vocoder::toXml() {
        char _buffer[256];
        char _formatd[] = {"<attribute name=\"%s\" value=\"%d\" />"};
        char _formatf[] = {"<attribute name=\"%s\" value=\"%9.40f\" />"};
        strcpy(_toXml,"<attributes>");
        sprintf(_buffer,_formatd,"DS_state",DS_state);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatd,"nPERIOD",nPERIOD);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatd,"nSAMPLE_RATE",nSAMPLE_RATE);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatd,"Pband",Pband);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatd,"Pinput",Pinput);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatd,"Plevel",Plevel);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatd,"Plrcross",Plrcross);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatd,"Pmuffle",Pmuffle);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatd,"Ppanning",Ppanning);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatd,"Pqq",Pqq);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatd,"Pring",Pring);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatd,"Pvolume",Pvolume);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatd,"VOC_BANDS",VOC_BANDS);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatf,"alpha",alpha);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatf,"beta",beta);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatf,"prls",prls);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatf,"gate",gate);
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
        sprintf(_buffer,_formatf,"cratio",cratio);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatf,"lpanning",lpanning);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatf,"rpanning",rpanning);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatf,"input",input);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatf,"level",level);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatf,"ncSAMPLE_RATE",ncSAMPLE_RATE);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatf,"nfSAMPLE_RATE",nfSAMPLE_RATE);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatf,"outvolume",outvolume);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatf,"ringworm",ringworm);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatf,"vulevel",vulevel);
        strcat(_toXml,_buffer);
        strcat(_toXml,"</attributes>");
        return _toXml;
}
