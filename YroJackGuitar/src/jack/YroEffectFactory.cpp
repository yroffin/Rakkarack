/*
 YroJackGuitar - a software synthesizer based on excelent work
 of Rakkarack team

 Copyright (C) 2002-2005 Yannick Roffin
 Author: Yannick Roffin

 This program is free software; you can redistribute it and/or modify
 it under the terms of version 2 of the GNU General Public License
 as published by the Free Software Foundation.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License (version 2) for more details.

 You should have received a copy of the GNU General Public License (version 2)
 along with this program; if not, write to the Free Software Foundation,
 Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
 */

#include <jack/YroEffectFactory.h>

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
	addEffect("#1.ampli",new YroAmpli());
	//addEffect("default#1",new YroEffectGenerator());
	addEffect("#2.distortion",new Distortion());
	addEffect("#3.expander",new Expander());
	addEffect("#4.ampli",new YroAmpli());
	addEffect("#5.scope",new YroScope());
}

/**
 * unload effect
 */
void YroEffectFactory::unload(const char *name) {
	/**
	 * TODO delete effect named "name"
	 */
	map<const char *,YroEffectPlugin *>::iterator effect;
	for(effect=effects.begin(); effect!=effects.end(); effect++) {
		YroEffectPlugin *data = effect->second;
		delete data;
	}
	effects.clear();
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
 * used global currentFrames variable
 */
void YroEffectFactory::allocate(
		jack_default_audio_sample_t *in1,
		jack_default_audio_sample_t *in2,
		jack_default_audio_sample_t *out1,
		jack_default_audio_sample_t *out2) {
	if(allocatedFrames == 0 || allocatedFrames != currentFrames) {
		bufferIn1 = audioSampleFactory->allocate(currentFrames,in1,"effects:inp1");
		bufferIn2 = audioSampleFactory->allocate(currentFrames,in2, "effects:inp2");
		bufferOut1 = audioSampleFactory->allocate(currentFrames,out1, "effects:out1");
		bufferOut2 = audioSampleFactory->allocate(currentFrames,out2, "effects:out2");
		allocatedFrames = currentFrames;
		return;
	}
	/**
	 * allocation is ok
	 * simple copy
	 */
	audioSampleFactory->copy(currentFrames,in1,"effects:inp1");
	audioSampleFactory->copy(currentFrames,in2,"effects:inp2");
	audioSampleFactory->copy(currentFrames,out1,"effects:out1");
	audioSampleFactory->copy(currentFrames,out2,"effects:out2");
}

/**
 * apply all effects
 */
int YroEffectFactory::render(
		jack_default_audio_sample_t *in1,
		jack_default_audio_sample_t *in2,
		jack_default_audio_sample_t *out1,
		jack_default_audio_sample_t *out2) {
	currentFrames = YroParamHelper::instance()->getIntegerPeriod();

	/**
	 * allocate buffers, only once
	 */
	allocate(in1, in2, out1, out2);

	jack_default_audio_sample_t *toProcess1 = audioSampleFactory->allocate(currentFrames, 0, "effects:inp1");
	jack_default_audio_sample_t *toProcess2 = audioSampleFactory->allocate(currentFrames, 0, "effects:inp2");
	jack_default_audio_sample_t *processed1 = audioSampleFactory->allocate(currentFrames, 0, "effects:out1");
	jack_default_audio_sample_t *processed2 = audioSampleFactory->allocate(currentFrames, 0, "effects:out2");
	jack_default_audio_sample_t *switchIt = 0;

	map<const char *,YroEffectPlugin *>::iterator effect;
	for(effect=effects.begin(); effect!=effects.end(); effect++) {
		YroEffectPlugin *data = effect->second;
		/**
		 * effect can be ignore
		 */
		if(data->getActive()==0) continue;

		/**
		 * process the effect
		 */
		data->setOutLeft(processed1);
		data->setOutRight(processed2);
		data->render(toProcess1, toProcess2);
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
	 * validate tranformation on
	 * output
	 */
	processed1 = audioSampleFactory->copy(currentFrames,processed1,out1);
	processed2 = audioSampleFactory->copy(currentFrames,processed2,out2);

	return 0;
}

map<const char*, YroEffectPlugin*, cmp_str> YroEffectFactory::getEffects() const {
	return effects;
}

} /* namespace std */
