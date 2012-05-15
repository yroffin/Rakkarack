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
private:
	YroAudioSampleFactory();
	static YroAudioSampleFactory *__instance;
	void release(const char *name);
	jack_default_audio_sample_t *get(const char *target);
	/**
	 * all sample in system are stored
	 * into this map, each on is named and could be used every where
	 */
	map<const char *, jack_default_audio_sample_t *, cmp_str> audioSamples;
	map<const char *, jack_nframes_t, cmp_str> allocatedSamples;
};

} /* namespace std */
#endif /* YROAUDIOSAMPLEFACTORY_H_ */
