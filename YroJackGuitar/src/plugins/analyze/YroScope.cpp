/*
 YroJackGuitar - a software synthesizer based on excelent work
 of Rakkarack team

 Copyright (C) 2002-2005 Yannick Roffin
 Author: Yannick Roffin

 This program is free software; you can redistribute it and/or modify
 it under the terms of version 2 of the GNU General Public License
 as published by the Free Software Foundation.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License (version 2) for more details.

 You should have received a copy of the GNU General Public License (version 2)
 along with this program; if not, write to the Free Software Foundation,
 Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
 */

#include <plugins/analyze/YroScope.h>

using namespace std;

YroScope::YroScope() : YroEffectPlugin("YroScope") {
	/**
	 * fix klass attribute
	 */
	klass = YroEffectPlugin::_YroScope;
	/**
	 * internal YroScope buffer is based
	 * on sample rate size
	 */
	left = audioSampleFactory->allocate(iSAMPLE_RATE,0,"extern:left");
	right = audioSampleFactory->allocate(iSAMPLE_RATE,0,"extern:right");
}

YroScope::~YroScope() {
}

/*
 * Apply the effect
 */
void YroScope::render(float *inpl, float *inpr) {
	for(int target = 0, origin = iPERIOD;target < iSAMPLE_RATE - iPERIOD;target ++, origin++) {
		left[target]  = left[origin];
		right[target] = right[origin];
	}
	for(int target = iSAMPLE_RATE - iPERIOD, origin = 0;target < iSAMPLE_RATE;target ++, origin++) {
		left[target]  = inpl[origin];
		right[target] = inpr[origin];
	}
}

/**
 * getter and setter
 */
jack_default_audio_sample_t* YroScope::getLeft() const {
	return (left);
}

void YroScope::setLeft(jack_default_audio_sample_t* left) {
	this->left = left;
}

jack_default_audio_sample_t* YroScope::getRight() const {
	return (right);
}

void YroScope::setRight(jack_default_audio_sample_t* right) {
	this->right = right;
}


/* namespace std */
