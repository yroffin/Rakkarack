/*
 
 Convolotron.C - Convolotron effect
 Author: Ryam Billing & Jospe Andreu

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

#include <plugins/effect/Convolotron.h>
using namespace std;

Convolotron::Convolotron(int DS, int uq, int dq) :
		YroEffectPlugin("Convolotron",
						"Convolotron1: 67,64,1,100,0,64,30,20,0,0,0;"
						"Convolotron2: 67,64,1,100,0,64,30,20,1,0,0;"
						"Convolotron3: 67,75,1,100,0,64,30,20,2,0,0;"
						"Convolotron4: 67,60,1,100,0,64,30,20,3,0,0;") {
	//default values
	Pvolume = 50;
	Ppanning = 64;
	Plrcross = 100;
	Psafe = 1;
	Phidamp = 60;
	Filenum = 0;
	Plength = 50;
	Puser = 0;
	real_len = 0;
	convlength = .5f;
	fb = 0.0f;
	feedback = 0.0f;
	adjust(DS);

	templ = (float *) malloc(sizeof(float) * iPERIOD);
	tempr = (float *) malloc(sizeof(float) * iPERIOD);

	maxx_size = (int) (nfSAMPLE_RATE * convlength); //just to get the max memory allocated
	buf = (float *) malloc(sizeof(float) * maxx_size);
	rbuf = (float *) malloc(sizeof(float) * maxx_size);
	lxn = (float *) malloc(sizeof(float) * maxx_size);
	maxx_size--;
	offset = 0;
	M_Resample = new Resample(0);
	U_Resample = new Resample(dq); //Downsample, uses sinc interpolation for bandlimiting to avoid aliasing
	D_Resample = new Resample(uq);

	setPreset(0);
	cleanup();
}

Convolotron::~Convolotron() {
}
;

/*
 * Cleanup the effect
 */
void Convolotron::cleanup() {
}

void Convolotron::adjust(int DS) {

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
	u_up = (double) nPERIOD / (double) iPERIOD;
	u_down = (double) iPERIOD / (double) nPERIOD;
}

/*
 * Effect output
 */
void Convolotron::render(jack_nframes_t nframes, float * smpsl, float * smpsr) {
	int i, j, xindex;
	float l, lyn;

	if (DS_state != 0) {
		memcpy(templ, smpsl, sizeof(float) * iPERIOD);
		memcpy(tempr, smpsr, sizeof(float) * iPERIOD);
		U_Resample->out(templ, tempr, smpsl, smpsr, iPERIOD, u_up);
	}

	for (i = 0; i < nPERIOD; i++) {

		l = smpsl[i] + smpsr[i] + feedback;
		oldl = l * hidamp + oldl * (alpha_hidamp); //apply damping while I'm in the loop
		lxn[offset] = oldl;

		//Convolve left channel
		lyn = 0;
		xindex = offset;

		for (j = 0; j < length; j++) {
			if (--xindex < 0)
				xindex = maxx_size; //length of lxn is maxx_size.
			lyn += buf[j] * lxn[xindex]; //this is all there is to convolution
		}

		feedback = fb * lyn;
		templ[i] = lyn * levpanl;
		tempr[i] = lyn * levpanr;

		if (++offset > maxx_size)
			offset = 0;

	};

	if (DS_state != 0) {
		D_Resample->out(templ, tempr, efxoutl, efxoutr, nPERIOD, u_down);

	} else {
		memcpy(efxoutl, templ, sizeof(float) * iPERIOD);
		memcpy(efxoutr, tempr, sizeof(float) * iPERIOD);
	}

}
;

/*
 * Parameter control
 */
void Convolotron::setVolume(int Pvolume) {
	this->Pvolume = Pvolume;
	outvolume = (float) Pvolume / 127.0f;
	if (Pvolume == 0)
		cleanup();

}
;

void Convolotron::setPanning(int Ppanning) {
	this->Ppanning = Ppanning;
	lpanning = ((float) Ppanning + 0.5f) / 127.0f;
	rpanning = 1.0f - lpanning;
	levpanl = lpanning * level * 2.0f;
	levpanr = rpanning * level * 2.0f;

}
;

void Convolotron::setFile(int value) {
	double sr_ratio;

	offset = 0;
	maxx_read = maxx_size / 2;
	memset(buf, 0, sizeof(float) * maxx_size);
	memset(rbuf, 0, sizeof(float) * maxx_size);
	if (!Puser) {
		Filenum = value;
		memset(Filename, 0, sizeof(Filename));
		/**
		 * TODO define DATADIR
		 */
		sprintf(Filename, "%s/%d.wav", "DATADIR", Filenum + 1);
	}

	sfinfo.format = 0;
	/**
	 * TODO
	 if (!(infile = sf_open(Filename, SFM_READ, &sfinfo))) {
	 real_len = 1;
	 length = 1;
	 rbuf[0] = 1.0f;
	 process_rbuf();
	 return (0);
	 }
	 */

	if (sfinfo.frames > maxx_read)
		real_len = maxx_read;
	else
		real_len = sfinfo.frames;

	// TODO
	//sf_seek(infile, 0, SEEK_SET);
	//sf_readf_float(infile, buf, real_len);
	//sf_close(infile);

	if (sfinfo.samplerate != (int) nSAMPLE_RATE) {
		sr_ratio = (double) nSAMPLE_RATE / ((double) sfinfo.samplerate);
		M_Resample->mono_out(buf, rbuf, real_len, sr_ratio,
				lrint((double) real_len * sr_ratio));
		real_len = lrintf((float) real_len * (float) sr_ratio);
	}

	else
		memcpy(rbuf, buf, real_len * sizeof(float));

	process_rbuf();
}
;

