/*
 * YroCompilerOutputter.h
 *
 *  Created on: 28 mai 2012
 *      Author: yannick
 */

#ifndef YROCOMPILEROUTPUTTER_H_
#define YROCOMPILEROUTPUTTER_H_

#include <iostream>
#include <cppunit/CompilerOutputter.h>

namespace CppUnit {

class YroCompilerOutputter : public CompilerOutputter {
public:
	YroCompilerOutputter(TestResultCollector *result, OStream &stream);
	virtual ~YroCompilerOutputter();
	void printSuccess();
	void printFailuresList();
};

} /* namespace std */
#endif /* YROCOMPILEROUTPUTTER_H_ */
