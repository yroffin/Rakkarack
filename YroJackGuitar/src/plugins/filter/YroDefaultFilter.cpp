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
	fSAMPLE_RATE = helper->getFloatSampleRate();
	cSAMPLE_RATE = 1.0f / fSAMPLE_RATE;
}

YroDefaultFilter::~YroDefaultFilter() {
}

float YroDefaultFilter::getOutgain() const {
	return outgain;
}

void YroDefaultFilter::setOutgain(float outgain) {
	this->outgain = outgain;
}


