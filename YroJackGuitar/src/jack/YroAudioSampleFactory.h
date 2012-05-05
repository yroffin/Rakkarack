/*
 * YroAudioSampleFactory.h
 *
 *  Created on: 5 mai 2012
 *      Author: yannick
 */

#ifndef YROAUDIOSAMPLEFACTORY_H_
#define YROAUDIOSAMPLEFACTORY_H_

#include <jack/jack.h>
#include "../core/YroObject.h"

namespace std {

class YroAudioSampleFactory: public std::YroObject {
public:
	YroAudioSampleFactory();
	virtual ~YroAudioSampleFactory();
	void release(jack_default_audio_sample_t *value);
	jack_default_audio_sample_t *allocate(jack_nframes_t nframes, jack_default_audio_sample_t *value);
	jack_default_audio_sample_t *copy(jack_nframes_t nframes, jack_default_audio_sample_t *origin, jack_default_audio_sample_t *target);
};

} /* namespace std */
#endif /* YROAUDIOSAMPLEFACTORY_H_ */
