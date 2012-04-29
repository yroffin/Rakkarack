/*
 * PluginEffect.cpp
 *
 *  Created on: 27 avr. 2012
 *      Author: yannick
 */

#include "PluginEffect.h"

PluginEffect::PluginEffect() {
	efxoutl = 0;
	efxoutr = 0;
	outvolume = 0;
	Pchange = 0;
	Ppreset = 0;
	memset((void *) ((this->fileName)), 0, 128);
}

PluginEffect::~PluginEffect() {
	// TODO Auto-generated destructor stub
}

void PluginEffect::out(float * smpsl, float * smpsr) {
	// TODO Auto-generated destructor stub
}
