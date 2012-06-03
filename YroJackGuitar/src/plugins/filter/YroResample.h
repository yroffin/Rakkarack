/*
 Resample - Resample

 Resample.h  - headers.
 Copyright (C) 2008-2010 Josep Andreu
 Author: Josep Andreu


 This program is free software; you can redistribute it and/or modify
 it under the terms of version 2 of the GNU General Public License
 as published by the Free Software Foundation.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License (version 2) for more details.

 You should have received a copy of the GNU General Public License
 (version2)  along with this program; if not, write to the Free Software
 Foundation,
 Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
 */

#ifndef YRORESAMPLE_H_
#define YRORESAMPLE_H_

#include <samplerate.h>
#include <common/YroObject.h>

class YroResample: public std::YroObject {
public:
	/*
	 Types:
	 SRC_SINC_BEST_QUALITY       = 0,
	 SRC_SINC_MEDIUM_QUALITY     = 1,
	 SRC_SINC_FASTEST            = 2,
	 SRC_ZERO_ORDER_HOLD         = 3,
	 SRC_LINEAR                  = 4
	 */
	YroResample(int type);
	virtual ~YroResample();
	void mono_out(float *inl, float *outl, int frames, double ratio,
			int o_frames);
protected:
	void cleanup();
	void out(float *inl, float *inr, float *outl, float *outr, int frames,
			double ratio);
private:
	// ratio Equal to output_sample_rate / input_sample_rate.
	SRC_DATA srcinfor;
	SRC_DATA srcinfol;
	int errorl, errorr, type;
	SRC_STATE *statel;
	SRC_STATE *stater;
};

#endif /* YRORESAMPLE_H_ */
