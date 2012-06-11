/*
 * YroEffectPlugin.h
 *
 *  Created on: 5 mai 2012
 *      Author: yannick
 */

#ifndef YROEFFECTPLUGIN_H_
#define YROEFFECTPLUGIN_H_

#include <plugins/YroRawEffectPlugin.h>

namespace std {

class YroEffectPlugin: public YroRawEffectPlugin {
public:
	YroEffectPlugin(const char *_name);
	YroEffectPlugin(const char *_name, const char *_presets);
	virtual ~YroEffectPlugin();
	/**
	 * getter
	 */
	float *getOutLeft();
	float *getOutRight();
	void setOutLeft(float *value);
	void setOutRight(float *value);
	int getBehaviour();
	/**
	 * statistics
	 */
	void dumpStats(const char *where) {
		YroRawEffectPlugin::dumpStats(where,efxoutl,efxoutr);
	}
protected:
	float *efxoutl;
	float *efxoutr;
};

} /* namespace std */

#endif /* YROEFFECTPLUGIN_H_ */
