/*
 Rakarrack Guitar FX

 Sequence.C - Simple compressor/Sequence effect with easy interface, minimal controls
 Copyright (C) 2010 Ryan Billing
 Author: Ryan Billing & Josep Andreu
 
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

#include <plugins/effect/Sequence.h>

using namespace std;

Sequence::Sequence(long int Quality, int DS, int uq, int dq) :
		YroEffectPlugin("Sequence",
		/**
		 TODO * ignore up to 7th parameters
		 */
		"Jumpy: 20,100,10,50,25,120,60,127,0,90,40,0,0,0,3;"
				"StairStep: 10,20,30,50,75,90,100,127,64,90,96,0,0,2,5;"
				"Mild: 20,30,10,40,25,60,100,50,0,90,40,0,0,0,4;"
				"WahWah: 11,55,15,95,12,76,11,36,30,80,110,0,4,1,2;"
				"FilterPan: 28,59,94,127,120,80,50,24,64,180,107,0,3,0,8;"
				"Stepper: 30,127,30,50,80,40,110,80,0,240,95,1,1,2,2;"
				"Shifter: 0,0,127,127,0,0,127,127,64,114,64,1,0,3,0;"
				"Tremor: 30,127,30,50,80,40,110,80,0,240,95,1,1,4,2;"
				"Boogie: 0,40,50,60,70,60,40,0,0,220,64,0,0,5,0;"
				"Chorus: 64,30,45,20,60,25,42,15,64,120,64,0,0,6,0;") {
	hq = Quality;
	adjust(DS);

	templ = (float *) malloc(sizeof(float) * iPERIOD);
	tempr = (float *) malloc(sizeof(float) * iPERIOD);

	outi = (float *) malloc(sizeof(float) * nPERIOD);
	outo = (float *) malloc(sizeof(float) * nPERIOD);

	U_Resample = new Resample(dq);
	D_Resample = new Resample(uq);

	filterl = NULL;
	filterr = NULL;

	MAXFREQ = 10000.0f;
	MINFREQ = 100.0f;
	fq = 75.0f;
	scount = 0;
	tcount = 0;
	rndflag = 0;
	subdiv = 2;
	filterl = new RBFilter(0, 80.0f, 40.0f, 2);
	filterr = new RBFilter(0, 80.0f, 40.0f, 2);
	modfilterl = new RBFilter(0, 25.0f, 0.15f, 2);
	modfilterr = new RBFilter(0, 25.0f, 0.15f, 2);
	setPreset(0);

	filterl->setmix(1, 0.33f, -1.0f, 0.25f);
	filterr->setmix(1, 0.33f, -1.0f, 0.25f);

	PS = new PitchShifter(window, hq, nfSAMPLE_RATE);
	PS->ratio = 1.0f;

	cleanup();
}
;

Sequence::~Sequence() {
}
;

/*
 * Cleanup the effect
 */
void Sequence::cleanup() {

	memset(outi, 0, sizeof(float) * nPERIOD);
	memset(outo, 0, sizeof(float) * nPERIOD);

}
;

/*
 * Effect output
 */
