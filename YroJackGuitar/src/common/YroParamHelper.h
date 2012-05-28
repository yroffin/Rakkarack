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

#ifndef YROPARAMHELPER_H_
#define YROPARAMHELPER_H_

class YroParamHelper {
public:
	static YroParamHelper *instance();
	virtual ~YroParamHelper();

	float getFloatPeriod() const;
	void setFloatPeriod(float floatPeriod);
	float getFloatSampleRate() const;
	void setFloatSampleRate(float floatSampleRate);
	int getIntegerPeriod() const;
	void setIntegerPeriod(int integerPeriod);
	unsigned int getIntegerSampleRate() const;
	void setIntegerSampleRate(unsigned int integerSampleRate);
	float getInverseFloatSampleRate() const;
	void setInverseFloatSampleRate(float inverseFloatSampleRate);
	int getTimePeriod() const;
	void setTimePeriod(int timePeriod);
	int getWaveshapeDownQuality() const;
	void setWaveshapeDownQuality(int waveshapeDownQuality);
	int getWaveshapeResampling() const;
	void setWaveshapeResampling(int waveshapeResampling);
	int getWaveshapeUpQuality() const;
	void setWaveshapeUpQuality(int waveshapeUpQuality);

	int getErrorNumber() const;
	void setErrorNumber(int errorNumber);

	int getMaxLength() const {
		return maxLength;
	}

	void check() {
		floatSampleRate = (float) ((((integerSampleRate))));
		inverseFloatSampleRate = 1.0f / floatSampleRate;
		floatPeriod = float(integerPeriod);
		if(integerPeriod > 0)
			timePeriod = integerSampleRate / 12 / integerPeriod;
		else
			timePeriod = 0;
	}

private:
	YroParamHelper();
	static YroParamHelper *__instance;
	int getBogomips();
	/**
	 * global parameter
	 */
	int errorNumber;
	unsigned int integerSampleRate;
	float floatSampleRate;
	float inverseFloatSampleRate;
	int integerPeriod;
	float floatPeriod;
	int waveshapeResampling;
	int waveshapeUpQuality;
	int waveshapeDownQuality;
	int timePeriod;

	/**
	 * TODO verify use of maxLength, UpAmo, UpQual, DownQual
	 * TODO appear many times at different place ... in original system
	 */
	int maxLength;
	int upsample;
	int UpAmo;
	int UpQual;
	int DownQual;
};

#endif /* YROPARAMHELPER_H_ */
