/*
 * YroDefaultFilter.cpp
 *
 *  Created on: 7 mai 2012
 *      Author: yannick
 */

#include "YroDefaultFilter.h"

YroDefaultFilter::YroDefaultFilter() {
	iPERIOD = helper->getIntegerPeriod();
	iSAMPLE_RATE = helper->getIntegerSampleRate();
	fPERIOD = helper->getFloatPeriod();
	fSAMPLE_RATE = helper->getFloatPeriod();
}

YroDefaultFilter::~YroDefaultFilter() {
}