void Sequence::render(jack_nframes_t nframes, float * smpsl, float * smpsr) {
	int i;
	int nextcount, dnextcount;
	int hPERIOD;

	float ldiff, rdiff, lfol, lfor, ftcount;
	float lmod = 0.0f;
	float rmod = 0.0f;
	float ldbl, ldbr;

	if ((Pmode == 3) || (Pmode == 5) || (Pmode == 6))
		hPERIOD = nPERIOD;
	else
		hPERIOD = iPERIOD;

	if ((rndflag) && (tcount < hPERIOD + 1)) //This is an Easter Egg
			{
		srand(time(NULL));
		for (i = 0; i < 8; i++) {
			fsequence[i] = RND1;
		}
	}

	switch (Pmode) {
	case 0: //Lineal

		nextcount = scount + 1;
		if (nextcount > 7)
			nextcount = 0;
		ldiff = ifperiod * (fsequence[nextcount] - fsequence[scount]);
		lfol = fsequence[scount];

		dscount = (scount + Pstdiff) % 8;
		dnextcount = dscount + 1;
		if (dnextcount > 7)
			dnextcount = 0;
		rdiff = ifperiod * (fsequence[dnextcount] - fsequence[dscount]);
		lfor = fsequence[dscount];

		for (i = 0; i < iPERIOD; i++) //Maintain sequenced modulator
				{

			if (++tcount >= intperiod) {
				tcount = 0;
				scount++;
				if (scount > 7)
					scount = 0; //reset to beginning of sequence buffer

				nextcount = scount + 1;
				if (nextcount > 7)
					nextcount = 0;
				ldiff = ifperiod * (fsequence[nextcount] - fsequence[scount]);
				lfol = fsequence[scount];

				dscount = (scount + Pstdiff) % 8;
				dnextcount = dscount + 1;
				if (dnextcount > 7)
					dnextcount = 0;
				rdiff = ifperiod * (fsequence[dnextcount] - fsequence[dscount]);
				lfor = fsequence[dscount];
			}

			ftcount = (float) tcount;

			lmod = lfol + ldiff * ftcount;
			rmod = lfor + rdiff * ftcount;

			if (Pamplitude) {
				ldbl = lmod * (1.0f - cosf(D_PI * ifperiod * ftcount));
				ldbr = rmod * (1.0f - cosf(D_PI * ifperiod * ftcount));

				efxoutl[i] = ldbl * smpsl[i];
				efxoutr[i] = ldbr * smpsr[i];
			}

			float frl = MINFREQ + MAXFREQ * lmod;
			float frr = MINFREQ + MAXFREQ * rmod;

			if (i % 8 == 0) {
				filterl->setfreq_and_q(frl, fq);
				filterr->setfreq_and_q(frr, fq);
			}

			efxoutl[i] = filterl->filterout_s(efxoutl[i]);
			efxoutr[i] = filterr->filterout_s(efxoutr[i]);

		}
		break;

	case 1: //Up Down

		for (i = 0; i < iPERIOD; i++) //Maintain sequenced modulator
				{

			if (++tcount >= intperiod) {
				tcount = 0;
				scount++;
				if (scount > 7)
					scount = 0; //reset to beginning of sequence buffer
				dscount = (scount + Pstdiff) % 8;
			}

			ftcount = M_PI * ifperiod * (float) (tcount);

			lmod = sinf(ftcount) * fsequence[scount];
			rmod = sinf(ftcount) * fsequence[dscount];

			if (Pamplitude) {
				ldbl = lmod * (1.0f - cosf(2.0f * ftcount));
				ldbr = rmod * (1.0f - cosf(2.0f * ftcount));

				efxoutl[i] = ldbl * smpsl[i];
				efxoutr[i] = ldbr * smpsr[i];
			}

			float frl = MINFREQ + MAXFREQ * lmod;
			float frr = MINFREQ + MAXFREQ * rmod;

			if (i % 8 == 0) {
				filterl->setfreq_and_q(frl, fq);
				filterr->setfreq_and_q(frr, fq);
			}

			efxoutl[i] = filterl->filterout_s(efxoutl[i]);
			efxoutr[i] = filterr->filterout_s(efxoutr[i]);

		}

		break;

	case 2: //Stepper

		for (i = 0; i < iPERIOD; i++) //Maintain sequenced modulator
				{

			if (++tcount >= intperiod) {
				tcount = 0;
				scount++;
				if (scount > 7)
					scount = 0; //reset to beginning of sequence buffer
				dscount = (scount + Pstdiff) % 8;
			}

			lmod = fsequence[scount];
			rmod = fsequence[dscount];

			lmod = modfilterl->filterout_s(lmod);
			rmod = modfilterr->filterout_s(rmod);

			if (Pamplitude) {
				ldbl = seqpower * lmod;
				ldbr = seqpower * rmod;

				efxoutl[i] = ldbl * smpsl[i];
				efxoutr[i] = ldbr * smpsr[i];
			}

			float frl = MINFREQ + lmod * MAXFREQ;
			float frr = MINFREQ + rmod * MAXFREQ;

			if (i % 8 == 0) {
				filterl->setfreq_and_q(frl, fq);
				filterr->setfreq_and_q(frr, fq);
			}

			efxoutl[i] = filterl->filterout_s(efxoutl[i]);
			efxoutr[i] = filterr->filterout_s(efxoutr[i]);

		}

		break;

	case 3: //Shifter

		nextcount = scount + 1;
		if (nextcount > 7)
			nextcount = 0;
		ldiff = ifperiod * (fsequence[nextcount] - fsequence[scount]);
		lfol = fsequence[scount];

		if (DS_state != 0) {
			memcpy(templ, smpsl, sizeof(float) * iPERIOD);
			memcpy(tempr, smpsr, sizeof(float) * iPERIOD);
			U_Resample->out(templ, tempr, smpsl, smpsr, iPERIOD, u_up);
		}

		for (i = 0; i < nPERIOD; i++) //Maintain sequenced modulator
				{

			if (++tcount >= intperiod) {
				tcount = 0;
				scount++;
				if (scount > 7)
					scount = 0; //reset to beginning of sequence buffer

				nextcount = scount + 1;
				if (nextcount > 7)
					nextcount = 0;
				ldiff = ifperiod * (fsequence[nextcount] - fsequence[scount]);
				lfol = fsequence[scount];
			}

			ftcount = (float) tcount;

			lmod = 1.0f + lfol + ldiff * ftcount;

			if (Pamplitude)
				lmod = 1.0f - (lfol + ldiff * ftcount) * .5f;

			outi[i] = (smpsl[i] + smpsr[i]) * .5;
			if (outi[i] > 1.0)
				outi[i] = 1.0f;
			if (outi[i] < -1.0)
				outi[i] = -1.0f;
		}

		PS->ratio = lmod;
		PS->smbPitchShift(PS->ratio, nPERIOD, window, hq, nfSAMPLE_RATE, outi,
				outo);

		memcpy(templ, outo, sizeof(float) * nPERIOD);
		memcpy(tempr, outo, sizeof(float) * nPERIOD);

		if (DS_state != 0) {
			D_Resample->out(templ, tempr, efxoutl, efxoutr, nPERIOD, u_down);
		} else {
			memcpy(efxoutl, templ, sizeof(float) * iPERIOD);
			memcpy(efxoutr, tempr, sizeof(float) * iPERIOD);
		}

		break;

	case 4: //Tremor

		nextcount = scount + 1;
		if (nextcount > 7)
			nextcount = 0;
		ldiff = ifperiod * (fsequence[nextcount] - fsequence[scount]);
		lfol = fsequence[scount];

		dscount = (scount + Pstdiff) % 8;
		dnextcount = dscount + 1;
		if (dnextcount > 7)
			dnextcount = 0;
		rdiff = ifperiod * (fsequence[dnextcount] - fsequence[dscount]);
		lfor = fsequence[dscount];

		for (i = 0; i < iPERIOD; i++) //Maintain sequenced modulator
				{

			if (++tcount >= intperiod) {
				tcount = 0;
				scount++;
				if (scount > 7)
					scount = 0; //reset to beginning of sequence buffer

				nextcount = scount + 1;
				if (nextcount > 7)
					nextcount = 0;
				ldiff = ifperiod * (fsequence[nextcount] - fsequence[scount]);
				lfol = fsequence[scount];

				dscount = (scount + Pstdiff) % 8;
				dnextcount = dscount + 1;
				if (dnextcount > 7)
					dnextcount = 0;
				rdiff = ifperiod * (fsequence[dnextcount] - fsequence[dscount]);
				lfor = fsequence[dscount];
			}
//Process Amplitude modulation
			if (Pamplitude) {
				ftcount = (float) tcount;
				lmod = lfol + ldiff * ftcount;
				rmod = lfor + rdiff * ftcount;

				ldbl = seqpower * lmod
						* (1.0f - cosf(D_PI * ifperiod * ftcount));
				ldbr = seqpower * rmod
						* (1.0f - cosf(D_PI * ifperiod * ftcount));

				efxoutl[i] = ldbl * smpsl[i];
				efxoutr[i] = ldbr * smpsr[i];
			} else {
				lmod = seqpower * fsequence[scount];
				rmod = seqpower * fsequence[dscount];
				lmod = modfilterl->filterout_s(lmod);
				rmod = modfilterr->filterout_s(rmod);

				efxoutl[i] = lmod * smpsl[i];
				efxoutr[i] = rmod * smpsr[i];
			}

		}
		;
		break;

	case 5: //Arpegiator

		lfol = floorf(fsequence[scount] * 12.75f);

		if (DS_state != 0) {
			memcpy(templ, smpsl, sizeof(float) * iPERIOD);
			memcpy(tempr, smpsr, sizeof(float) * iPERIOD);
			U_Resample->out(templ, tempr, smpsl, smpsr, iPERIOD, u_up);
		}

		for (i = 0; i < nPERIOD; i++) //Maintain sequenced modulator
				{

			if (++tcount >= intperiod) {
				tcount = 0;
				scount++;
				if (scount > 7)
					scount = 0; //reset to beginning of sequence buffer
				lfol = floorf(fsequence[scount] * 12.75f);
			}

			lmod = powf(2.0f, lfol / 12.0f);

			if (Pamplitude)
				lmod = powf(2.0f, -lfol / 12.0f);

			outi[i] = (smpsl[i] + smpsr[i]) * .5;
			if (outi[i] > 1.0)
				outi[i] = 1.0f;
			if (outi[i] < -1.0)
				outi[i] = -1.0f;
		}

		PS->ratio = lmod;
		PS->smbPitchShift(PS->ratio, nPERIOD, window, hq, nfSAMPLE_RATE, outi,
				outo);

		memcpy(templ, outo, sizeof(float) * nPERIOD);
		memcpy(tempr, outo, sizeof(float) * nPERIOD);

		if (DS_state != 0) {
			D_Resample->out(templ, tempr, efxoutl, efxoutr, nPERIOD, u_down);
		} else {
			memcpy(efxoutl, templ, sizeof(float) * nPERIOD);
			memcpy(efxoutr, tempr, sizeof(float) * nPERIOD);
		}

		break;

	case 6: //Chorus

		nextcount = scount + 1;
		if (nextcount > 7)
			nextcount = 0;
		ldiff = ifperiod * (fsequence[nextcount] - fsequence[scount]);
		lfol = fsequence[scount];

		if (DS_state != 0) {
			memcpy(templ, smpsl, sizeof(float) * iPERIOD);
			memcpy(tempr, smpsr, sizeof(float) * iPERIOD);
			U_Resample->out(templ, tempr, smpsl, smpsr, iPERIOD, u_up);
		}

		for (i = 0; i < nPERIOD; i++) //Maintain sequenced modulator
				{

			if (++tcount >= intperiod) {
				tcount = 0;
				scount++;
				if (scount > 7)
					scount = 0; //reset to beginning of sequence buffer

				nextcount = scount + 1;
				if (nextcount > 7)
					nextcount = 0;
				ldiff = ifperiod * (fsequence[nextcount] - fsequence[scount]);
				lfol = fsequence[scount];
			}

			ftcount = (float) tcount;

			lmod = 1.0f + (lfol + ldiff * ftcount) * .03f;
			if (Pamplitude)
				lmod = 1.0f - (lfol + ldiff * ftcount) * .03f;

			outi[i] = (smpsl[i] + smpsr[i]) * .5;
			if (outi[i] > 1.0)
				outi[i] = 1.0f;
			if (outi[i] < -1.0)
				outi[i] = -1.0f;
		}

		PS->ratio = lmod;
		PS->smbPitchShift(PS->ratio, nPERIOD, window, hq, nfSAMPLE_RATE, outi,
				outo);

		if (Pstdiff == 1) {
			for (i = 0; i < nPERIOD; i++) {
				templ[i] = smpsl[i] - smpsr[i] + outo[i];
				tempr[i] = smpsl[i] - smpsr[i] + outo[i];
			}
		} else if (Pstdiff == 2) {
			for (i = 0; i < nPERIOD; i++) {
				templ[i] = outo[i] * (1.0f - panning);
				tempr[i] = outo[i] * panning;
			}
		} else {
			memcpy(templ, outo, sizeof(float) * nPERIOD);
			memcpy(tempr, outo, sizeof(float) * nPERIOD);
		}

		if (DS_state != 0) {
			D_Resample->out(templ, tempr, efxoutl, efxoutr, nPERIOD, u_down);
		} else {
			memcpy(efxoutl, templ, sizeof(float) * nPERIOD);
			memcpy(efxoutr, tempr, sizeof(float) * nPERIOD);
		}

		break;

		// here case 6:
		//
		// break;

	}

}
;

