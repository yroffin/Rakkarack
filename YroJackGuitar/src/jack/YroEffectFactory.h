/*
 * YroEffectFactory.h
 *
 *  Created on: 5 mai 2012
 *      Author: yannick
 */

#ifndef YROEFFECTFACTORY_H_
#define YROEFFECTFACTORY_H_

#include <jack/jack.h>
#include <core/YroObject.h>
#include <jack/YroAudioSampleFactory.h>
#include <plugins/YroEffectPlugin.h>
#include <plugins/YroEffectGenerator.h>
#include <plugins/effect/YroDistortion.h>

namespace std {

class YroEffectFactory: public std::YroObject {
public:
	static YroEffectFactory *instance() {
		if (__instance == 0) {
			__instance = new YroEffectFactory();
		}
		return __instance;
	}
	virtual ~YroEffectFactory();
	/**
	 * render all effects
	 */
	void load(const char *config);
	int render(jack_nframes_t nframes, jack_default_audio_sample_t *int1,
			jack_default_audio_sample_t *in2, jack_default_audio_sample_t *out1,
			jack_default_audio_sample_t *out2);
	void allocate(jack_nframes_t nframes, jack_default_audio_sample_t *in1,
			jack_default_audio_sample_t *in2, jack_default_audio_sample_t *out1,
			jack_default_audio_sample_t *out2);
	YroEffectPlugin *addEffect(const char *instance,YroEffectPlugin *effect);
	YroEffectPlugin *getEffect(const char *name);
private:
	YroEffectFactory();
	static YroEffectFactory *__instance;
	int loaded;
	map<const char *,YroEffectPlugin *, cmp_str> effects;
	YroAudioSampleFactory *audioSampleFactory;
	jack_nframes_t allocatedFrames;
	jack_default_audio_sample_t *bufferIn1;
	jack_default_audio_sample_t *bufferIn2;
	jack_default_audio_sample_t *bufferOut1;
	jack_default_audio_sample_t *bufferOut2;
};

} /* namespace std */
#endif /* YROEFFECTFACTORY_H_ */
