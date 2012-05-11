/*
 * Distortion.h
 *
 *  Created on: 7 mai 2012
 *      Author: yannick
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
	AnalogFilter *lpfl, *lpfr, *hpfl, *hpfr, *blockDCl, *blockDCr, *DCl, *DCr;
	WaveShaper *dwshapel, *dwshaper;
};

}
#endif /* YRODISTORTION_H_ */
