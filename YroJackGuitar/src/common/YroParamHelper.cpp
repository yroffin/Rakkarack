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
	/**
	 * default value
	 */
	waveshapeResampling = 0;
	waveshapeUpQuality = 4;
	waveshapeDownQuality = 2;
	upsample = 0;
	getBogomips();
}

YroParamHelper::~YroParamHelper() {
}

int YroParamHelper::getErrorNumber() const {
	return errorNumber;
}

void YroParamHelper::setErrorNumber(int errorNumber) {
	this->errorNumber = errorNumber;
}

int YroParamHelper::getBogomips() {
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

