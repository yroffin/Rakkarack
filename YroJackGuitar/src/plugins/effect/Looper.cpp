/*
 ZynAddSubFX - a software synthesizer
 
 Looper.C - Looper effect
 Copyright (C) 2002-2005 Nasca Octavian Paul
 Author: Nasca Octavian Paul

 Modified for rakarrack by Josep Andreu
 Reverse Looper effect by Transmogrifox

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

#include <plugins/effect/Looper.h>

using namespace std;

Looper::Looper(float size) :
		YroEffectPlugin("Looper",
				"Looper2seconds: 64,0,1,0,1,0,64,1,0,1,64,1,0,0;"
						"Misc: 64,0,1,0,1,1,64,1,0,1,64,1,0,0;") {
	//default values
	Pclear = 1;
	Pplay = 0;
	Precord = 0;
	Pstop = 1;
	PT1 = 1;
	PT2 = 0;
	Pautoplay = 0;
	rplaystate = 0;
	first_time1 = 1;
	first_time2 = 1;
	Pms = 0;
	Ptempo = 120;
	setTempo(120);
	Pbar = 2;
	setBar(2);
	ticker.cleanup();

	Srate_Attack_Coeff = 1.0f / (fSAMPLE_RATE * ATTACK);
	maxx_delay = (int) lrintf(fSAMPLE_RATE * size);
	fade = (int) iSAMPLE_RATE / 2; //1/2 SR fade time available

	ldelay = new float[maxx_delay];
	rdelay = new float[maxx_delay];
	t2ldelay = new float[maxx_delay];
	t2rdelay = new float[maxx_delay];

	setPreset(0);
	cleanup();
}
;

Looper::~Looper() {
}
;

/*
 * Cleanup the effect
 */
void Looper::cleanuppt1() {
	int i;
	for (i = 0; i < maxx_delay; i++) {
		ldelay[i] = 0.0f;
		rdelay[i] = 0.0f;
	}

}
;
void Looper::cleanuppt2() {
	int i;
	for (i = 0; i < maxx_delay; i++) {
		t2ldelay[i] = 0.0f;
		t2rdelay[i] = 0.0f;
	}

}
;

void Looper::cleanup() {
	cleanuppt1();
	cleanuppt2();

}
;
/*
 * Initialize the delays
 */
void Looper::initdelays() {

	if (PT1) {
		kl = 0;
		ticker.cleanup();
		dl = maxx_delay;
		rvkl = dl - 1;
		cleanuppt1();
	}

	if (PT2) {
		kl2 = 0;
		ticker.cleanup();
		dl2 = maxx_delay;
		rvkl2 = dl2 - 1;
		cleanuppt2();
	}
	Srate_Attack_Coeff = 90.0f / (dl + dl2); // Set swell time

}
;

/*
 * Effect output
 */
void Looper::render(jack_nframes_t nframes, float * smpsl, float * smpsr) {
	int i;
	float rswell, lswell;
	float ticktock[iPERIOD];
	if ((Pmetro) && (Pplay) && (!Pstop))
		ticker.metronomeout(ticktock);

	for (i = 0; i < iPERIOD; i++) {

		if ((Pplay) && (!Pstop)) {
			if (Precord) {
				if ((Prec1) && (PT1)) {
					ldelay[kl] += pregain1 * smpsl[i];
					rdelay[kl] += pregain1 * smpsr[i];
				}
				if ((Prec2) && (PT2)) {
					t2ldelay[kl2] += pregain2 * smpsl[i];
					t2rdelay[kl2] += pregain2 * smpsr[i];
				}

			}

			if (!Pclear) {
				if (++kl >= dl)
					kl = 0;
				rvkl = dl - 1 - kl;
				if (++kl2 >= dl2)
					kl2 = 0;
				rvkl2 = dl2 - 1 - kl2;
				if ((Plink) || (PT1))
					timeposition(kl);
				else
					timeposition(kl2);

			}

			if (Preverse) {

				lswell = (float) (abs(kl - rvkl)) * Srate_Attack_Coeff;
				if (lswell <= PI) {
					lswell = 0.5f * (1.0f - cosf(lswell)); //Clickless transition
					efxoutl[i] =
							(fade1 * ldelay[rvkl] + fade2 * t2ldelay[rvkl2])
									* lswell; //Volume ducking near zero crossing.
				} else {
					efxoutl[i] = fade1 * ldelay[rvkl] + fade2 * t2ldelay[rvkl2];
				}

				rswell = (float) (abs(kl - rvkl)) * Srate_Attack_Coeff;
				if (rswell <= PI) {
					rswell = 0.5f * (1.0f - cosf(rswell)); //Clickless transition
					efxoutr[i] =
							(fade1 * rdelay[rvkl] + fade2 * t2rdelay[rvkl2])
									* rswell; //Volume ducking near zero crossing.
				} else {
					efxoutr[i] = fade1 * rdelay[rvkl] + fade2 * t2rdelay[rvkl2];
				}

			} else {

				efxoutl[i] = fade1 * ldelay[kl] + fade2 * t2ldelay[kl2];
				efxoutr[i] = fade1 * rdelay[kl] + fade2 * t2rdelay[kl2];

			}

		} else {
			efxoutl[i] = 0.0f;
			efxoutr[i] = 0.0f;
		}

		if ((Pmetro) && (Pplay) && (!Pstop)) {
			efxoutl[i] += ticktock[i] * mvol; //if you want to hear the metronome in Looper
			efxoutr[i] += ticktock[i] * mvol;
		}
	};

}
;

