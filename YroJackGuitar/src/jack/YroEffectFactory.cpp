/*
 * YroEffectFactory.cpp
 *
 *  Created on: 5 mai 2012
 *      Author: yannick
 */

#include "YroEffectFactory.h"

namespace std {

YroEffectFactory::YroEffectFactory() {
	effects.push_back(new YroEffectGenerator());
	allocatedFrames = 0;
	audioSampleFactory = new YroAudioSampleFactory();
}

YroEffectFactory::~YroEffectFactory() {
	delete audioSampleFactory;
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
	if(allocatedFrames == 0) {
		LOG->info((const char *) "allocation for frame size %d",nframes);
		bufferIn1 = audioSampleFactory->allocate(nframes,in1);
		bufferIn2 = audioSampleFactory->allocate(nframes,in2);
		bufferOut1 = audioSampleFactory->allocate(nframes,out1);
		bufferOut2 = audioSampleFactory->allocate(nframes,out2);
		allocatedFrames = nframes;
		return;
	}
	if(allocatedFrames != nframes) {
		LOG->info((const char *) "re-allocation for frame size %d (before it as %d)",nframes,allocatedFrames);
		audioSampleFactory->release(bufferIn1);
		audioSampleFactory->release(bufferIn2);
		audioSampleFactory->release(bufferOut1);
		audioSampleFactory->release(bufferOut2);
		bufferIn1 = audioSampleFactory->allocate(nframes,in1);
		bufferIn2 = audioSampleFactory->allocate(nframes,in2);
		bufferOut1 = audioSampleFactory->allocate(nframes,out1);
		bufferOut2 = audioSampleFactory->allocate(nframes,out2);
		allocatedFrames = nframes;
		return;
	}
	/**
	 * allocation is ok
	 * simple copy
	 */
	audioSampleFactory->copy(nframes,in1,bufferIn1);
	audioSampleFactory->copy(nframes,in2,bufferIn2);
	audioSampleFactory->copy(nframes,out1,bufferOut1);
	audioSampleFactory->copy(nframes,out2,bufferOut2);
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
	LOG->debug((const char *) "check allocation for frames %d",nframes);
	allocate(nframes, in1, in2, out1, out2);

	jack_default_audio_sample_t *toProcess1 = bufferIn1;
	jack_default_audio_sample_t *toProcess2 = bufferIn2;
	jack_default_audio_sample_t *processed1 = bufferOut1;
	jack_default_audio_sample_t *processed2 = bufferOut2;
	jack_default_audio_sample_t *switchIt = 0;

	vector<YroEffectPlugin *>::iterator effect;
	for(effect=effects.begin(); effect!=effects.end(); effect++) {
		YroEffectPlugin *data = *effect;
		LOG->debug("check allocation for plugin [%s]", data->getName());
		/**
		 * process the effect
		 */
		data->setInLeft(toProcess1);
		data->setInRight(toProcess2);
		data->setOutLeft(processed1);
		data->setOutRight(processed2);
		LOG->debug("render for plugin [%s]", data->getName());
		data->render(nframes);
		/**
		 * switch
		 */
		LOG->debug("switch data from plugin [%s]", data->getName());
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
	LOG->debug("switch data");
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

	return 0;
}

} /* namespace std */