void Convolotron::process_rbuf() {
	int ii, j, N, N2;
	float tailfader, alpha, a0, a1, a2, Nm1p, Nm1pp, IRpowa, IRpowb, ngain,
			maxamp;
	memset(buf, 0, sizeof(float) * real_len);

	if (length > real_len)
		length = real_len;
	/*Blackman Window function
	 wn = a0 - a1*cos(2*pi*n/(N-1)) + a2 * cos(4*PI*n/(N-1)
	 a0 = (1 - alpha)/2; a1 = 0.5; a2 = alpha/2
	 */
	alpha = 0.16f;
	a0 = 0.5f * (1.0f - alpha);
	a1 = 0.5f;
	a2 = 0.5 * alpha;
	N = length;
	N2 = length / 2;
	Nm1p = D_PI / ((float) (N - 1));
	Nm1pp = 4.0f * PI / ((float) (N - 1));

	for (ii = 0; ii < length; ii++) {
		if (ii < N2) {
			tailfader = 1.0f;
		} else {
			tailfader = a0 - a1 * cosf(ii * Nm1p) + a2 * cosf(ii * Nm1pp); //Calculate Blackman Window for right half of IR
		}

		buf[ii] = rbuf[ii] * tailfader; //Apply window function

	}

	memcpy(buf, rbuf, real_len * sizeof(float));

	IRpowa = IRpowb = maxamp = 0.0f;
	//compute IR signal power
	for (j = 0; j < maxx_read; j++) {
		IRpowa += fabsf(rbuf[j]);
		if (maxamp < fabsf(buf[j]))
			maxamp = fabsf(buf[j]); //find maximum level to normalize

		if (j < length) {
			IRpowb += fabsf(buf[j]);
		}

	}

	//if(maxamp < 0.3f) maxamp = 0.3f;
	ngain = IRpowa / IRpowb;
	if (ngain > maxx_read)
		ngain = maxx_read;
	for (j = 0; j < length; j++)
		buf[j] *= ngain;

}

void Convolotron::setHidamp(int Phidamp) {
	this->Phidamp = Phidamp;
	hidamp = 1.0f - (float) Phidamp / 127.1f;
	alpha_hidamp = 1.0f - hidamp;
}
;

void Convolotron::setSafe(int value) {
	Psafe = value;
}
void Convolotron::setLength(int value) {
	if (Psafe) {
		if (value < helper->getMaxLength())
			Plength = value;
		else
			Plength = helper->getMaxLength();
	} else
		Plength = value;
	convlength = ((float) Plength) / 1000.0f; //time in seconds
	length = (int) (nfSAMPLE_RATE * convlength); //time in samples
	process_rbuf();
}
void Convolotron::setLevel(int value) {
	Plevel = value;
	level = dB2rap (60.0f * (float)Plevel / 127.0f - 40.0f);
	levpanl = lpanning * level * 2.0f;
	levpanr = rpanning * level * 2.0f;
}
void Convolotron::setUser(int value) {
	Puser = value;
}
void Convolotron::setFb(int value) {
	Pfb = value;
	if (Pfb < 0) {
		fb = (float) .1f * value / 250.0f * .15f;
	} else {
		fb = (float) .1f * value / 500.0f * .15f;
	}
}

int  Convolotron::getVolume() {return Pvolume;}
int  Convolotron::getPanning() {return Ppanning;}
int  Convolotron::getSafe() {return Psafe;}
int  Convolotron::getLength() {return Plength;}
int  Convolotron::getFile() {return 0; /** TODO file ??? */}
int  Convolotron::getHidamp() {return Phidamp;}
int  Convolotron::getLevel() {return Plevel;}
int  Convolotron::getUser() {return Puser;}
int  Convolotron::getFb() {return Pfb;}
/**
 * toXml member
*/
const char *Convolotron::toXml() {
        char _buffer[256];
        char _formatd[] = {"<attribute name=\"%s\" value=\"%d\" />"};
        char _formatf[] = {"<attribute name=\"%s\" value=\"%9.40f\" />"};
        strcpy(_toXml,"<attributes>");
        sprintf(_buffer,_formatd,"DS_state",DS_state);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatd,"Filenum",Filenum);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatd,"maxx_size",maxx_size);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatd,"maxx_read",maxx_read);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatd,"real_len",real_len);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatd,"length",length);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatd,"nPERIOD",nPERIOD);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatd,"nSAMPLE_RATE",nSAMPLE_RATE);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatd,"offset",offset);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatd,"Pfb",Pfb);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatd,"Phidamp",Phidamp);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatd,"Plength",Plength);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatd,"Plevel",Plevel);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatd,"Plrcross",Plrcross);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatd,"Ppanning",Ppanning);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatd,"Psafe",Psafe);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatd,"Puser",Puser);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatd,"Pvolume",Pvolume);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatf,"level",level);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatf,"fb",fb);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatf,"feedback",feedback);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatf,"levpanl",levpanl);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatf,"levpanr",levpanr);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatf,"lpanning",lpanning);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatf,"rpanning",rpanning);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatf,"hidamp",hidamp);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatf,"alpha_hidamp",alpha_hidamp);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatf,"convlength",convlength);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatf,"oldl",oldl);
        strcat(_toXml,_buffer);
        sprintf(_buffer,_formatf,"nfSAMPLE_RATE",nfSAMPLE_RATE);
        strcat(_toXml,_buffer);
        strcat(_toXml,"</attributes>");
        return _toXml;
}
