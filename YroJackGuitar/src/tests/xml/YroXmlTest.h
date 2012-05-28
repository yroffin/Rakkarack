/*
 * YroXmlTest.h
 *
 *  Created on: 28 mai 2012
 *      Author: yannick
 */

#ifndef YROXMLTEST_H_
#define YROXMLTEST_H_

#include <cppunit/extensions/HelperMacros.h>

namespace test {

class YroXmlTest : public CppUnit::TestFixture {
public:
	YroXmlTest();
	virtual ~YroXmlTest();
	CPPUNIT_TEST_SUITE( YroXmlTest );
	CPPUNIT_TEST( testXml );
	CPPUNIT_TEST_SUITE_END();

public:
	void setUp();
	void tearDown();

	void testXml();
};

} /* namespace std */
#endif /* YROXMLTEST_H_ */
