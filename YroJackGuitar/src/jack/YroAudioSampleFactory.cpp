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

#include <core/YroInternalException.h>
#include <jack/YroAudioSampleFactory.h>

namespace std {

YroAudioSampleFactory *YroAudioSampleFactory::__instance = 0;

YroAudioSampleFactory::YroAudioSampleFactory() {
	LOG->info("Create instance for YroAudioSampleFactory ...");
}

YroAudioSampleFactory::~YroAudioSampleFactory() {
	/**
	 * TODO release all samples
	 */
}

/**
 * release this audio sample
 */
void YroAudioSampleFactory::release(const char *name) {
	delete audioSamples[name];
	audioSamples.erase(name);
	allocatedSamples.erase(name);
}

/**
 * allocate a named audio sample
 * check with current allocated memory to change or not
 * this allocation
 * @param jack_nframes_t nframes the current requested size
 * @param jack_default_audio_sample_t *origin the initial copied values on this allocated memory
 * @param const char *target the logicial name of this sampe
 */
jack_default_audio_sample_t *YroAudioSampleFactory::allocate(jack_nframes_t nframes, const jack_default_audio_sample_t *origin, const char *target) {
	/**
	 * on sample could not be allocated twice, it must not be
	 * already in the map
	 */
	if(audioSamples.find(target) !=  audioSamples.end()) {
		/**
		 * this sample is already allocated
		 * reallocate it if size change
		 */
		if(allocatedSamples[target] != nframes) {
			LOG->info("release target [%s] for reallocate to %d (now it's %d)", target, nframes, allocatedSamples[target]);
			release(target);
		} else {
			return audioSamples[target];
		}
	}
	LOG->info("allocate target [%s] with 0x%08x (size %d)", target, origin, nframes);
	allocatedSamples[target] = nframes;
	audioSamples[target] = new jack_default_audio_sample_t[nframes];
	return copy(nframes, origin, target);
}

/**
 * copy one raw audio sample chunk
 * on this name target audio sample
 */
jack_default_audio_sample_t *YroAudioSampleFactory::copy(jack_nframes_t nframes, const jack_default_audio_sample_t *origin, const char *target) {
	/**
	 * size check
	 */
	if(allocatedSamples[target] != nframes) {
		/**
		 * critic problem with size of memory
		 */
		throw new YroInternalException("Internal error, size %d doesn't match %d", allocatedSamples[target], nframes);
	}
	if(origin == 0) {
		memset(get(target), 0, nframes * sizeof(jack_default_audio_sample_t));
	} else {
		memcpy(get(target), origin, nframes * sizeof(jack_default_audio_sample_t));
	}
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
		throw new YroInternalException("sample %s requested, but not found", target);
	}
	return audioSamples[target];
}

} /* namespace std */
