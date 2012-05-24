/*
 
 Echotron.C - Echotron effect
 Author: Ryan Billing & Josep Andreu

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

#include <plugins/effect/Echotron.h>
using namespace std;

Echotron::Echotron() :
		YroEffectPlugin("Echotron",
				"Summer: 64,45,34,4,0,76,3,41,0,96,-13,64,1,1,1,1;"
						"Ambience: 96,64,16,4,0,180,50,64,1,96,-4,64,1,0,0,0;"
						"Arranjer: 64,64,10,4,0,400,32,64,1,96,-8,64,1,0,0,0;"
						"Suction: 0,47,28,8,0,92,0,64,3,32,0,64,1,1,1,1;"
						"SucFlange: 64,36,93,8,0,81,0,64,3,32,0,64,1,0,1,1;") {
	initparams = 0;
	//default values
	Pvolume = 50;
	Ppanning = 64;
	Plrcross = 100;
	Phidamp = 60;
	Filenum = 0;
	Plength = 10;
	Puser = 0;
	fb = 0.0f;
	lfeedback = 0.0f;
	rfeedback = 0.0f;
	subdiv_dmod = 1.0f;
	subdiv_fmod = 1.0f;
	f_qmode = 0;

	maxx_size = (iSAMPLE_RATE * 6); //6 Seconds delay time

	lxn = (float *) malloc(sizeof(float) * (1 + maxx_size));
	rxn = (float *) malloc(sizeof(float) * (1 + maxx_size));

	offset = 0;

	lpfl = new AnalogFilter(0, 800, 1, 0);
	;
	lpfr = new AnalogFilter(0, 800, 1, 0);
	;

	float center, qq;
	for (int i = 0; i < ECHOTRON_MAXFILTERS; i++) {
		center = 500;
		qq = 1.0f;
		filterbank[i].sfreq = center;
		filterbank[i].sq = qq;
		filterbank[i].sLP = 0.25f;
		filterbank[i].sBP = -1.0f;
		filterbank[i].sHP = 0.5f;
		filterbank[i].sStg = 1.0f;
		filterbank[i].l = new RBFilter(0, center, qq, 0);
		filterbank[i].r = new RBFilter(0, center, qq, 0);

		filterbank[i].l->setmix(1, filterbank[i].sLP, filterbank[i].sBP,
				filterbank[i].sHP);
		filterbank[i].r->setmix(1, filterbank[i].sLP, filterbank[i].sBP,
				filterbank[i].sHP);
	};

	setPreset(0);
	cleanup();
}

Echotron::~Echotron() {
}

/*
 * Cleanup the effect
 */
void Echotron::cleanup() {
	memset(lxn, 0, sizeof(float) * maxx_size);
	memset(rxn, 0, sizeof(float) * maxx_size);

	lpfl->cleanup();
	lpfr->cleanup();

}
;

/*
 * Effect output
 */
void Echotron::render(jack_nframes_t nframes, float * smpsl, float * smpsr) {
	int i, j, k, rxindex, lxindex;
	float l, r, lyn, ryn;
	int length = Plength;

	if ((Pmoddly) || (Pmodfilts))
		modulate_delay();
	else
		interpl = interpr = 0;

	float tmpmodl = oldldmod;
	float tmpmodr = oldrdmod;
	int intmodl, intmodr;

	for (i = 0; i < iPERIOD; i++) {
		tmpmodl += interpl;
		tmpmodr += interpr;

		intmodl = lrintf(tmpmodl);
		intmodr = lrintf(tmpmodr);

		l = lpfl->filterout_s(smpsl[i] + lfeedback); //High Freq damping
		r = lpfr->filterout_s(smpsr[i] + rfeedback);

		lxn[offset] = l;
		rxn[offset] = r;

		//Convolve
		lyn = 0.0f;
		ryn = 0.0f;

		if (Pfilters) {
			j = 0;
			for (k = 0; k < length; k++) {
				lxindex = offset + ltime[k] + intmodl;
				rxindex = offset + rtime[k] + intmodr;
				if (lxindex >= maxx_size)
					lxindex -= maxx_size;
				if (rxindex >= maxx_size)
					rxindex -= maxx_size;
				if ((iStages[k] >= 0) && (j < ECHOTRON_MAXFILTERS)) {
					lyn += filterbank[j].l->filterout_s(lxn[lxindex])
							* ldata[k]; //filter each tap specified
					ryn += filterbank[j].r->filterout_s(rxn[rxindex])
							* rdata[k];
					j++;
				} else {
					lyn += lxn[lxindex] * ldata[k];
					ryn += rxn[rxindex] * rdata[k];
				}

			}

		} else {
			for (k = 0; k < length; k++) {

				lxindex = offset + ltime[k] + intmodl;
				rxindex = offset + rtime[k] + intmodr;
				if (lxindex >= maxx_size)
					lxindex -= maxx_size;
				if (rxindex >= maxx_size)
					rxindex -= maxx_size;
				lyn += lxn[lxindex] * ldata[k];
				ryn += rxn[rxindex] * rdata[k];
			}

		}

		lfeedback = (lrcross * ryn + ilrcross * lyn) * lpanning;
		rfeedback = (lrcross * lyn + ilrcross * ryn) * rpanning;
		efxoutl[i] = lfeedback;
		efxoutr[i] = rfeedback;
		lfeedback *= fb;
		rfeedback *= fb;

		offset--;
		if (offset < 0)
			offset = maxx_size;

	};

	if (initparams)
		init_params();

}
;

