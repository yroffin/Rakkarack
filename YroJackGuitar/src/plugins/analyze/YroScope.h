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

#ifndef YROSCOPE_H_
#define YROSCOPE_H_

#include <plugins/YroEffectPlugin.h>

namespace std {

class YroScope: public std::YroEffectPlugin {
public:
	YroScope();
	virtual ~YroScope();
	void render(float *inpl, float *inpr);
	jack_default_audio_sample_t* getLeft() const;
	void setLeft(jack_default_audio_sample_t* left);
	jack_default_audio_sample_t* getRight() const;
	void setRight(jack_default_audio_sample_t* right);
private:
	jack_default_audio_sample_t *left;
	jack_default_audio_sample_t *right;
};

} /* namespace std */
#endif /* YROSCOPE_H_ */
