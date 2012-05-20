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
 
 YroAmpli.cpp
 */

#include <plugins/ampli/YroAmpli.h>

namespace std {

YroAmpli::YroAmpli() : YroEffectPlugin(
		"YroAmpli",
		"Default: 100;") {
	/**
	 * fix klass attribute
	 */
	klass = YroEffectPlugin::_YroAmpli;
	factor = 1.0;
}

YroAmpli::~YroAmpli() {
}

/*
 * Apply the effect
 */
void YroAmpli::render(float *inpl, float *inpr) {
	for(int target = 0;target < iPERIOD;target ++) {
		efxoutl[target] = inpl[target] * factor;
		efxoutr[target] = inpr[target] * factor;
	}
}

float YroAmpli::getFactor() const {
	return factor;
}

void YroAmpli::setFactor(float factor) {
	this->factor = factor;}

} /* namespace std */
