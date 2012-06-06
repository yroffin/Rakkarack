/*
  ZynAddSubFX - a software synthesizer

  Distorsion.h - Distorsion Effect
  Copyright (C) 2002-2005 Nasca Octavian Paul
  Author: Nasca Octavian Paul

  Modified for rakarrack by Josep Andreu & Hernan Ordiales & Ryan Billing

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

#ifndef DISTORTION_H_
#define DISTORTION_H_

#include <plugins/YroEffectPlugin.h>
#include <plugins/filter/AnalogFilter.h>
#include <plugins/filter/WaveShaper.h>

namespace std {

class Distortion: public YroEffectPlugin {
public:
	Distortion();
	virtual ~Distortion();
	virtual void render(jack_nframes_t nframes, float *smpsl, float *smpsr);
	const char *toXml();

	/**
	 * setter
	 */
	void cleanup ();
	void applyFilters(float * efxoutl, float * efxoutr);

	enum functions {
		_preset,
		_volume,
		_panning,
		_lrcross,
		_drive,
		_level,
		_type,
		_negate,
		_lpf,
		_hpf,
		_stereo,
		_prefiltering,
		_octave
	};

	int  get0() {return getPreset();};
	void set0(int value) {setPreset(value);};
	int  get1() {return getVolume();};
	void set1(int value) {setVolume(value);};
	int  get2() {return getPanning();};
	void set2(int value) {setPanning(value);};
	int  get3() {return getLrcross();};
	void set3(int value) {setLrcross(value);};
	int  get4() {return getDrive();};
	void set4(int value) {setDrive(value);};
	int  get5() {return getLevel();};
	void set5(int value) {setLevel(value);};
	int  get6() {return getType();};
	void set6(int value) {setType(value);};
	int  get7() {return getNegate();};
	void set7(int value) {setNegate(value);};
	int  get8() {return getLpf();};
	void set8(int value) {setLpf(value);};
	int  get9() {return getHpf();};
	void set9(int value) {setHpf(value);};
	int  get10() {return getStereo();};
	void set10(int value) {setStereo(value);};
	int  get11() {return getPrefiltering();};
	void set11(int value) {setPrefiltering(value);};
	int  get12() {return getOctave();};
	void set12(int value) {setOctave(value);};

	int getDrive();
	void setDrive(int pdrive);
	int getHpf() const;
	void setHpf(int phpf);
	int getLevel() const;
	void setLevel(int plevel);
	int getLpf() const;
	void setLpf(int plpf);
	int getOctave() const;
	void setOctave(int poctave);
	int getPanning() const;
	void setPanning(int ppanning);
	int getVolume() const;
	void setVolume(int pvolume);
	int getLrcross() const;
	void setLrcross(int plrcross);
	int getType() const;
	void setType(int ptype);
	int getNegate() const;
	void setNegate(int pnegate);
	int getStereo() const;
	void setStereo(int pstereo);
	int getPrefiltering() const;
	void setPrefiltering(int pprefiltering);

	void toStringCompute();

private:
	float outvolume;
	float *octoutl;
	float *octoutr;
	int Pvolume;	//Volumul or E/R
	int Ppanning;	//Panning
	int Plrcross;	// L/R Mixing
	int Pdrive;		//the input amplification
	int Plevel;		//the ouput amplification
	int Ptype;		//Distorsion type
	int Pnegate;	//if the input is negated
	int Plpf;		//lowpass filter
	int Phpf;		//highpass filter
	int Pstereo;	//0=mono,1=stereo
	int Pprefiltering;	//if you want to do the filtering before the distorsion
	int Poctave;	//mix sub octave

	//Parametrii reali
	float panning, lrcross, octave_memoryl, togglel, octave_memoryr,toggler,octmix;
	AnalogFilter *lpfl, *lpfr, *hpfl, *hpfr, *blockDCl, *blockDCr, *DCl, *DCr;
	WaveShaper *dwshapel, *dwshaper;
};

}
#endif /* YRODISTORTION_H_ */
