/*
 * YroInternalException.h
 *
 *  Created on: 6 mai 2012
 *      Author: yannick
 */

#ifndef YROINTERNALEXCEPTION_H_
#define YROINTERNALEXCEPTION_H_

#include <exception>

class YroInternalException: public std::exception {
public:
	YroInternalException(const char *message);
	virtual const char* what() const throw () {
		return message;
	}
private:
	const char *message;
};

#endif /* YROINTERNALEXCEPTION_H_ */