/*
 * Parameter control
 */

void Looper::setMvol(int value) {
	mvol = 2.0f * (float) value / 100.0f;
}

void Looper::setTempo(int value) {
	Ptempo = value;
	setBar(Pbar);
	ticker.set_tempo(value);
}

int Looper::set_len(int value) {

	int x = value / barlen;
	int a1 = 0;
	int a2 = 0;

	a1 = abs(value - (x * barlen));
	a2 = abs(value - ((x + 1) * barlen));

	if (a2 < a1)
		return (((x + 1) * barlen) + 1);

	return (x * barlen + 1);

}

int Looper::cal_len(int value) {

	int lenbar = lrintf((float) value * 60.0 / (float) Ptempo * fSAMPLE_RATE);

	looper_ts = lenbar / value;
	return (lenbar);
// return(lrintf((float)value*60.0/(float)Ptempo*fSAMPLE_RATE));

}

void Looper::timeposition(int value) {

	looper_bar = value / barlen + 1;
	looper_qua = (value % barlen) / looper_ts + 1;

}

void Looper::setBar(int value) {

	Pbar = value;
	switch (value) {

	case 0:
		barlen = cal_len(2);
		if (!Pms)
			ticker.set_meter(2);
		break;
	case 1:
		barlen = cal_len(3);
		if (!Pms)
			ticker.set_meter(3);
		break;
	case 2:
		barlen = cal_len(4);
		if (!Pms)
			ticker.set_meter(4);
		break;
	case 3:
		barlen = cal_len(5);
		if (!Pms)
			ticker.set_meter(5);
		break;
	case 4:
		barlen = cal_len(6);
		if (!Pms)
			ticker.set_meter(6);
		break;
	case 5:
		barlen = cal_len(7);
		if (!Pms)
			ticker.set_meter(7);
		break;
	case 6:
		barlen = cal_len(9);
		if (!Pms)
			ticker.set_meter(9);
		break;
	case 7:
		barlen = cal_len(11);
		if (!Pms)
			ticker.set_meter(11);
		break;
	}

}

void Looper::setFade() {
	fade1 = (float) (Pfade1) / 64.0f;
	fade2 = (float) (Pfade2) / 64.0f;
	pregain1 = 1.015f / (fade1 + 0.015f); //this is so material being recorded mixes at the same level as what you hear
	pregain2 = 1.015f / (fade2 + 0.015f); //magic number is ~1/66, keeps record gain from getting too high
	fade1 *= track1gain;
	fade2 *= track2gain;
}

void Looper::getstate() {
	progstate[0] = Pplay;
	progstate[1] = Pstop;
	progstate[2] = Precord;
	progstate[3] = Pclear;
	progstate[4] = PT1;
	progstate[5] = PT2;
}
;

void Looper::clearAll() {
	Pclear = 1; //Clear everything and erase the loop
	if (PT1)
		first_time1 = 1;
	if (PT2)
		first_time2 = 1;
	if ((PT1) && (PT2))
		Pplay = 0;
	Precord = 0;
	getstate();
}

