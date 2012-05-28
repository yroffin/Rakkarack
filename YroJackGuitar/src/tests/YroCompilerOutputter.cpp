/*
 * YroCompilerOutputter.cpp
 *
 *  Created on: 28 mai 2012
 *      Author: yannick
 */

#include <tests/YroCompilerOutputter.h>

using namespace CppUnit;

YroCompilerOutputter::YroCompilerOutputter(TestResultCollector *result, OStream &stream) : CompilerOutputter(result, stream) {
}

YroCompilerOutputter::~YroCompilerOutputter() {
}

void YroCompilerOutputter::printSuccess() {
	fprintf(stderr, "Success");
}

void YroCompilerOutputter::printFailuresList() {
	fprintf(stderr, "Success");
}
