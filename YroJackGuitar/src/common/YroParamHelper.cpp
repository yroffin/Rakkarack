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

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <common/YroParamHelper.h>

YroParamHelper *YroParamHelper::__instance = 0;

YroParamHelper::YroParamHelper() {
	integerSampleRate = -1;
	integerPeriod = -1;

	/**
	 * default value
	 */
	waveshapeResampling = 0;
	waveshapeUpQuality = 4;
	waveshapeDownQuality = 2;
	upsample = 0;
	getBogomips();

    harmonizerDownsample = 5;
    harmonizerUpQuality = 0;
    harmonizerDownQuality = 0;
    reverbtronDownsample = 0;
    reverbtronUpQuality = 0;
    reverbtronDownQuality = 0;
    convolotronDownsample = 6;
    convolotronUpQuality = 0;
    convolotronDownQuality = 0;
    sequenceDownsample = 0;
    sequenceUpQuality = 0;
    sequenceDownQuality = 0;
    shifterDownsample = 0;
    shifterUpQuality = 0;
    shifterDownQuality = 0;
    vocoderDownsample = 0;
    vocoderUpQuality = 0;
    vocoderDownQuality = 0;
    stereoHarmDownsample = 0;
    stereoHarmUpQuality = 0;
    stereoHarmDownQuality = 0;

    harmonizerQuality = 4;
    stereoHarmQuality = 0;
    sequenceDownsample = 5;
    shifterDownsample = 5;
}

YroParamHelper::~YroParamHelper() {
}

int YroParamHelper::getErrorNumber() const {
	return errorNumber;
}

void YroParamHelper::setErrorNumber(int errorNumber) {
	this->errorNumber = errorNumber;
}

YroParamHelper* YroParamHelper::instance() {
	if (__instance == 0) {
		__instance = new YroParamHelper();
	}
	return __instance;
}

int YroParamHelper::getConvolotronDownQuality() const {
	return convolotronDownQuality;
}

void YroParamHelper::setConvolotronDownQuality(int convolotronDownQuality) {
	this->convolotronDownQuality = convolotronDownQuality;
}

int YroParamHelper::getConvolotronDownsample() const {
	return convolotronDownsample;
}

void YroParamHelper::setConvolotronDownsample(int convolotronDownsample) {
	this->convolotronDownsample = convolotronDownsample;
}

int YroParamHelper::getConvolotronUpQuality() const {
	return convolotronUpQuality;
}

void YroParamHelper::setConvolotronUpQuality(int convolotronUpQuality) {
	this->convolotronUpQuality = convolotronUpQuality;
}

int YroParamHelper::getHarmonizerDownQuality() const {
	return harmonizerDownQuality;
}

void YroParamHelper::setHarmonizerDownQuality(int harmonizerDownQuality) {
	this->harmonizerDownQuality = harmonizerDownQuality;
}

int YroParamHelper::getHarmonizerDownsample() const {
	return harmonizerDownsample;
}

void YroParamHelper::setHarmonizerDownsample(int harmonizerDownsample) {
	this->harmonizerDownsample = harmonizerDownsample;
}

int YroParamHelper::getHarmonizerUpQuality() const {
	return harmonizerUpQuality;
}

void YroParamHelper::setHarmonizerUpQuality(int harmonizerUpQuality) {
	this->harmonizerUpQuality = harmonizerUpQuality;
}

int YroParamHelper::getReverbtronDownQuality() const {
	return reverbtronDownQuality;
}

void YroParamHelper::setReverbtronDownQuality(int reverbtronDownQuality) {
	this->reverbtronDownQuality = reverbtronDownQuality;
}

int YroParamHelper::getReverbtronDownsample() const {
	return reverbtronDownsample;
}

void YroParamHelper::setReverbtronDownsample(int reverbtronDownsample) {
	this->reverbtronDownsample = reverbtronDownsample;
}

int YroParamHelper::getReverbtronUpQuality() const {
	return reverbtronUpQuality;
}

void YroParamHelper::setReverbtronUpQuality(int reverbtronUpQuality) {
	this->reverbtronUpQuality = reverbtronUpQuality;
}

int YroParamHelper::getSequenceDownQuality() const {
	return sequenceDownQuality;
}

void YroParamHelper::setSequenceDownQuality(int sequenceDownQuality) {
	this->sequenceDownQuality = sequenceDownQuality;
}

int YroParamHelper::getSequenceDownsample() const {
	return sequenceDownsample;
}

void YroParamHelper::setSequenceDownsample(int sequenceDownsample) {
	this->sequenceDownsample = sequenceDownsample;
}

int YroParamHelper::getSequenceUpQuality() const {
	return sequenceUpQuality;
}

void YroParamHelper::setSequenceUpQuality(int sequenceUpQuality) {
	this->sequenceUpQuality = sequenceUpQuality;
}

int YroParamHelper::getShifterDownQuality() const {
	return shifterDownQuality;
}

void YroParamHelper::setShifterDownQuality(int shifterDownQuality) {
	this->shifterDownQuality = shifterDownQuality;
}

int YroParamHelper::getShifterDownsample() const {
	return shifterDownsample;
}

void YroParamHelper::setShifterDownsample(int shifterDownsample) {
	this->shifterDownsample = shifterDownsample;
}

int YroParamHelper::getShifterUpQuality() const {
	return shifterUpQuality;
}

void YroParamHelper::setShifterUpQuality(int shifterUpQuality) {
	this->shifterUpQuality = shifterUpQuality;
}

int YroParamHelper::getStereoHarmDownQuality() const {
	return stereoHarmDownQuality;
}

