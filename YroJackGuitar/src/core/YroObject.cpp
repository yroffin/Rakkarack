/*
 * YroObject.cpp
 *
 *  Created on: 4 mai 2012
 *      Author: yannick
 */

#include "YroObject.h"

namespace std {

YroObject::YroObject() {
	LOG = YroLogger::instance();
}

YroObject::~YroObject() {
}

} /* namespace std */
