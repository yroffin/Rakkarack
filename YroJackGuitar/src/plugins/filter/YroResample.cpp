/*
 * YroResample.cpp
 *
 *  Created on: 7 mai 2012
 *      Author: yannick
 */

#include "YroResample.h"

YroResample::YroResample(int type) {
	statel = src_new(type, 1, &errorl);
	stater = src_new(type, 1, &errorr);
}

YroResample::~YroResample() {
}

void YroResample::cleanup() {
	src_reset(statel);
	src_reset(stater);
}

void YroResample::out(float *inl, float *inr, float *outl, float *outr,
		int frames, double ratio) {

	long int o_frames = lrint((double) frames * ratio);
	srcinfol.data_in = inl;
	srcinfol.input_frames = frames;
	srcinfol.data_out = outl;
	srcinfol.output_frames = o_frames;
	srcinfol.src_ratio = ratio;
	srcinfol.end_of_input = 0;

	srcinfor.data_in = inr;
	srcinfor.input_frames = frames;
	srcinfor.data_out = outr;
	srcinfor.output_frames = o_frames;
	srcinfor.src_ratio = ratio;
	srcinfor.end_of_input = 0;

	errorl = src_process(statel, &srcinfol);
	errorr = src_process(stater, &srcinfor);

}

void YroResample::mono_out(float *inl, float *outl, int frames, double ratio,
		int o_frames) {

	srcinfol.data_in = inl;
	srcinfol.input_frames = frames;
	srcinfol.data_out = outl;
	srcinfol.output_frames = o_frames;
	srcinfol.src_ratio = ratio;
	srcinfol.end_of_input = 0;

	errorl = src_process(statel, &srcinfol);

}
