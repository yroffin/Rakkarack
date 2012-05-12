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

	/**
	 * setter
	 */
	void cleanup ();
	void applyFilters();

	enum functions {
		_preset,
		_volume,
		_type,
		_lrcross,
		_drive,
		_level,
		_negate,
		_prefiltering,
		_stereo,
		_panning,
		_octave,
		_lpf,
		_hpf
	};

	int  get0() {return getPreset();};
	void set0(int value) {setPreset(value);};
	int  get2() {return getPtype();};
	void set2(int value) {setPtype(value);};
	int  get3() {return getPlrcross();};
	void set3(int value) {setPlrcross(value);};
	int  get4() {return getPdrive();};
	void set4(int value) {setPdrive(value);};
	int  get5() {return getPlevel();};
	void set5(int value) {setPlevel(value);};
	int  get6() {return getPnegate();};
	void set6(int value) {setPnegate(value);};
	int  get7() {return getPprefiltering();};
	void set7(int value) {setPprefiltering(value);};
	int  get8() {return getPstereo();};
	void set8(int value) {setPstereo(value);};
	int  get9() {return getPpanning();};
	void set9(int value) {setPpanning(value);};
	int  get10() {return getPoctave();};
	void set10(int value) {setPoctave(value);};
	int  get11() {return getPlpf();};
	void set11(int value) {setPlpf(value);};
	int  get12() {return getPhpf();};
	void set12(int value) {setPhpf(value);};

	int getPdrive();
	void setPdrive(int pdrive);
	int getPreset() const;
	void setPreset(int ppreset);
	int getPhpf() const;
	void setPhpf(int phpf);
	int getPlevel() const;
	void setPlevel(int plevel);
	int getPlpf() const;
	void setPlpf(int plpf);
	int getPoctave() const;
	void setPoctave(int poctave);
	int getPpanning() const;
	void setPpanning(int ppanning);
	int getPvolume() const;
	void setPvolume(int pvolume);
	int getPlrcross() const;
	void setPlrcross(int plrcross);
	int getPtype() const;
	void setPtype(int ptype);
	int getPnegate() const;
	void setPnegate(int pnegate);
	int getPstereo() const;
	void setPstereo(int pstereo);
	int getPprefiltering() const;
	void setPprefiltering(int pprefiltering);

private:
	float outvolume;
	float *octoutl;
	float *octoutr;
	int Ppreset;
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
