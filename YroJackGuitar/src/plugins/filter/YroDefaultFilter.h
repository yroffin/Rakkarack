/*
 * YroDefaultFilter.h
 *
 *  Created on: 7 mai 2012
 *      Author: yannick
 */

#ifndef YRODEFAULTFILTER_H_
#define YRODEFAULTFILTER_H_

#include <core/YroObject.h>
#include <core/YroParamHelper.h>
#include <plugins/YroEffectPlugin.h>

class YroDefaultFilter : public std::YroObject {
public:
	YroDefaultFilter();
	virtual ~YroDefaultFilter();
	float getOutgain() const;
	void setOutgain(float outgain);
	/**
	 * default function
	 */
	virtual void filterout (int iPeriod, float fPeriod, float * smp) { };
	virtual void setfreq (float frequency) {};
	virtual void setfreq_and_q (float frequency, float q_) { };
	virtual void setq (float q_) { };
	virtual void setgain (float dBgain) {};
	virtual void cleanup() {};
protected:
	float outgain;

	int   iPERIOD;
	int   iSAMPLE_RATE;
	float fPERIOD;
	float fSAMPLE_RATE;
	float cSAMPLE_RATE;
};

#endif /* YRODEFAULTFILTER_H_ */