/*
 * Parameter control
 */

void Sequence::setRanges(int value) {

	switch (value) {

	case 1: //typical for wahwah pedal
		MINFREQ = 450.0f;
		MAXFREQ = 2500.0f;
		break;
	case 2:
		MINFREQ = 150.0f;
		MAXFREQ = 4000.0f;
		break;
	case 3:
		MINFREQ = 40.0f;
		MAXFREQ = 800.0f;
		break;
	case 4:
		MINFREQ = 100.0f;
		MAXFREQ = 1600.0f;
		break;
	case 5:
		MINFREQ = 80.0f;
		MAXFREQ = 16000.0f;
		break;
	case 6:
		MINFREQ = 60.0f;
		MAXFREQ = 18000.0f;
		break;
	case 7:
		MINFREQ = 40.0f;
		MAXFREQ = 2200.0f;
		break;
	case 8:
		MINFREQ = 20.0f;
		MAXFREQ = 6000.0f;
		break;

	}
}

void Sequence::adjust(int DS) {

	DS_state = DS;

	switch (DS) {

	case 0:
		nPERIOD = iPERIOD;
		nSAMPLE_RATE = iSAMPLE_RATE;
		nfSAMPLE_RATE = fSAMPLE_RATE;
		window = 2048;
		break;

	case 1:
		nPERIOD = lrintf(fPERIOD * 96000.0f / fSAMPLE_RATE);
		nSAMPLE_RATE = 96000;
		nfSAMPLE_RATE = 96000.0f;
		window = 2048;
		break;

	case 2:
		nPERIOD = lrintf(fPERIOD * 48000.0f / fSAMPLE_RATE);
		nSAMPLE_RATE = 48000;
		nfSAMPLE_RATE = 48000.0f;
		window = 2048;
		break;

	case 3:
		nPERIOD = lrintf(fPERIOD * 44100.0f / fSAMPLE_RATE);
		nSAMPLE_RATE = 44100;
		nfSAMPLE_RATE = 44100.0f;
		window = 2048;
		break;

	case 4:
		nPERIOD = lrintf(fPERIOD * 32000.0f / fSAMPLE_RATE);
		nSAMPLE_RATE = 32000;
		nfSAMPLE_RATE = 32000.0f;
		window = 2048;
		break;

	case 5:
		nPERIOD = lrintf(fPERIOD * 22050.0f / fSAMPLE_RATE);
		nSAMPLE_RATE = 22050;
		nfSAMPLE_RATE = 22050.0f;
		window = 1024;
		break;

	case 6:
		nPERIOD = lrintf(fPERIOD * 16000.0f / fSAMPLE_RATE);
		nSAMPLE_RATE = 16000;
		nfSAMPLE_RATE = 16000.0f;
		window = 1024;
		break;

	case 7:
		nPERIOD = lrintf(fPERIOD * 12000.0f / fSAMPLE_RATE);
		nSAMPLE_RATE = 12000;
		nfSAMPLE_RATE = 12000.0f;
		window = 512;
		break;

	case 8:
		nPERIOD = lrintf(fPERIOD * 8000.0f / fSAMPLE_RATE);
		nSAMPLE_RATE = 8000;
		nfSAMPLE_RATE = 8000.0f;
		window = 512;
		break;

	case 9:
		nPERIOD = lrintf(fPERIOD * 4000.0f / fSAMPLE_RATE);
		nSAMPLE_RATE = 4000;
		nfSAMPLE_RATE = 4000.0f;
		window = 256;
		break;
	}
	u_up = (double) nPERIOD / (double) iPERIOD;
	u_down = (double) iPERIOD / (double) nPERIOD;
}