void YroParamHelper::setStereoHarmDownQuality(int stereoHarmDownQuality) {
	this->stereoHarmDownQuality = stereoHarmDownQuality;
}

int YroParamHelper::getStereoHarmDownsample() const {
	return stereoHarmDownsample;
}

void YroParamHelper::setStereoHarmDownsample(int stereoHarmDownsample) {
	this->stereoHarmDownsample = stereoHarmDownsample;
}

int YroParamHelper::getStereoHarmUpQuality() const {
	return stereoHarmUpQuality;
}

void YroParamHelper::setStereoHarmUpQuality(int stereoHarmUpQuality) {
	this->stereoHarmUpQuality = stereoHarmUpQuality;
}

int YroParamHelper::getVocoderDownQuality() const {
	return vocoderDownQuality;
}

void YroParamHelper::setVocoderDownQuality(int vocoderDownQuality) {
	this->vocoderDownQuality = vocoderDownQuality;
}

int YroParamHelper::getVocoderDownsample() const {
	return vocoderDownsample;
}

void YroParamHelper::setVocoderDownsample(int vocoderDownsample) {
	this->vocoderDownsample = vocoderDownsample;
}

int YroParamHelper::getVocoderUpQuality() const {
	return vocoderUpQuality;
}

void YroParamHelper::setVocoderUpQuality(int vocoderUpQuality) {
	this->vocoderUpQuality = vocoderUpQuality;
}

int YroParamHelper::getHarmonizerQuality() const {
	return harmonizerQuality;
}

void YroParamHelper::setHarmonizerQuality(int harmonizerQuality) {
	this->harmonizerQuality = harmonizerQuality;
}

int YroParamHelper::getStereoHarmQuality() const {
	return stereoHarmQuality;
}

void YroParamHelper::setStereoHarmQuality(int stereoHarmQuality) {
	this->stereoHarmQuality = stereoHarmQuality;
}

int YroParamHelper::getBogomips() {
	/**
	 * re-activate this
	 */
	return 0;
	float bogomips;
	char temp[256];
	char *tmp;
	FILE *fp;

	if ((fp = fopen("/proc/cpuinfo", "r")) != NULL) {
		memset(temp, 0, sizeof(temp));

		while (fgets(temp, sizeof temp, fp) != NULL) {

			if (strstr(temp, "bogomips") != NULL)

			{
				tmp = strdup(temp);
				strsep(&tmp, ":");
				sscanf(tmp, "%f", &bogomips);
				break;
			}

		}

		maxLength = lrintf(150.0f / 4800.0f * bogomips);
		if (upsample) {
			maxLength /= (UpAmo + 8);
			maxLength /= (6 - ((UpQual + DownQual) / 2));
			// printf("Max Len: %d\n",maxx_len);

		}
		if (maxLength < 5) {
			if (maxLength < 2)
				maxLength = 2;
			/**
			 * TODO ... Message
			Message(0, "!! Rakarrack CPU Usage Warning !!",
					"It appears your CPU will not easily handle convolution with the current settings.  Be careful with the Convolotron effect settings.\nPlease read Help (F1) for more information.");
			 */
		}

		fclose(fp);
		return (1);
	}

	return (0);

}

float YroParamHelper::getFloatPeriod() const {
	return floatPeriod;
}

void YroParamHelper::setFloatPeriod(float floatPeriod) {
	this->floatPeriod = floatPeriod;
	check();
}

float YroParamHelper::getFloatSampleRate() const {
	if (floatSampleRate == 0.) {
		fprintf(stderr, "Internal error, sample rate cannot be null");
		exit(-1);
	}
	return floatSampleRate;
}

void YroParamHelper::setFloatSampleRate(float floatSampleRate) {
	this->floatSampleRate = floatSampleRate;
	check();
}

int YroParamHelper::getIntegerPeriod() const {
	return integerPeriod;
}

void YroParamHelper::setIntegerPeriod(int integerPeriod) {
	this->integerPeriod = integerPeriod;
	check();
}

unsigned int YroParamHelper::getIntegerSampleRate() const {
	if (integerSampleRate == 0) {
		fprintf(stderr, "Internal error, sample rate cannot be null");
		exit(-1);
	}
	return integerSampleRate;
}

void YroParamHelper::setIntegerSampleRate(unsigned int integerSampleRate) {
	this->integerSampleRate = integerSampleRate;
	check();
}

float YroParamHelper::getInverseFloatSampleRate() const {
	return inverseFloatSampleRate;
}

void YroParamHelper::setInverseFloatSampleRate(float inverseFloatSampleRate) {
	this->inverseFloatSampleRate = inverseFloatSampleRate;
}

int YroParamHelper::getTimePeriod() const {
	return timePeriod;
}

void YroParamHelper::setTimePeriod(int timePeriod) {
	this->timePeriod = timePeriod;
}

int YroParamHelper::getWaveshapeDownQuality() const {
	return waveshapeDownQuality;
}

void YroParamHelper::setWaveshapeDownQuality(int waveshapeDownQuality) {
	this->waveshapeDownQuality = waveshapeDownQuality;
}

int YroParamHelper::getWaveshapeResampling() const {
	return waveshapeResampling;
}

void YroParamHelper::setWaveshapeResampling(int waveshapeResampling) {
	this->waveshapeResampling = waveshapeResampling;
}

int YroParamHelper::getWaveshapeUpQuality() const {
	return waveshapeUpQuality;
}

void YroParamHelper::setWaveshapeUpQuality(int waveshapeUpQuality) {
	this->waveshapeUpQuality = waveshapeUpQuality;
}

