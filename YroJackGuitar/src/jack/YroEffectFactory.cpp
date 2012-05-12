/*
 * YroEffectFactory.cpp
 *
 *  Created on: 5 mai 2012
 *      Author: yannick
 */

#include <jack/YroEffectFactory.h>
#include <plugins/effect/Distortion.h>

namespace std {

YroEffectFactory *YroEffectFactory::__instance = 0;

/**
 * constructor
 */
YroEffectFactory::YroEffectFactory() {
	LOG->info("Create instance for YroEffectFactory ...");
	allocatedFrames = 0;
	loaded = 0;
	audioSampleFactory = YroAudioSampleFactory::instance();
}

/**
 * cleanup
 */
YroEffectFactory::~YroEffectFactory() {
}

/**
 * load behaviour
 */
void YroEffectFactory::load(const char *config) {
	if(loaded == 1) return;
	// TODO
	//addEffect("default#0",new YroEffectGenerator());
	addEffect("distortion#1",new Distortion());
}

YroEffectPlugin *YroEffectFactory::addEffect(const char *instance,YroEffectPlugin *effect) {
	effects.insert(pair<const char*,YroEffectPlugin *>(strdup(instance),effect));
	return effect;
}

YroEffectPlugin *YroEffectFactory::getEffect(const char *name) {
	return effects[name];
}

/**
 * initialize first step
 * allocate, or initialize the elements to be processed
 */
void YroEffectFactory::allocate(
		jack_nframes_t nframes,
		jack_default_audio_sample_t *in1,
		jack_default_audio_sample_t *in2,
		jack_default_audio_sample_t *out1,
		jack_default_audio_sample_t *out2) {
	if(allocatedFrames == 0 || allocatedFrames != nframes) {
		LOG->info((const char *) "allocation (or reallocation for %d) for frame size %d", allocatedFrames, nframes);
		bufferIn1 = audioSampleFactory->allocate(nframes,in1,"effects:inp1");
		bufferIn2 = audioSampleFactory->allocate(nframes,in2, "effects:inp2");
		bufferOut1 = audioSampleFactory->allocate(nframes,out1, "effects:out1");
		bufferOut2 = audioSampleFactory->allocate(nframes,out2, "effects:out2");
		audioSampleFactory->allocate(nframes,in1, "extern:left");
		audioSampleFactory->allocate(nframes,in2, "extern:right");
		allocatedFrames = nframes;
		return;
	}
	/**
	 * allocation is ok
	 * simple copy
	 */
	audioSampleFactory->copy(nframes,in1,"effects:inp1");
	audioSampleFactory->copy(nframes,in2,"effects:inp2");
	audioSampleFactory->copy(nframes,out1,"effects:out1");
	audioSampleFactory->copy(nframes,out2,"effects:out2");
}

/**
 * apply all effects
 */
int YroEffectFactory::render(
		jack_nframes_t nframes,
		jack_default_audio_sample_t *in1,
		jack_default_audio_sample_t *in2,
		jack_default_audio_sample_t *out1,
		jack_default_audio_sample_t *out2) {

	/**
	 * allocate buffers, only once
	 */
	allocate(nframes, in1, in2, out1, out2);

	jack_default_audio_sample_t *toProcess1 = audioSampleFactory->get("effects:inp1");
	jack_default_audio_sample_t *toProcess2 = audioSampleFactory->get("effects:inp2");
	jack_default_audio_sample_t *processed1 = audioSampleFactory->get("effects:out1");
	jack_default_audio_sample_t *processed2 = audioSampleFactory->get("effects:out2");
	jack_default_audio_sample_t *switchIt = 0;

	map<const char *,YroEffectPlugin *>::iterator effect;
	for(effect=effects.begin(); effect!=effects.end(); effect++) {
		YroEffectPlugin *data = effect->second;
		/**
		 * process the effect
		 */
		data->setOutLeft(processed1);
		data->setOutRight(processed2);
		data->render(nframes, toProcess1, toProcess2);
		/**
		 * switch
		 */
		switchIt = toProcess1;
		toProcess1 = processed1;
		processed1 = switchIt;
		switchIt = toProcess2;
		toProcess2 = processed2;
		processed2 = switchIt;
	}

	/**
	 * switch
	 */
	switchIt = toProcess1;
	toProcess1 = processed1;
	processed1 = switchIt;
	switchIt = toProcess2;
	toProcess2 = processed2;
	processed2 = switchIt;

	/**
	 * validate tranformation
	 */
	processed1 = audioSampleFactory->copy(nframes,processed1,out1);
	processed2 = audioSampleFactory->copy(nframes,processed2,out2);

	/**
	 * produce output audio sample
	 * and advertise the window system
	 */
	audioSampleFactory->copy(nframes,processed1,"extern:left");
	audioSampleFactory->copy(nframes,processed2,"extern:right");

	return 0;
}

} /* namespace std */
