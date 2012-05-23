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

#include <jack/jack.h>
#include <jack/YroEffectFactory.h>
#include <jack/YroAudioSampleFactory.h>

#ifndef YROJACKDRIVER_H_
#define YROJACKDRIVER_H_

namespace std {

class YroJackDriver : public YroObject {
public:
	static YroJackDriver *instance() {
		if (__instance == 0) {
			__instance = new YroJackDriver();
		}
		return (__instance);
	}
	virtual ~YroJackDriver();
	int initialize();
	virtual int process(jack_nframes_t nframes);
	virtual void jackShutdown();
	YroEffectFactory* getEffectFactory() const;
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
