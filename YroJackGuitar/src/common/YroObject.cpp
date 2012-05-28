/*
 * YroObject.cpp
 *
 *  Created on: 28 mai 2012
 *      Author: yannick
 */

#include "YroObject.h"

using namespace std;

YroObject::YroObject() {
	LOG = YroLogger::instance();
	helper = YroParamHelper::instance();
}

YroObject::~YroObject() {
}

void YroObject::toStringCat(const char *format, ...) {
	va_list ap;
	va_start(ap, format);
	memset(_toStringFormat,0,sizeof(_toStringFormat));
	vsnprintf(_toStringFormat, sizeof(_toStringFormat), format, ap);
	va_end(ap);
	strcat(_toString, _toStringFormat);
}

/* namespace std */
