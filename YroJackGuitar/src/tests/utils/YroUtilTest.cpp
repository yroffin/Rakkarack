/*
 * YroUtilTest.cpp
 *
 *  Created on: 28 mai 2012
 *      Author: yannick
 */

#include "YroUtilTest.h"
#include <plugins/YroPreset.h>
#include <plugins/YroPresets.h>

using namespace test;

YroUtilTest::YroUtilTest() {
}

YroUtilTest::~YroUtilTest() {
}

void YroUtilTest::setUp() {
}

void YroUtilTest::tearDown() {
}

void YroUtilTest::testPresets() {
	new std::YroPresets(
			"Overdrive 1: 84, 64, 35, 56, 40, 0, 0, 6703, 21, 0, 0;"
			"Overdrive 2: 85, 64, 35, 29, 45, 1, 0, 25040, 21, 0, 0;"
			"Distorsion 1: 0, 64, 0, 87, 14, 6, 0, 3134, 157, 0, 1;"
			"Distorsion 2: 0, 64, 127, 87, 14, 0, 1, 3134, 102, 0, 0;"
			"Distorsion 3: 0, 64, 127, 127, 12, 13, 0, 5078, 56, 0, 1;"
			"Guitar Amp:  84, 64, 35, 63, 50, 2, 0, 824, 21, 0, 0;");
}

/* namespace std */