/*
 * Parameter control
 */
void Echotron::setVolume(int Pvolume) {
	this->Pvolume = Pvolume;
	outvolume = (float) Pvolume / 127.0f;
	if (Pvolume == 0)
		cleanup();

}
;

void Echotron::setPanning(int value) {
	Ppanning = value;
	rpanning = ((float) Ppanning) / 64.0f;
	lpanning = 2.0f - rpanning;
	lpanning = 10.0f * powf(lpanning, 4);
	rpanning = 10.0f * powf(rpanning, 4);
	lpanning = 1.0f - 1.0f / (lpanning + 1.0f);
	rpanning = 1.0f - 1.0f / (rpanning + 1.0f);
	lpanning *= 1.1f;
	rpanning *= 1.1f;
	if (lpanning > 1.0f)
		lpanning = 1.0f;
	if (rpanning > 1.0f)
		rpanning = 1.0f;
}
;

void Echotron::setFile(int value) {
	float tPan = 0.0f;
	float tTime = 0.0f;
	float tLevel = 0.0f;
	float tLP = 0.0f;
	float tBP = 0.0f;
	float tHP = 0.0f;
	float tFreq = 20.0f;
	float tQ = 1.0f;
	int tiStages = 0;

	FILE *fs;

	char wbuf[128];

	if (!Puser) {
		Filenum = value;
		memset(Filename, 0, sizeof(Filename));
		/**
		 * TODO DATADIR ....
		 */
		sprintf(Filename, "%s/%d.dly", "DATADIR", Filenum + 1);
	}

	if ((fs = fopen(Filename, "r")) == NULL) {
		loaddefault();
	}

	while (fgets(wbuf, sizeof wbuf, fs) != NULL) {
		//fgets(wbuf,sizeof wbuf,fs);
		if (wbuf[0] != '#')
			break;
		memset(wbuf, 0, sizeof(wbuf));
	}

	sscanf(wbuf, "%f\t%f\t%d", &subdiv_fmod, &subdiv_dmod, &f_qmode); //Second line has tempo subdivision
	//printf("subdivs:\t%f\t%f\n",subdiv_fmod,subdiv_dmod);

	int count = 0;
	memset(iStages, 0, sizeof(iStages));

	while ((fgets(wbuf, sizeof wbuf, fs) != NULL) && (count < ECHOTRON_F_SIZE)) {
		if (wbuf[0] == 10)
			break; // Check Carriage Return
		if (wbuf[0] == '#')
			continue;
		sscanf(wbuf, "%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%d", &tPan, &tTime,
				&tLevel, &tLP, &tBP, &tHP, &tFreq, &tQ, &tiStages);
		//printf("params:\n%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%d\n",tPan, tTime, tLevel,
		//tLP,  tBP,  tHP,  tFreq,  tQ,  tiStages);
		if ((tPan < -1.0f) || (tPan > 1.0f)) {
			helper->setErrorNumber(5);
			break;
		} else
			fPan[count] = tPan;

		if ((tTime < -6.0) || (tTime > 6.0f)) {
			helper->setErrorNumber(6);
			break;
		} else
			fTime[count] = fabs(tTime);

		if ((tLevel < -10.0f) || (tLevel > 10.0f)) {
			helper->setErrorNumber(7);
			break;
		} else
			fLevel[count] = tLevel;

		if ((tLP < -2.0f) || (tLP > 2.0f)) {
			helper->setErrorNumber(8);
			break;
		} else
			fLP[count] = tLP;

		if ((tBP < -2.0f) || (tBP > 2.0f)) {
			helper->setErrorNumber(9);
			break;
		} else
			fBP[count] = tBP;

		if ((tHP < -2.0f) || (tHP > 2.0f)) {
			helper->setErrorNumber(10);
			break;
		} else
			fHP[count] = tHP;

		if ((tFreq < 20.0f) || (tFreq > 26000.0f)) {
			helper->setErrorNumber(11);
			break;
		} else
			fFreq[count] = tFreq;

		if ((tQ < 0.0) || (tQ > 300.0f)) {
			helper->setErrorNumber(12);
			break;
		} else
			fQ[count] = tQ;

		if ((tiStages < 0) || (tiStages > MAX_FILTER_STAGES)) {
			helper->setErrorNumber(13);
			break;
		} else
			iStages[count] = tiStages - 1; //check in main loop if <0, then skip filter

		memset(wbuf, 0, sizeof(wbuf));
		count++;
	}
	fclose(fs);

	if (!Pchange)
		Plength = count;
	cleanup();
	init_params();
}

