/*
 * YroParamHelper.h
 *
 *  Created on: 7 mai 2012
 *      Author: yannick
 */

#ifndef YROPARAMHELPER_H_
#define YROPARAMHELPER_H_

#include <core/YroObject.h>

class YroParamHelper: public std::YroObject {
public:
	static YroParamHelper *instance() {
		if (__instance == 0) {
			__instance = new YroParamHelper();
		}
		return __instance;
	}
	virtual ~YroParamHelper();

	int getTimePeriod() const {
		return timePeriod;
	}

	void setTimePeriod(int timePeriod) {
		this->timePeriod = timePeriod;
	}

	void check() {
		floatSampleRate = (float) (integerSampleRate);
		inverseFloatSampleRate = 1.0f / floatSampleRate;
		floatPeriod = float(integerPeriod);
		if(integerPeriod > 0)
			timePeriod = integerSampleRate / 12 / integerPeriod;
		else
			timePeriod = 0;
		LOG->info("[CHECK] sample rate : %d", integerSampleRate);
		LOG->info("[CHECK] frames      : %d", integerPeriod);
		LOG->info("[CHECK] time period : %d", timePeriod);
	}

	int getWaveshapeDownQuality() const {
		return waveshapeDownQuality;
	}

	void setWaveshapeDownQuality(int waveshapeDownQuality) {
		this->waveshapeDownQuality = waveshapeDownQuality;
	}

	int getWaveshapeResampling() const {
		return waveshapeResampling;
	}

	void setWaveshapeResampling(int waveshapeResampling) {
		this->waveshapeResampling = waveshapeResampling;
	}

	int getWaveshapeUpQuality() const {
		return waveshapeUpQuality;
	}

	void setWaveshapeUpQuality(int waveshapeUpQuality) {
		this->waveshapeUpQuality = waveshapeUpQuality;
	}

	float getInverseFloatSampleRate() const {
		return inverseFloatSampleRate;
	}

	void setInverseFloatSampleRate(float inverseFloatSampleRate) {
		this->inverseFloatSampleRate = inverseFloatSampleRate;
	}

	int getIntegerPeriod() const {
		return integerPeriod;
	}

	void setIntegerPeriod(int integerPeriod) {
		this->integerPeriod = integerPeriod;
		check();
	}

	float getFloatPeriod() const {
		return floatPeriod;
	}

	void setFloatPeriod(float floatPeriod) {
		this->floatPeriod = floatPeriod;
	}

	float getFloatSampleRate() const {
		return floatSampleRate;
	}

	void setFloatSampleRate(float floatSampleRate) {
		this->floatSampleRate = floatSampleRate;
	}

	unsigned int getIntegerSampleRate() const {
		return integerSampleRate;
	}

	void setIntegerSampleRate(unsigned int integerSampleRate) {
		this->integerSampleRate = integerSampleRate;
		check();
	}
private:
	YroParamHelper();
	static YroParamHelper *__instance;
	/**
	 * global parameter
	 */
	unsigned int integerSampleRate;
	float floatSampleRate;
	float inverseFloatSampleRate;
	int integerPeriod;
	float floatPeriod;
	int waveshapeResampling;
	int waveshapeUpQuality;
	int waveshapeDownQuality;
	int timePeriod;
};

#endif /* YROPARAMHELPER_H_ */
