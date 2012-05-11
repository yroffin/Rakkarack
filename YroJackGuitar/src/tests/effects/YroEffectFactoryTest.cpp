/*
 * YroEffectFactoryTest.cpp
 *
 *  Created on: 8 mai 2012
 *      Author: yannick
 */

#include "YroEffectFactoryTest.h"

using namespace std;

namespace test {

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( YroEffectFactoryTest);

void YroEffectFactoryTest::setUp() {
	YroJackDriver::instance();
	YroParamHelper::instance()->setIntegerSampleRate(22050);
	YroEffectFactory::instance();
	for (int i = 0; i < TABLE_SIZE; i++) {
		sine[i] = .1f * (float) sin(((double) i / (double) TABLE_SIZE) * M_PI * 1.2f);
	}
	left_phase = 0;
	right_phase = 0;
}

void YroEffectFactoryTest::tearDown() {
}

void YroEffectFactoryTest::init(jack_nframes_t nframes, jack_default_audio_sample_t *out1, jack_default_audio_sample_t *out2) {
	for (jack_nframes_t i = 0; i < nframes; i++) {
		out1[i] = sine[left_phase]; /* left */
		out2[i] = sine[right_phase]; /* right */
		left_phase += 1;
		if (left_phase >= TABLE_SIZE)
			left_phase -= TABLE_SIZE;
		right_phase += 3; /* higher pitch so we can distinguish left and right. */
		if (right_phase >= TABLE_SIZE)
			right_phase -= TABLE_SIZE;
	}
}

void YroEffectFactoryTest::testBasic() {
	printf("Test: stress\n");
	YroEffectFactory *factory = YroEffectFactory::instance();
	YroParamHelper::instance()->setIntegerPeriod(16);
	factory->addEffect("distortion#1",new std::Distortion());

	jack_nframes_t nframes = 16;
	jack_default_audio_sample_t *in1, *in2, *out1, *out2;

	/**
	 * first render simulation
	 */
	in1 = new jack_default_audio_sample_t[nframes];
	in2 = new jack_default_audio_sample_t[nframes];
	out1 = new jack_default_audio_sample_t[nframes];
	out2 = new jack_default_audio_sample_t[nframes];

	init(nframes, in1, in2);
	init(nframes, out1, out2);

	factory->render(nframes, in1, in2, out1, out2);

	for (int exec = 0; exec < 1024; exec++) {
		/**
		 * second render simulation
		 */
		delete[] in1;
		delete[] in2;
		delete[] out1;
		delete[] out2;

		in1 = new jack_default_audio_sample_t[nframes];
		in2 = new jack_default_audio_sample_t[nframes];
		out1 = new jack_default_audio_sample_t[nframes];
		out2 = new jack_default_audio_sample_t[nframes];

		init(nframes, in1, in2);
		init(nframes, out1, out2);

		factory->render(nframes, in1, in2, out1, out2);
	}
	printf("Test: stress ended\n");
}

void YroEffectFactoryTest::dump(jack_nframes_t nframes,jack_default_audio_sample_t *in1, jack_default_audio_sample_t *in2, jack_default_audio_sample_t *out1, jack_default_audio_sample_t *out2) {
	printf("in1: ");
	for(jack_nframes_t x=0;x<nframes;x++) {
		if(x > 0) printf(", %f", in1[x]);
		else printf("%f", in1[x]);
	}
	printf("\nin2: ");
	for(jack_nframes_t x=0;x<nframes;x++) {
		if(x > 0) printf(", %f", in2[x]);
		else printf("%f", in2[x]);
	}
	printf("\n");
	printf("out1: ");
	for(jack_nframes_t x=0;x<nframes;x++) {
		if(x > 0) printf(", %f", out1[x]);
		else printf("%f", out1[x]);
	}
	printf("\nout2: ");
	for(jack_nframes_t x=0;x<nframes;x++) {
		if(x > 0) printf(", %f", out2[x]);
		else printf("%f", out2[x]);
	}
	printf("\n");
}

void YroEffectFactoryTest::compare(jack_nframes_t nframes,jack_default_audio_sample_t *out1, jack_default_audio_sample_t *out2) {
	for(jack_nframes_t x=0;x<nframes;x++) {
		CPPUNIT_ASSERT_EQUAL(int(out1[x] * 1000) % 1000, int(out2[x] * 1000) % 1000);
	}
}

void YroEffectFactoryTest::testDistortion() {
	printf("Test: Distortion\n");
	YroParamHelper::instance()->setIntegerPeriod(16);

	std::YroEffectFactory *factory = std::YroEffectFactory::instance();
	Distortion *eff = (Distortion *) factory->addEffect("distortion#2",new Distortion());

	/**
	 * wet/dry    : -64 ... not used in Distortion ?
	 * L/R Cr.    : 0
	 * Drive      : 127
	 * Level      : 76
	 * Type       : DynoFET
	 * Neg        : true
	 * Pre Filter : true
	 * Stereo     : false
	 * Pan        : 0
	 * Sub Oct    : 0
	 * LPF        : 2982
	 * HPF        : 645
	 */
	eff->setPlrcross(0);
	eff->setPdrive(127);
	eff->setPlevel(76);
	eff->setPtype(27);
	eff->setPnegate(1);
	eff->setPprefiltering(1);
	eff->setPstereo(0);
	eff->setPpanning(0);
	eff->setPoctave(0);
	eff->setPlpf(2982);
	eff->setPhpf(645);

	jack_nframes_t nframes = 16;
	jack_default_audio_sample_t *in1, *in2, *out1, *out2;

	/**
	 * first render simulation
	 */
	in1 = new jack_default_audio_sample_t[nframes];
	in2 = new jack_default_audio_sample_t[nframes];
	out1 = new jack_default_audio_sample_t[nframes];
	out2 = new jack_default_audio_sample_t[nframes];

	init(nframes, in1, in2);
	init(nframes, out1, out2);

	factory->render(nframes, in1, in2, out1, out2);

	dump(nframes, in1, in2, out1, out2);
	for(jack_nframes_t x=0;x<nframes;x++) {
		int c = (out1[x] != 0. && out2[x] != 0.);
		CPPUNIT_ASSERT_EQUAL(1, c);
	}

	printf("Test: Distortion ended\n");
}

void YroEffectFactoryTest::testChorus() {
	printf("Test: Chorus\n");
	YroParamHelper::instance()->setIntegerPeriod(16);

	std::YroEffectFactory *factory = std::YroEffectFactory::instance();
	Chorus *eff = (Chorus *) factory->addEffect("chorus#2",new Chorus());

	eff->setPlrcross(0);
	eff->setPpanning(0);

	jack_nframes_t nframes = 16;
	jack_default_audio_sample_t *in1, *in2, *out1, *out2;

	/**
	 * first render simulation
	 */
	in1 = new jack_default_audio_sample_t[nframes];
	in2 = new jack_default_audio_sample_t[nframes];
	out1 = new jack_default_audio_sample_t[nframes];
	out2 = new jack_default_audio_sample_t[nframes];

	init(nframes, in1, in2);
	init(nframes, out1, out2);

	factory->render(nframes, in1, in2, out1, out2);

	dump(nframes, in1, in2, out1, out2);

	float verif1[16] = {-0.001284, -0.003113, -0.003914, -0.004067, -0.004036, -0.003816, -0.003551, -0.003246, -0.002938, -0.002620, -0.002223, -0.001692, -0.000171, -0.000027, 0.000070, 0.000134};
	compare(nframes, out1, verif1);
	float verif2[16] = {-0.324965, -0.787524, -0.990300, -1.029034, -1.021163, -0.965348, -0.898507, -0.821213, -0.743281, -0.662865, -0.562343, -0.428202, -0.043322, -0.006895, 0.017683, 0.033956};
	compare(nframes, out2, verif2);

	printf("Test: Chorus ended\n");
}

} /* namespace test */