void Echotron::loaddefault() {
	Plength = 1;
	fPan[0] = 0.0f; //
	fTime[0] = 1.0f; //default 1 measure delay
	fLevel[0] = 0.7f;
	fLP[0] = 1.0f;
	fBP[0] = -1.0f;
	fHP[0] = 1.0f;
	fFreq[0] = 800.0f;
	;
	fQ[0] = 2.0f;
	iStages[0] = 1;
	subdiv_dmod = 1.0f;
	subdiv_fmod = 1.0f;
	init_params();
}

void Echotron::init_params() {

	float hSR = fSAMPLE_RATE * 0.5f;
	float tmp_time;
	float tpanl, tpanr;
	float tmptempo;
	int tfcnt = 0;

	initparams = 0;
	depth = ((float) (Pdepth - 64)) / 64.0f;
	dlyrange = 0.008 * powf(2.0f, 4.5f * depth);
	width = ((float) Pwidth) / 127.0f;

	tmptempo = (float) Ptempo;
	lfo.setPfreq((int) lrintf(subdiv_fmod * tmptempo));
	dlfo.setPfreq((int) lrintf(subdiv_dmod * tmptempo));

	for (int i = 0; i < Plength; i++) {
		tmp_time = lrintf(fTime[i] * tempo_coeff * fSAMPLE_RATE);
		if (tmp_time < maxx_size)
			rtime[i] = tmp_time;
		else
			rtime[i] = maxx_size;

		ltime[i] = rtime[i];

		if (fPan[i] >= 0.0f) {
			tpanr = 1.0;
			tpanl = 1.0f - fPan[i];
		} else {
			tpanl = 1.0;
			tpanr = 1.0f + fPan[i];
		}

		ldata[i] = fLevel[i] * tpanl;
		rdata[i] = fLevel[i] * tpanr;

		if ((tfcnt < ECHOTRON_MAXFILTERS) && (iStages[i] >= 0)) {
			int Freq = fFreq[i] * powf(2.0f, depth * 4.5f);
			if (Freq < 20.0)
				Freq = 20.0f;
			if (Freq > hSR)
				Freq = hSR;
			filterbank[tfcnt].l->setfreq_and_q(Freq, fQ[i]);
			filterbank[tfcnt].r->setfreq_and_q(Freq, fQ[i]);
			filterbank[tfcnt].l->setstages(iStages[i]);
			filterbank[tfcnt].r->setstages(iStages[i]);
			filterbank[tfcnt].l->setmix(1, fLP[i], fBP[i], fHP[i]);
			filterbank[tfcnt].r->setmix(1, fLP[i], fBP[i], fHP[i]);
			filterbank[tfcnt].l->setmode(f_qmode);
			filterbank[tfcnt].r->setmode(f_qmode);
			tfcnt++;
		}
	}

}
;

