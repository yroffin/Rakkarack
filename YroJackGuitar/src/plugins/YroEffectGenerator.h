/*
 * YroEffectGenerator.h
 *
 *  Created on: 5 mai 2012
 *      Author: yannick
 */

#ifndef YROEFFECTGENERATOR_H_
#define YROEFFECTGENERATOR_H_

#include "YroEffectPlugin.h"

namespace std {

#ifndef M_PI
#define M_PI  (3.14159265)
#endif

#define TABLE_SIZE   (200)

class YroEffectGenerator: public std::YroEffectPlugin {
public:
	YroEffectGenerator();
	virtual ~YroEffectGenerator();
	virtual void render(jack_nframes_t nframes);
private:
	float sine[TABLE_SIZE];
	int left_phase;
	int right_phase;
};

} /* namespace std */
#endif /* YROEFFECTGENERATOR_H_ */
