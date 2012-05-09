/*
 * YroAudioSampleFactory.h
 *
 *  Created on: 5 mai 2012
 *      Author: yannick
 */

#include <stdio.h>
#include <string.h>
#include <jack/jack.h>
#include <core/YroObject.h>

#ifndef YROAUDIOSAMPLEFACTORY_H_
#define YROAUDIOSAMPLEFACTORY_H_

namespace std {

class YroAudioSampleFactory: public std::YroObject {
public:
	static YroAudioSampleFactory *instance() {
		if (__instance == 0) {
			__instance = new YroAudioSampleFactory();
		}
		return __instance;
	}
	virtual ~YroAudioSampleFactory();
	jack_default_audio_sample_t *allocate(jack_nframes_t nframes, const jack_default_audio_sample_t *value, const char *target);
	jack_default_audio_sample_t *copy(jack_nframes_t nframes, const jack_default_audio_sample_t *origin, const char *target);
	jack_default_audio_sample_t *copy(jack_nframes_t nframes, const jack_default_audio_sample_t *origin, jack_default_audio_sample_t *target);
	jack_default_audio_sample_t *get(const char *target);
private:
	YroAudioSampleFactory();
	static YroAudioSampleFactory *__instance;
	void release(const char *name);
	/**
	 * all sample in system are stored
	 * into this map, each on is named and could be used every where
	 */
	map<const char *, jack_default_audio_sample_t *, cmp_str> audioSamples;
};

} /* namespace std */
#endif /* YROAUDIOSAMPLEFACTORY_H_ */