void Echotron::modulate_delay() {

	float lfmod, rfmod, lfol, lfor, dlfol, dlfor;
	float fperiod = 1.0f / fPERIOD;

	lfo.render(1, &lfol, &lfor);
	dlfo.render(1, &dlfol, &dlfor);
	if (Pmodfilts) {
		lfmod = powf(2.0f, (lfol * width + 0.25f + depth) * 4.5f);
		rfmod = powf(2.0f, (lfor * width + 0.25f + depth) * 4.5f);
		for (int i = 0; i < ECHOTRON_MAXFILTERS; i++) {

			filterbank[i].l->setfreq(lfmod * fFreq[i]);
			filterbank[i].r->setfreq(rfmod * fFreq[i]);

		}

	}

	if (Pmoddly) {
		oldldmod = ldmod;
		oldrdmod = rdmod;
		ldmod = width * dlfol;
		rdmod = width * dlfor;

		ldmod = lrintf(dlyrange * tempo_coeff * fSAMPLE_RATE * ldmod);
		rdmod = lrintf(dlyrange * tempo_coeff * fSAMPLE_RATE * rdmod);

		interpl = (ldmod - oldldmod) * fperiod;
		interpr = (rdmod - oldrdmod) * fperiod;
	} else {
		oldldmod = 0.0f;
		oldrdmod = 0.0f;
		ldmod = 0.0f;
		rdmod = 0.0f;
		interpl = 0.0f;
		interpr = 0.0f;
	}

}
;

void Echotron::setHidamp(int Phidamp) {
	this->Phidamp = Phidamp;
	hidamp = 1.0f - (float) Phidamp / 127.1f;
	float fr = 20.0f * powf(2.0, hidamp * 10.0f);
	lpfl->setfreq(fr);
	lpfr->setfreq(fr);
}
;

void Echotron::setFb(int value) {
	Pfb = value;
	fb = (float) value / 64.0f;
}

void Echotron::setDepth(int value) {
	Pdepth = value;
	initparams = 1;
}
void Echotron::setWidth(int value) {
	Pwidth = value;
	initparams = 1;
}
void Echotron::setLength(int value) {
	Plength = value;
	if (Plength > 127)
		Plength = 127;
	initparams = 1;
}
void Echotron::setUser(int value) {
	Puser = value;
}
void Echotron::setTempo(int value) {
	Ptempo = value;
	float tmptempo = (float) Ptempo;
	tempo_coeff = 60.0f / tmptempo;
	lfo.setPfreq((int) lrintf(subdiv_fmod * tmptempo));
	dlfo.setPfreq((int) lrintf(subdiv_dmod * tmptempo));
	initparams = 1;
}
void Echotron::setLrcross(int value) {
	Plrcross = value;
	lrcross = ((float) (Plrcross) - 64) / 64.0;
	ilrcross = 1.0f - abs(lrcross);
}
void Echotron::setLfoStereo(int value) {
	lfo.setPstereo(value);
	dlfo.setPstereo(value);
}
void Echotron::setModdly(int value) {
	Pmoddly = value; //delay modulation on/off
}
void Echotron::setModfilts(int value) {
	Pmodfilts = value; //filter modulation on/off
	if (!Pmodfilts)
		initparams = 1;
}
void Echotron::setLfoType(int value) {
//LFO Type
	lfo.setPlfOtype(value);
	dlfo.setPlfOtype(value);
}
void Echotron::setFilters(int value) {
	Pfilters = value; //Pfilters
}

int  Echotron::getVolume() {return Pvolume;}
int  Echotron::getDepth() {return Pdepth;}
int  Echotron::getWidth() {return Pwidth;}
int  Echotron::getLength() {return Plength;}
int  Echotron::getUser() {return Puser;}
int  Echotron::getTempo() {return Ptempo;}
int  Echotron::getHidamp() {return Phidamp;}
int  Echotron::getLrcross() {return Plrcross;}
int  Echotron::getFile() {return 0; /** TODO PFILE */}
int  Echotron::getLfoStereo() {return lfo.getPstereo();}
int  Echotron::getFb() {return Pfb;}
int  Echotron::getPanning() {return Ppanning;}
int  Echotron::getModdly() {return Pmoddly;}
int  Echotron::getModfilts() {return Pmodfilts;}
int  Echotron::getLfoType() {return lfo.getPlfOtype();}
int  Echotron::getFilters() {return Pfilters;}