void Sequence::setSequence(int value) {
	int testegg, i;
	Psequence[7] = value;
	fsequence[7] = (float) value / 127.0f;
	seqpower = 0.0f;
	for (i = 0; i < 8; i++)
		seqpower += fsequence[i];
	if (seqpower > 0.1f) {
		seqpower = 15.0f / seqpower;
		rndflag = 0;
	}
	testegg = 0;
	for (i = 0; i < 8; i++)
		testegg += Psequence[i];
	if (testegg < 4) {
		seqpower = 5.0f; //Easter egg
		rndflag = 1;
	}
}
void Sequence::setVolume(int value) {
	Pvolume = value;
	outvolume = (float) Pvolume / 127.0f;
}
void Sequence::setTempo(int value) {
	Ptempo = value;
	if ((Pmode == 3) || (Pmode == 5) || (Pmode == 6))
		fperiod = nfSAMPLE_RATE * 60.0f / (subdiv * (float) value);
	else
		fperiod = fSAMPLE_RATE * 60.0f / (subdiv * (float) value); //number of samples before next value

	ifperiod = 1.0f / fperiod;
	intperiod = (int) fperiod;
}
void Sequence::setQ(int value) {
	Pq = value;
	panning = ((float) value + 64.0f) / 128.0;
	fq = powf(60.0f, ((float) value - 64.0f) / 64.0f);
}
void Sequence::setAmplitude(int value) {
	Pamplitude = value;
}
void Sequence::setStdiff(int value) {
	Pstdiff = value;
}
void Sequence::setMode(int value) {
	Pmode = value;
	setTempo(Ptempo);
}
void Sequence::setRange(int value) {
	Prange = value;
	setRanges(Prange);
}

