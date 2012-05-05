/*
 * YroObject.h
 *
 *  Created on: 4 mai 2012
 *      Author: yannick
 */

#ifndef YROOBJECT_H_
#define YROOBJECT_H_

#include "../utils/YroLogger.h"

namespace std {

class YroObject {
public:
	YroObject();
	virtual ~YroObject();
	YroLogger *LOG;
protected:
};

} /* namespace std */
#endif /* YROOBJECT_H_ */
