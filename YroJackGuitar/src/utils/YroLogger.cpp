/*
 * YroLogger.cpp
 *
 *  Created on: 4 mai 2012
 *      Author: yannick
 */

#include <iostream>
#include <stdarg.h>
#include <stdio.h>

#include "YroLogger.h"

namespace std {

YroLogger *YroLogger::__instance;

YroLogger::YroLogger() {
	myLogger = Logger::getLogger("default");
}

YroLogger::~YroLogger() {
}

void YroLogger::error(const char *format, ...) {
	va_list ap;
	va_start(ap, format);
	memset(buffer,0,sizeof(buffer));
	vsnprintf(buffer, sizeof(buffer), format, ap);
	va_end(ap);
	LOG4CXX_ERROR(myLogger, (const char *) buffer);
}

void YroLogger::debug(const char *format, ...) {
	if(!myLogger->isDebugEnabled()) return;
	va_list ap;
	va_start(ap, format);
	memset(buffer,0,sizeof(buffer));
	vsnprintf(buffer, sizeof(buffer), format, ap);
	va_end(ap);
	LOG4CXX_DEBUG(myLogger, (const char *) buffer);
}

void YroLogger::info(const char *format, ...) {
	va_list ap;
	va_start(ap, format);
	memset(buffer,0,sizeof(buffer));
	vsnprintf(buffer, sizeof(buffer), format, ap);
	va_end(ap);
	LOG4CXX_INFO(myLogger, (const char *) buffer);
}

} /* namespace std */