void Looper::setVolume(int value) {
	Pvolume = value;
	outvolume = (float) Pvolume / 127.0f;
	clearAll();
}
void Looper::setMisc1(int value) {
	if (Pplay) {
		Pplay = 0;
	} else {
		Pplay = (PT1 || PT2); //prevents accidental record state when niether track is active
		if (!Pstop) { //if stop was pushed last time, then resume where it left off
			if (PT1)
				kl = 0;
			if (PT2)
				kl2 = 0;
			ticker.cleanup();
		}
		Pstop = 0;
	}
	if (Pstop) {
		Pstop = 0;
		Pplay = 0;
	}
	Pclear = 0;
	clearAll();
}
void Looper::setMisc2(int value) {
	if (Pstop)
		Pstop = 0;
	else
		Pstop = 1;
	if (Precord) {
		Pstop = 0;
		if ((first_time1 && Prec1) && PT1) {
			dl = set_len(kl);
			//printf("dl %dn",dl);
			first_time1 = 0;
			if (Plink) {
				dl2 = dl;
			}
			kl = 0;
			ticker.cleanup();
		}
		if ((first_time2 && Prec2) && PT2) {
			dl2 = set_len(kl2);
			first_time2 = 0;
			if (Plink) {
				dl = dl2;
			}
			kl2 = 0;
			ticker.cleanup();
		}
		Precord = 0;
		Pplay = 0;
	}
	clearAll();
}
void Looper::setMisc3(int value) {
	if (Precord) {
		if ((first_time1 && Prec1) && PT1) {
			dl = set_len(kl);
			first_time1 = 0;
			if (Plink) {
				dl2 = dl;
			}
			kl = 0;
			ticker.cleanup();
		}
		if ((first_time2 && Prec2) && PT2) {
			dl2 = set_len(kl2);
			first_time2 = 0;
			if (Plink) {
				dl = dl2;
			}
			kl2 = 0;
			ticker.cleanup();
		}
		Precord = 0;
		Pplay = rplaystate;
		if (Pautoplay)
			Pplay = 1;
		Pstop = 0;
	} else {
		Precord = 1;
		rplaystate = Pplay;
		if (Pautoplay) {
			Pplay = 1;
			if ((first_time1 && Prec1) || (first_time2 && Prec2))
				ticker.cleanup();
		} else
			Pplay = 0;
	}
	Pstop = 0;
	Pclear = 0;
	clearAll();
}
void Looper::setMisc4(int value) {
	Pclear = 1; //Clear everything and erase the loop
	if (PT1)
		first_time1 = 1;
	if (PT2)
		first_time2 = 1;
	if ((PT1) && (PT2))
		Pplay = 0;
	Precord = 0;
	if (PT1)
		kl = 0;
	if (PT2)
		kl2 = 0;
	ticker.cleanup();
	initdelays();
	clearAll();
}
void Looper::setReverse(int value) {
	Preverse = value; //Playback in reverse
	clearAll();
}
void Looper::setFade1(int value) {
	Pfade1 = value;
	setFade();
	clearAll();
}
void Looper::setMisc5(int value) {
	if (PT1) {
		PT1 = 0;
	} else {
		PT1 = 1;
		if (!Plink) {
			if (first_time1)
				initdelays();
			PT2 = 0; //if they aren't linked in time, then you want to toggle 1&2
			track2gain = 0.0f;
			kl = 0;
			ticker.cleanup();
			setFade();
		}
	}
	track1gain = (float) PT1;
	setFade();
	clearAll();
}
void Looper::setMisc6(int value) {
	if (PT2) {
		PT2 = 0;
	} else {
		PT2 = 1;
		if (!Plink) {
			PT1 = 0;
			if (first_time2)
				initdelays();
			kl2 = 0;
			ticker.cleanup();
			track1gain = 0.0f;
		}
	}
	track2gain = (float) PT2;
	setFade();
	clearAll();
}
void Looper::setAutoplay(int value) {
	Pautoplay = value;
	clearAll();
}
void Looper::setFade2(int value) {
	Pfade2 = value;
	setFade();
	clearAll();
}
void Looper::setRec1(int value) {
	Prec1 = value;
	clearAll();
}
void Looper::setRec2(int value) {
	Prec2 = value;
	clearAll();
}
void Looper::setLink(int value) {
	Plink = value;
	if (Plink) {
		if (Prec1)
			dl2 = dl;
		if (Prec2)
			dl = dl2; //if both are true, then it is only a redundant assignment
	}
	clearAll();
}
void Looper::setMetro(int value) {
	Pmetro = value;
	clearAll();
}
void Looper::setMs(int value) {
	Pms = value;
	if (Pms == 0)
		setBar(Pbar);
	if (Pms == 1)
		ticker.set_meter(1);
	if (Pms == 2)
		ticker.set_meter(0);
	clearAll();
}

int Looper::getVolume() {
	return Pvolume;
}
int Looper::getMisc1() {
	return 0;
}
int Looper::getMisc2() {
	return 0;
}
int Looper::getMisc3() {
	return 0;
}
int Looper::getMisc4() {
	return 0;
}
int Looper::getReverse() {
	return Preverse;
}
int Looper::getFade1() {
	return Pfade1;
}
int Looper::getMisc5() {
	return 0;
}
int Looper::getMisc6() {
	return 0;
}
int Looper::getAutoplay() {
	return Pautoplay;
}
int Looper::getFade2() {
	return Pfade2;
}
int Looper::getRec1() {
	return Prec1;
}
int Looper::getRec2() {
	return Prec2;
}
int Looper::getLink() {
	return Plink;
}
int Looper::getTempo() {
	return Ptempo;
}
int Looper::getBar() {
	return Pbar;
}
int Looper::getMetro() {
	return Pmetro;
}
int Looper::getMs() {
	return Pms;
}
