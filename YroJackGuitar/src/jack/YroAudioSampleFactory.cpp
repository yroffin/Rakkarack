/*
 * YroAudioSampleFactory.cpp
 *
 *  Created on: 5 mai 2012
 *      Author: yannick
 */

#include <stdio.h>
#include <string.h>

#include <core/YroInternalException.h>
#include <jack/YroAudioSampleFactory.h>

namespace std {

YroAudioSampleFactory *YroAudioSampleFactory::__instance = 0;

YroAudioSampleFactory::YroAudioSampleFactory() {
	LOG->info("Create instance for YroAudioSampleFactory ...");
}

YroAudioSampleFactory::~YroAudioSampleFactory() {
}

/**
 * release this audio sample
 */
void YroAudioSampleFactory::release(const char *name) {
	delete audioSamples[name];
	audioSamples.erase(name);
}

/**
 * allocate a named audio sample
 */
jack_default_audio_sample_t *YroAudioSampleFactory::allocate(jack_nframes_t nframes, const jack_default_audio_sample_t *origin, const char *target) {
	/**
	 * on sample could not be allocated twice, it must not be
	 * already in the map
	 */
	if(audioSamples.find(target) !=  audioSamples.end()) {
		/**
		 * this sample is already allocated
		 */
		LOG->debug("release target %s", target);
	}
	audioSamples[target] = new jack_default_audio_sample_t[nframes];
	return copy(nframes, origin, target);
}

/**
 * copy one raw audio sample chunk
 * on this name target audio sample
 */
jack_default_audio_sample_t *YroAudioSampleFactory::copy(jack_nframes_t nframes, const jack_default_audio_sample_t *origin, const char *target) {
	memcpy(get(target), origin, nframes * sizeof(jack_default_audio_sample_t));
	return audioSamples[target];
}

/**
 * simple raw copy
 */
jack_default_audio_sample_t *YroAudioSampleFactory::copy(jack_nframes_t nframes, const jack_default_audio_sample_t *origin, jack_default_audio_sample_t *target) {
	memcpy(target, origin, nframes * sizeof(jack_default_audio_sample_t));
	return target;
}

/**
 * simple getter on audio sample
 */
jack_default_audio_sample_t* YroAudioSampleFactory::get(const char* target) {
	if(audioSamples.find(target) ==  audioSamples.end()) {
		/**
		 * this sample is already allocated
		 */
		LOG->error("sample %s requested, but not found", target);
		return 0;
	}
	return audioSamples[target];
}

} /* namespace std */
