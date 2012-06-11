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

	CPPUNIT_TEST( testAlienwah );
	CPPUNIT_TEST( testAnalogPhaser );
	CPPUNIT_TEST( testArpie );
	CPPUNIT_TEST( testChorus );
	CPPUNIT_TEST( testCoilCrafter );
	CPPUNIT_TEST( testCompBand );
	CPPUNIT_TEST( testCompressor );
	CPPUNIT_TEST( testConvolotron );
	CPPUNIT_TEST( testDualFlanger );
	CPPUNIT_TEST( testDynamicFilter );
	CPPUNIT_TEST( testEcho );
	CPPUNIT_TEST( testEchotron );
	CPPUNIT_TEST( testEQ );
	CPPUNIT_TEST( testExciter );
	CPPUNIT_TEST( testGate );
	CPPUNIT_TEST( testHarmonizer );
	CPPUNIT_TEST( testLooper );
	CPPUNIT_TEST( testMBDist );
	CPPUNIT_TEST( testMBVvol );
	CPPUNIT_TEST( testMusicDelay );
	CPPUNIT_TEST( testNewDist );
	CPPUNIT_TEST( testOpticaltrem );
	CPPUNIT_TEST( testPan );
	CPPUNIT_TEST( testPhaser );
	CPPUNIT_TEST( testRBEcho );
	CPPUNIT_TEST( testReverb );
	CPPUNIT_TEST( testReverbtron );
	CPPUNIT_TEST( testRing );
	CPPUNIT_TEST( testRyanWah );
	CPPUNIT_TEST( testSequence );
	CPPUNIT_TEST( testShelfBoost );
	CPPUNIT_TEST( testShifter );
	CPPUNIT_TEST( testShuffle );
	CPPUNIT_TEST( testStereoHarm );
	CPPUNIT_TEST( testStompBox );
	CPPUNIT_TEST( testSustainer );
	CPPUNIT_TEST( testSynthfilter );
	CPPUNIT_TEST( testValve );
	CPPUNIT_TEST( testVibe );
	CPPUNIT_TEST( testVocoder );

	CPPUNIT_TEST( testYroScope );
	CPPUNIT_TEST( testExpander );
	CPPUNIT_TEST_SUITE_END();

public:
	void setUp();
	void tearDown();

	void checkup(std::YroEffectPlugin *efx, const char *effectName,int indice);
	void checkup(std::YroEffectPlugin *efx);

	void testAlienwah();
	void testAnalogPhaser();
	void testArpie();
	void testChorus();
	void testCoilCrafter();
	void testCompBand();
	void testCompressor();
	void testConvolotron();
	void testDualFlanger();
	void testDynamicFilter();
	void testEcho();
	void testEchotron();
	void testEQ();
	void testExciter();
	void testGate();
	void testHarmonizer();
	void testLooper();
	void testMBDist();
	void testMBVvol();
	void testMusicDelay();
	void testNewDist();
	void testOpticaltrem();
	void testPan();
	void testPhaser();
	void testRBEcho();
	void testReverb();
	void testReverbtron();
	void testRing();
	void testRyanWah();
	void testSequence();
	void testShelfBoost();
	void testShifter();
	void testShuffle();
	void testStereoHarm();
	void testStompBox();
	void testSustainer();
	void testSynthfilter();
	void testValve();
	void testVibe();
	void testVocoder();

	void testYroScope();
	void testExpander();

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