int  Sequence::getSequence() {return Psequence[7];}
int  Sequence::getVolume() {return Pvolume;}
int  Sequence::getTempo() {return Ptempo;}
int  Sequence::getQ() {return Pq;}
int  Sequence::getAmplitude() {return Pamplitude;}
int  Sequence::getStdiff() {return Pstdiff;}
int  Sequence::getMode() {return Pmode;}
int  Sequence::getRange() {return Prange;}
/**
 * toXml member
*/
const char *Sequence::toXml() {
        char _buffer[256];
        char _formatd[] = {"<attribute name=\"%s\" value=\"%d\" />"};
        char _formatf[] = {"<attribute name=\"%s\" value=\"%9.40f\" />"};
        strcpy(_toXml,"<attributes>");
        sprintf(_buffer,_formatd,"DS_state",DS_state);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatd,"hq",hq);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatd,"nPERIOD",nPERIOD);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatd,"nSAMPLE_RATE",nSAMPLE_RATE);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatd,"Pamplitude",Pamplitude);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatd,"Pmode",Pmode);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatd,"Pq",Pq);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatd,"Prange",Prange);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatd,"Psequence[8]",Psequence[8]);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatd,"Pstdiff",Pstdiff);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatd,"Ptempo",Ptempo);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatd,"Pvolume",Pvolume);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatd,"rndflag",rndflag);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatd,"subdiv",subdiv);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatd,"tcount",tcount);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatd,"scount",scount);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatd,"dscount",dscount);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatd,"intperiod",intperiod);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatf,"fq",fq);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatf,"fsequence[8]",fsequence[8]);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatf,"ifperiod",ifperiod);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatf,"fperiod",fperiod);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatf,"seqpower",seqpower);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatf,"MINFREQ",MINFREQ);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatf,"MAXFREQ",MAXFREQ);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatf,"nfSAMPLE_RATE",nfSAMPLE_RATE);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatf,"panning",panning);
        strcat(_toXml,_buffer);
        strcat(_toXml,"</attributes>");
        return _toXml;
}
