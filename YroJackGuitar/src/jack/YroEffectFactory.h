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

#ifndef YROEFFECTFACTORY_H_
#define YROEFFECTFACTORY_H_

#include <jack/jack.h>
#include <jack/YroAudioSampleFactory.h>
#include <plugins/YroEffectPlugin.h>
#include <plugins/YroEffectGenerator.h>
#include <plugins/ampli/YroAmpli.h>
#include <plugins/distortion/Distortion.h>
#include <plugins/noisegate/Expander.h>
#include <plugins/effect/Chorus.h>
#include <plugins/effect/Echo.h>
#include <plugins/analyze/YroScope.h>

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
	void unload(const char *name);
	int render(jack_default_audio_sample_t *int1,
			jack_default_audio_sample_t *in2, jack_default_audio_sample_t *out1,
			jack_default_audio_sample_t *out2);
	void allocate(jack_default_audio_sample_t *in1,
			jack_default_audio_sample_t *in2, jack_default_audio_sample_t *out1,
			jack_default_audio_sample_t *out2);
	YroEffectPlugin *addEffect(const char *instance,YroEffectPlugin *effect);
	YroEffectPlugin *getEffect(const char *name);
	map<const char*, YroEffectPlugin*, cmp_str> getEffects() const;
private:
	YroEffectFactory();
	static YroEffectFactory *__instance;
	int loaded;
	map<const char *,YroEffectPlugin *, cmp_str> effects;
	YroAudioSampleFactory *audioSampleFactory;
	jack_nframes_t currentFrames;
	jack_nframes_t allocatedFrames;
	jack_default_audio_sample_t *bufferIn1;
	jack_default_audio_sample_t *bufferIn2;
	jack_default_audio_sample_t *bufferOut1;
	jack_default_audio_sample_t *bufferOut2;
};

} /* namespace std */
#endif /* YROEFFECTFACTORY_H_ */
