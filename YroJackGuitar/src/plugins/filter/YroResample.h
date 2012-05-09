/*
 * YroResample.h
 *
 *  Created on: 7 mai 2012
 *      Author: yannick
 */

#ifndef YRORESAMPLE_H_
#define YRORESAMPLE_H_

#include <samplerate.h>
#include <core/YroObject.h>

class YroResample: public std::YroObject {
public:
	YroResample(int type);
	virtual ~YroResample();
	void mono_out(float *inl, float *outl, int frames, double ratio,
			int o_frames);
protected:
	void cleanup();
	void out(float *inl, float *inr, float *outl, float *outr, int frames,
			double ratio);
private:
	// ratio Equal to output_sample_rate / input_sample_rate.
	SRC_DATA srcinfor;
	SRC_DATA srcinfol;
	int errorl, errorr;
	SRC_STATE *statel;
	SRC_STATE *stater;
};

#endif /* YRORESAMPLE_H_ */
