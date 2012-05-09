/*
 * YroEffectGenerator.cpp
 *
 *  Created on: 5 mai 2012
 *      Author: yannick
 */

#include <math.h>
#include <string.h>
#include "YroEffectGenerator.h"

namespace std {

YroEffectGenerator::YroEffectGenerator() : YroEffectPlugin("simple generator plugin") {
	for (int i = 0; i < TABLE_SIZE; i++) {
		sine[i] = .1f * (float) sin(((double) i / (double) TABLE_SIZE) * M_PI * 1.2f);
	}
	left_phase = 0;
	right_phase = 0;
}

YroEffectGenerator::~YroEffectGenerator() {
}

void YroEffectGenerator::render(jack_nframes_t nframes, float *inLeft, float *inRight) {
	LOG->debug("render %d frames with [%s] - %08x/%08x/%08x/%08x",
			nframes,
			getName(),
			inLeft,
			inRight,
			efxoutl,
			efxoutr);
	for (jack_nframes_t i = 0; i < nframes; i++) {
		efxoutl[i] = sine[left_phase]; /* left */
		efxoutr[i] = sine[right_phase]; /* right */
		left_phase += 1;
		if (left_phase >= TABLE_SIZE)
			left_phase -= TABLE_SIZE;
		right_phase += 3; /* higher pitch so we can distinguish left and right. */
		if (right_phase >= TABLE_SIZE)
			right_phase -= TABLE_SIZE;
	}
}

} /* namespace std */
