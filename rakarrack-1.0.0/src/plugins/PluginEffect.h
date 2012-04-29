/*
 * PluginEffect.h
 *
 *  Created on: 27 avr. 2012
 *      Author: yannick
 */

#ifndef PLUGINEFFECT_H_
#define PLUGINEFFECT_H_

#include <stdio.h>
#include <string.h>

class PluginEffect {
public:
	PluginEffect();

	virtual ~PluginEffect();
	virtual void out(float * smpsl, float * smpsr);

	void setPfreq(int pfreq) {
		Pfreq = pfreq;
	}

	int getPafreq() const {
		return Pafreq;
	}

	int getPfreq() const {
		return Pfreq;
	}

	int getPpreset() const {
		return Ppreset;
	}

	void setPpreset(int Ppreset) {
		this->Ppreset = Ppreset;
	}

	int getPchange() const {
		return Pchange;
	}

	void setPchange(int pchange) {
		Pchange = pchange;
	}

	void setFileName(const char * fileName) {
		if (fileName == 0) {
			memset(this->fileName, 0, 128);
			return;
		}
		int _len = strlen(fileName);
		if (_len > 127)
			_len = 127;
		strncpy(this->fileName, fileName, _len);
		this->fileName[_len] = 0;
	}

	const char* getFileName() const {
		return fileName;
	}

	float* getEfxoutl() const {
		return efxoutl;
	}

	void setEfxoutl(float* efxoutl) {
		this->efxoutl = efxoutl;
	}

	float* getEfxoutr() const {
		return efxoutr;
	}

	void setEfxoutr(float* efxoutr) {
		this->efxoutr = efxoutr;
	}

	float getOutvolume() const {
		return outvolume;
	}

	void setOutvolume(float outvolume) {
		this->outvolume = outvolume;
	}

protected:
	float *efxoutl;
	float *efxoutr;
	float outvolume;
	char fileName[128];
	int Ppreset;
	int Pchange;
	int Pafreq;
	int Pfreq;
};

#endif /* PLUGINEFFECT_H_ */
