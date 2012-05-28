/*
 * YroUtilTest.h
 *
 *  Created on: 28 mai 2012
 *      Author: yannick
 */

#ifndef YroUtilTest_H_
#define YroUtilTest_H_

#include <cppunit/extensions/HelperMacros.h>

namespace test {

class YroUtilTest : public CppUnit::TestFixture {
public:
	YroUtilTest();
	virtual ~YroUtilTest();
	CPPUNIT_TEST_SUITE( YroUtilTest);
	CPPUNIT_TEST( testPresets );
	CPPUNIT_TEST_SUITE_END();

public:
	void setUp();
	void tearDown();
	void testPresets();
};

} /* namespace std */
#endif /* YroUtilTest_H_ */
