/*
 * YroObject.h
 *
 *  Created on: 4 mai 2012
 *      Author: yannick
 */

#ifndef YROOBJECT_H_
#define YROOBJECT_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <utils/YroLogger.h>

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
	YroObject();
	virtual ~YroObject();
	YroLogger *LOG;
protected:
};

} /* namespace std */
#endif /* YROOBJECT_H_ */
