/*
 * YroProperties.cpp
 *
 *  Created on: 4 mai 2012
 *      Author: yannick
 */

#include <string.h>
#include "YroProperties.h"

namespace std {

YroProperties *YroProperties::__instance;

YroProperties::YroProperties() {
}

YroProperties::~YroProperties() {
}

int YroProperties::get(const char *entry, int &value, int defaultValue) {
	value = defaultValue;
	return 0;
}
int YroProperties::get(const char *entry, float &value, float defaultValue) {
	value = defaultValue;
	return 0;
}
int YroProperties::get(const char *entry, double &value, double defaultValue) {
	value = defaultValue;
	return 0;
}
int YroProperties::get(const char *entry, char *value, const char *defaultValue, int maxSize) {
	strcpy(value, defaultValue);
	return 0;
}

} /* namespace std */
