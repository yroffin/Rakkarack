/*
 * YroWaveShaper.h
 *
 *  Created on: 7 mai 2012
 *      Author: yannick
 */

#ifndef YROWAVESHAPER_H_
#define YROWAVESHAPER_H_

#include <core/YroObject.h>
#include <core/YroParamHelper.h>
#include <plugins/YroEffectPlugin.h>
#include <plugins/filter/YroResample.h>

class YroWaveShaper: public std::YroObject {
public:
	YroWaveShaper();
	virtual ~YroWaveShaper();
	void waveshapesmps(jack_nframes_t n, float * smps, int type, int drive, int eff);
protected:
	void cleanup();

	int period_coeff;

	double u_up;
	double u_down;

	float dthresh; //dynamic threshold in compression waveshaper
	float dyno;
	float dynodecay; //dynamically control symmetry

	float compg; //used by compression distortion
	float cratio; //used by compression for hardness
	float tmpgain; // compression distortion temp variable
	float ncSAMPLE_RATE;
	float *temps;

	float R, P, Vgbias, Vsupp, Ip, Vmin, Vg, Vfactor, Vdyno; //Valve1 Modeling variables.
	float mu, V2bias, Is, Vg2, vfact, ffact, Vlv2out, V2dyno; //Valve2 variables

	YroResample *U_Resample;
	YroResample *D_Resample;
};

#endif /* YROWAVESHAPER_H_ */
