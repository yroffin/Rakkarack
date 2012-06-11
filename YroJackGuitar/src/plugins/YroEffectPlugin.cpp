/*
 * YroEffectPlugin.cpp
 *
 *  Created on: 5 mai 2012
 *      Author: yannick
 */

#include <plugins/YroEffectPlugin.h>

using namespace std;

YroEffectPlugin::YroEffectPlugin(const char *_name) : YroRawEffectPlugin(_name) {
}

/**
 * default constructor
 * @param const char *_name, name of this effect
 * @param const char *_preset, preset value
 */
YroEffectPlugin::YroEffectPlugin(const char *_name, const char *_presets) : YroRawEffectPlugin(_name,_presets)  {
}

YroEffectPlugin::~YroEffectPlugin() {
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
int YroEffectPlugin::getBehaviour() {
	return _applyEffectOnInAndCopyToOut;
}
