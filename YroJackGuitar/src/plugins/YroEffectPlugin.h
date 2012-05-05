/*
 * YroEffectPlugin.h
 *
 *  Created on: 5 mai 2012
 *      Author: yannick
 */

#ifndef YROEFFECTPLUGIN_H_
#define YROEFFECTPLUGIN_H_

#include <jack/jack.h>
#include "../core/YroObject.h"

namespace std {

class YroEffectPlugin: public std::YroObject {
public:
	YroEffectPlugin(const char *name);
	virtual ~YroEffectPlugin();
	/**
	 * processing
	 */
	virtual void render(jack_nframes_t nframes) {}

	/**
	 * getter
	 */
	float *getInLeft();
	float *getInRight();
	float *getOutLeft();
	float *getOutRight();
	void setInLeft(float *value);
	void setInRight(float *value);
	void setOutLeft(float *value);
	void setOutRight(float *value);
	/**
	 * inline
	 */
	const char *getName() {return name;}
protected:
	float *inLeft;
	float *inRight;
	float *outLeft;
	float *outRight;
	const char *name;
};

} /* namespace std */
#endif /* YROEFFECTPLUGIN_H_ */
