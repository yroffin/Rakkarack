/*
 * YroJackDriver.h
 *
 *  Created on: 4 mai 2012
 *      Author: yannick
 */

#ifndef YROJACKDRIVER_H_
#define YROJACKDRIVER_H_

#include <jack/jack.h>
#include "../core/YroObject.h"
#include "YroEffectFactory.h"
#include "YroAudioSampleFactory.h"

namespace std {

class YroJackDriver : public YroObject {
public:
	static YroJackDriver *instance() {
		if (__instance == 0) {
			__instance = new YroJackDriver();
		}
		return __instance;
	}
	virtual ~YroJackDriver();
	int initialize();
	virtual int process(jack_nframes_t nframes);
	virtual void jackShutdown();
private:
	YroJackDriver();
	static YroJackDriver *__instance;
	jack_port_t *input_port1, *input_port2;
	jack_port_t *output_port1, *output_port2;
	jack_client_t *client;
	/**
	 * processing part
	 */
	YroEffectFactory *effectFactory;
	YroAudioSampleFactory *audioSampleFactory;
};

} /* namespace std */
#endif /* YROJACKDRIVER_H_ */
