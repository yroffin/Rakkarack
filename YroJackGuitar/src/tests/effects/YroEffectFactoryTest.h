/*
 * YroEffectFactoryTest.h
 *
 *  Created on: 8 mai 2012
 *      Author: yannick
 */

#ifndef YROEFFECTFACTORYTEST_H_
#define YROEFFECTFACTORYTEST_H_

#include <stdio.h>
#include <jack/YroJackDriver.h>
#include <jack/YroEffectFactory.h>
#include <cppunit/extensions/HelperMacros.h>

#ifndef M_PI
#define M_PI  (3.14159265)
#endif

#define TABLE_SIZE   (200)

namespace test {

class YroEffectFactoryTest : public CppUnit::TestFixture {
	CPPUNIT_TEST_SUITE( YroEffectFactoryTest );
	CPPUNIT_TEST( testBasic );
	CPPUNIT_TEST( testDistortion );
	CPPUNIT_TEST( testChorus );
	CPPUNIT_TEST( testYroScope );
	CPPUNIT_TEST_SUITE_END();

public:
	void setUp();
	void tearDown();

	void testBasic();
	void testDistortion();
	void testChorus();
	void testYroScope();

private:
	void init(jack_nframes_t nframes, jack_default_audio_sample_t *out1, jack_default_audio_sample_t *out2);
	void dump(const char *name, jack_nframes_t nframes,jack_default_audio_sample_t *in1);
	void compare(jack_nframes_t nframes,jack_default_audio_sample_t *out1, jack_default_audio_sample_t *out2);

	float sine[TABLE_SIZE];
	int left_phase;
	int right_phase;
};

} /* namespace test */
#endif /* YROEFFECTFACTORYTEST_H_ */
