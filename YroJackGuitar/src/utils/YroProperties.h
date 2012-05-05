/*
 * YroProperties.h
 *
 *  Created on: 4 mai 2012
 *      Author: yannick
 */

#ifndef YroProperties_H_
#define YroProperties_H_

namespace std {

class YroProperties {
public:
	static YroProperties *instance() {
		if (__instance == 0) {
			__instance = new YroProperties();
		}
		return __instance;
	}
	virtual ~YroProperties();
	/**
	 * getter
	 */
	int get(const char *entry, int &value, int defaultValue);
	int get(const char *entry, float &value, float defaultValue);
	int get(const char *entry, double &value, double defaultValue);
	int get(const char *entry, char *value, const char *defaultValue, int maxSize);
private:
	YroProperties();
	static YroProperties *__instance;
};

} /* namespace std */
#endif /* YroProperties_H_ */
