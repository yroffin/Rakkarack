/*
 * YroInternalException.cpp
 *
 *  Created on: 6 mai 2012
 *      Author: yannick
 */

#include <string.h>
#include <core/YroInternalException.h>

YroInternalException::YroInternalException(const char * _message) {
	message = strdup(_message);
}

