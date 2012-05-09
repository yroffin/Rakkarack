/*
 * YroAnalogFilter.h
 *
 *  Created on: 7 mai 2012
 *      Author: yannick
 */

#ifndef YROANALOGFILTER_H_
#define YROANALOGFILTER_H_

#include "YroDefaultFilter.h"

class YroAnalogFilter: public YroDefaultFilter {
public:
	YroAnalogFilter(unsigned char Ftype, float Ffreq, float Fq,
			unsigned char Fstages);
	virtual ~YroAnalogFilter();

	/**
	 * default function
	 */
	void filterout(int iPeriod, float fPeriod, float * smp);
	void setfreq(float frequency);
	void setfreq_and_q(float frequency, float q_);
	void setq(float q_);
	void setgain(float dBgain);
	void cleanup();
protected:
	float filterout_s(float smp);
	void setSR(unsigned int SR);
	void settype(int type_);
	void setstages(int stages_);
	void reversecoeffs();
	float H(float freq); //Obtains the response for a given frequency
private:
	struct fstage {
		float c1, c2;
	} x[MAX_FILTER_STAGES + 1], y[MAX_FILTER_STAGES + 1], oldx[MAX_FILTER_STAGES
			+ 1], oldy[MAX_FILTER_STAGES + 1];

	void singlefilterout(int iPeriod, float fPeriod, float * smp, fstage & x, fstage & y, float * c,
			float * d);
	float singlefilterout_s(float smp, fstage & x, fstage & y, float * c,
			float * d);

	void computefiltercoefs();
	int type; //The type of the filter (LPF1,HPF1,LPF2,HPF2...)
	int stages; //how many times the filter is applied (0->1,1->2,etc.)
	int order; //the order of the filter (number of poles)
	int needsinterpolation, firsttime;
	int abovenq; //this is 1 if the frequency is above the nyquist
	int oldabovenq; //if the last time was above nyquist (used to see if it needs interpolation)

	float freq; //Frequency given in Hz
	float q; //Q factor (resonance or Q factor)
	float gain; //the gain of the filter (if are shelf/peak) filters

	float c[3], d[3]; //coefficients

	float oldc[3], oldd[3]; //old coefficients(used only if some filter paremeters changes very fast, and it needs interpolation)

	float xd[3], yd[3]; //used if the filter is applied more times
};

#endif /* YROANALOGFILTER_H_ */
