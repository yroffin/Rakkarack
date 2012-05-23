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
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <utils/YroLogger.h>
#include <common/YroParamHelper.h>
#include <common/YroInternalException.h>

#ifndef YROOBJECT_H_
#define YROOBJECT_H_

namespace std {

struct cmp_str
{
   bool operator()(char const *a, char const *b)
   {
      return strcmp(a, b) < 0;
   }
};

class YroObject {
public:
	YroObject() {
		LOG = YroLogger::instance();
		helper = YroParamHelper::instance();
	};
	virtual ~YroObject() {};
	virtual void toStringInit(const char *value) {
		strcpy(_toString, value);
	}
	virtual void toStringCat(const char *format, ...) {
		va_list ap;
		va_start(ap, format);
		memset(_toStringFormat,0,sizeof(_toStringFormat));
		vsnprintf(_toStringFormat, sizeof(_toStringFormat), format, ap);
		va_end(ap);
		strcat(_toString, _toStringFormat);
	}
	virtual void toStringCompute() {
	}
	virtual const char *toString() {
		toStringCompute();
		return _toString;
	}
	YroLogger *getLogger() {
		return LOG;
	}
protected:
	YroLogger *LOG;
	YroParamHelper *helper;
private:
	char _toStringFormat[1024];
	char _toString[1024];
};

}
 /* namespace std */
#endif /* YROOBJECT_H_ */
