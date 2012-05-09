/*
 * YroEffectPlugin.cpp
 *
 *  Created on: 5 mai 2012
 *      Author: yannick
 */

#include <plugins/YroEffectPlugin.h>

namespace std {

YroEffectPlugin::YroEffectPlugin(const char *_name) {
	name = strdup(_name);
	iPERIOD = helper->getIntegerPeriod();
	iSAMPLE_RATE = helper->getIntegerSampleRate();
	fPERIOD = helper->getFloatPeriod();
	fSAMPLE_RATE = helper->getFloatPeriod();
}

YroEffectPlugin::~YroEffectPlugin() {
	free((void *) name);
}

/**
 * getter
 */
float *YroEffectPlugin::getOutLeft() {
	return efxoutl;
}

float *YroEffectPlugin::getOutRight() {
	return efxoutr;
}

void YroEffectPlugin::setOutLeft(float *value) {
	efxoutl = value;
}

void YroEffectPlugin::setOutRight(float *value) {
	efxoutr = value;
}

} /* namespace std */
