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
 
 YroAmpli.h
 */

#ifndef YROAMPLI_H_
#define YROAMPLI_H_

#include <plugins/YroEffectPlugin.h>

namespace std {

class YroAmpli: public std::YroEffectPlugin {
public:
	YroAmpli();
	virtual ~YroAmpli();
	void render(float *inpl, float *inpr);

	enum functions {
		_preset,
		_factor
	};

	int  get0() {return getPreset();};
	void set0(int value) {setPreset(value);};
	int  get1() {return int(getFactor()*100.);};
	void set1(int value) {
		LOG->debug("YroAmpli::setFactor(%f)", float(value)/100.);
		setFactor(float(value)/100.);
	};

	float getFactor() const;
	void setFactor(float factor);
private:
	float factor;
};

} /* namespace std */
#endif /* YROAMPLI_H_ */
