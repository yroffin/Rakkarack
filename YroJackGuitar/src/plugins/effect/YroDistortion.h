/*
 * YroDistortion.h
 *
 *  Created on: 7 mai 2012
 *      Author: yannick
 */

#ifndef YRODISTORTION_H_
#define YRODISTORTION_H_

#include <plugins/YroEffectPlugin.h>
#include <plugins/filter/YroAnalogFilter.h>
#include <plugins/filter/YroWaveShaper.h>

namespace std {

class YroDistortion: public YroEffectPlugin {
public:
	YroDistortion();
	virtual ~YroDistortion();
	virtual void render(jack_nframes_t nframes, float *smpsl, float *smpsr);

	/**
	 * setter
	 */
	void cleanup ();
	void applyFilters(int iPeriod, float fPeriod);

	int getPdrive() const;
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
	YroAnalogFilter *lpfl, *lpfr, *hpfl, *hpfr, *blockDCl, *blockDCr, *DCl, *DCr;
	YroWaveShaper *dwshapel, *dwshaper;
};

}
#endif /* YRODISTORTION_H_ */
