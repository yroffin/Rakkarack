/*
 * YroEffectPlugin.cpp
 *
 *  Created on: 5 mai 2012
 *      Author: yannick
 */

#include <string.h>
#include "YroEffectPlugin.h"

namespace std {

YroEffectPlugin::YroEffectPlugin(const char *_name) {
	name = strdup(_name);
}

YroEffectPlugin::~YroEffectPlugin() {
	delete name;
}

/**
 * getter
 */
float *YroEffectPlugin::getInLeft() {
	return inLeft;
}

float *YroEffectPlugin::getInRight() {
	return inRight;
}

float *YroEffectPlugin::getOutLeft() {
	return outLeft;
}

float *YroEffectPlugin::getOutRight() {
	return outRight;
}

void YroEffectPlugin::setInLeft(float *value) {
	inLeft = value;
}

void YroEffectPlugin::setInRight(float *value) {
	inRight = value;
}

void YroEffectPlugin::setOutLeft(float *value) {
	outLeft = value;
}

void YroEffectPlugin::setOutRight(float *value) {
	outRight = value;
}

} /* namespace std */
