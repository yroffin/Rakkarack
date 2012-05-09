/*
 * YroParamHelper.cpp
 *
 *  Created on: 7 mai 2012
 *      Author: yannick
 */

#include "YroParamHelper.h"

YroParamHelper *YroParamHelper::__instance = 0;

YroParamHelper::YroParamHelper() {
	LOG->info("Create instance for YroParamHelper ...");
	waveshapeResampling = 0;
	waveshapeUpQuality = 4;
	waveshapeDownQuality = 2;
}

YroParamHelper::~YroParamHelper() {
}

