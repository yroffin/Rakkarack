/*
 * YroAudioSampleFactory.cpp
 *
 *  Created on: 5 mai 2012
 *      Author: yannick
 */

#include <stdio.h>
#include <string.h>
#include "YroAudioSampleFactory.h"

namespace std {

YroAudioSampleFactory::YroAudioSampleFactory() {
}

YroAudioSampleFactory::~YroAudioSampleFactory() {
}

void YroAudioSampleFactory::release(jack_default_audio_sample_t *value) {
	delete value;
}

jack_default_audio_sample_t *YroAudioSampleFactory::allocate(jack_nframes_t nframes, jack_default_audio_sample_t *value) {
	jack_default_audio_sample_t *target = new jack_default_audio_sample_t[nframes];
	return copy(nframes, value, target);
}

jack_default_audio_sample_t *YroAudioSampleFactory::copy(jack_nframes_t nframes, jack_default_audio_sample_t *origin, jack_default_audio_sample_t *target) {
	memcpy(target, origin, nframes);
	return target;
}

} /* namespace std */
