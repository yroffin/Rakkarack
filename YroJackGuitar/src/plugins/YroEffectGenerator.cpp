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
		sine[i] = 0.2f * (float) sin(((double) i / (double) TABLE_SIZE) * M_PI * 2.0f);
	}
	left_phase = 0;
	right_phase = 0;
}

YroEffectGenerator::~YroEffectGenerator() {
}

void YroEffectGenerator::render(jack_nframes_t nframes) {
	LOG->debug("render %d frames with [%s] - %08x/%08x/%08x/%08x",
			nframes,
			getName(),
			inLeft,
			inRight,
			outLeft,
			outRight);
	for (jack_nframes_t i = 0; i < nframes; i++) {
		outLeft[i] = sine[left_phase]; /* left */
		outRight[i] = sine[right_phase]; /* right */
		left_phase += 1;
		if (left_phase >= TABLE_SIZE)
			left_phase -= TABLE_SIZE;
		right_phase += 3; /* higher pitch so we can distinguish left and right. */
		if (right_phase >= TABLE_SIZE)
			right_phase -= TABLE_SIZE;
	}
}

} /* namespace std */
