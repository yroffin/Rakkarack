/*
 * YroEffectFactoryTest.cpp
 *
 *  Created on: 8 mai 2012
 *      Author: yannick
 */

#include "YroEffectFactoryTest.h"
#include <utils/xml/tinyxml.h>
#include <utils/YroMd5.h>

using namespace std;

namespace test {

void YroEffectFactoryTest::setUp() {
	YroJackDriver::instance();
	YroParamHelper::instance()->setIntegerPeriod(128);
	YroParamHelper::instance()->setIntegerSampleRate(48000);
	YroEffectFactory::instance();
	for (int i = 0; i < TABLE_SIZE; i++) {
		sine[i] = .1f
				* (float) sin(((double) i / (double) TABLE_SIZE) * M_PI * 1.2f);
	}
	left_phase = 0;
	right_phase = 0;
}

void YroEffectFactoryTest::tearDown() {
}

void YroEffectFactoryTest::init(jack_nframes_t nframes,
		jack_default_audio_sample_t *out1, jack_default_audio_sample_t *out2) {
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

/**
 * dump this sample
 */
void YroEffectFactoryTest::dump(const char *name, jack_nframes_t nframes,
		jack_default_audio_sample_t *in1) {
	printf("%s: ", name);
	for (jack_nframes_t x = 0; x < nframes; x++) {
		if (x > 0)
			printf(", %f", in1[x]);
		else
			printf("%f", in1[x]);
	}
	printf("\n");
}

void YroEffectFactoryTest::compare(jack_nframes_t nframes,
		jack_default_audio_sample_t *out1, jack_default_audio_sample_t *out2) {
	for (jack_nframes_t x = 0; x < nframes; x++) {
		CPPUNIT_ASSERT_EQUAL(int(out1[x] * 1000) % 1000,
				int(out2[x] * 1000) % 1000);
	}
}

/**
 * load this xml chunk
 */
int load(TiXmlDocument &xml, const char *filename) {
	string line;
	string data;
	ifstream myfile(filename);
	if (myfile.is_open()) {
		while (myfile.good()) {
			getline(myfile, line);
			data += line;
		}
		myfile.close();
	} else {
		fprintf(stderr, "Unable to open file %s\n", filename);
		exit(1);
	}
	xml.Parse(data.c_str());
	fprintf(stdout, "Loading %s ok ...\n", filename);
	return 0;
}

/**
 * compare to contexts elements
 * @param TiXmlDocument &reference, reference document
 * @param  TiXmlDocument &current, the current element to compare
 */
int compareContexts(YroRawEffectPlugin *efx, TiXmlDocument &reference,
		TiXmlDocument &current, const char *filename) {
	TiXmlNode* element = 0;
	while ((element =
			reference.FirstChildElement("attributes")->IterateChildren(element))
			!= 0) {
		TiXmlNode* cmp = 0;
		int errors = 0;
		char tokens[16384];
		sprintf(tokens, "Errors, while checking members initialization (%s):",
				filename);
		while ((cmp = current.FirstChildElement("attributes")->IterateChildren(
				cmp)) != 0) {
			if (strcmp(element->ToElement()->Attribute("name"),
					cmp->ToElement()->Attribute("name")) == 0) {
				/**
				 * find attribute, then compare
				 */
				if (strcmp(element->ToElement()->Attribute("value"),
						cmp->ToElement()->Attribute("value")) != 0) {
					fprintf(stderr,
							"Attribute %s in error, expected %s, receive %s for effect %s\n",
							element->ToElement()->Attribute("name"),
							element->ToElement()->Attribute("value"),
							cmp->ToElement()->Attribute("value"),
							efx->getName());
					char __token[1024];
					sprintf(__token, " %s[%s/%s ko]",
							element->ToElement()->Attribute("name"),
							element->ToElement()->Attribute("value"),
							cmp->ToElement()->Attribute("value"));
					strcat(tokens, __token);
					errors++;
				}
			}
		}
		if (errors > 0) {
			CPPUNIT_FAIL(tokens);
		}
	}
	return 0;
}

/**
 * load this xml chunk
 */
int extract(TiXmlDocument &xml, float *left, float *right, int size,
		unsigned int &real, int &preset, const char *hashl, const char *hashr,
		const char *ileft, const char *iright) {
	TiXmlNode* element = 0;
	int check = (xml.FirstChildElement("elements") != 0);
	CPPUNIT_ASSERT_EQUAL(check, 1);
	const char *name = strdup(
			xml.FirstChildElement("elements")->Attribute("effect"));
	preset = atoi(xml.FirstChildElement("elements")->Attribute("preset"));
	const char *hashil = strdup(
			xml.FirstChildElement("elements")->Attribute(hashl));
	const char *hashir = strdup(
			xml.FirstChildElement("elements")->Attribute(hashr));
	cout << "name: " << name << ", preset " << preset << endl;
	YroMd5 *myMd5 = new YroMd5();
	real = 0;
	while ((element = xml.FirstChildElement("elements")->IterateChildren(
			element)) != 0) {
		int index = atoi(element->ToElement()->Attribute("index"));
		left[index] = atof(element->ToElement()->Attribute(ileft));
		right[index] = atof(element->ToElement()->Attribute(iright));
		real++;
	}
	const char *hashilComputed = myMd5->digestMemory((BYTE *) left,
			real * sizeof(float));
	int checkl = (strcmp(hashil, hashilComputed) == 0);
	const char *hashirComputed = myMd5->digestMemory((BYTE *) right,
			real * sizeof(float));
	int checkr = (strcmp(hashir, hashirComputed) == 0);
	return checkl && checkr;
}

/**
 * stress test basic
 */
void YroEffectFactoryTest::checkup(YroRawEffectPlugin *efx,
		const char *effectName, int indice) {
	char ctx[1024];
	char buf[1024];

	sprintf(ctx, "src/tests/data/%s-%08x-context.xml", effectName, indice);
	sprintf(buf, "src/tests/data/%s-%08x-buffer.xml", effectName, indice);

	jack_default_audio_sample_t *left, *right;
	jack_nframes_t nframes = YroParamHelper::instance()->getIntegerPeriod();
	left = new jack_default_audio_sample_t[nframes];
	right = new jack_default_audio_sample_t[nframes];
	int preset = 0;
	TiXmlDocument xmlBuffer, xmlContextReference, xmlContext;
	load(xmlBuffer, buf);
	load(xmlContextReference, ctx);
	unsigned int real = 0;
	if (extract(xmlBuffer, left, right, nframes, real, preset, "hashil",
			"hashir", "ileft", "iright")) {
		float *oleft = new jack_default_audio_sample_t[nframes];
		float *oright = new jack_default_audio_sample_t[nframes];
		float *oleftCheck = new jack_default_audio_sample_t[nframes];
		float *orightCheck = new jack_default_audio_sample_t[nframes];
		efx->setPreset(preset);
		efx->cleanup();
		switch (efx->getBehaviour()) {
		case YroRawEffectPlugin::_applyEffectOnInAndCopyToOut:
			((YroEffectPlugin *) efx)->setOutLeft(oleft);
			((YroEffectPlugin *) efx)->setOutRight(oright);
			efx->render(nframes, left, right);
			break;
		case YroRawEffectPlugin::_applyEffectOnInput:
			memcpy(oleft, left, nframes * sizeof(float));
			memcpy(oright, right, nframes * sizeof(float));
			efx->render(nframes, oleft, oright);
			break;
		}
		unsigned int real = 0;
		if (extract(xmlBuffer, oleftCheck, orightCheck, nframes, real, preset,
				"hashol", "hashor", "oleft", "oright")) {
			/**
			 * check result
			 */
			xmlContext.Parse(efx->toXml());
			if (compareContexts(efx, xmlContextReference, xmlContext, ctx)
					!= 0) {
				fprintf(stderr, "Contexts in error for effect %s ...\n",
						efx->getName());
			} else {
				fprintf(stdout, "Contexts ok for effect %s ...\n",
						efx->getName());
			}
			int checkByteError = 0;
			for (unsigned int i = 0; i < real; i++) {
				float expected = oleftCheck[i];
				float result = oleft[i];
				if(expected != result) {
					fprintf(stderr,"While checking %s index (left) %d, expected %9.40f, result %9.40f, delta %9.40f\n",
							efx->getName(),i, expected, result, expected - result);
				}
			}
			for (unsigned int i = 0; i < real; i++) {
				float expected = orightCheck[i];
				float result = oright[i];
				if(expected != result) {
					fprintf(stderr,"While checking %s index (right) %d, expected %9.40f, result %9.40f, delta %9.40f\n",
							efx->getName(),i, expected, result, expected - result);
				}
			}
			if (checkByteError == 0) {
				fprintf(stdout, "%d bytes checked ok ...\n", real);
			}
		}
	} else {
		fprintf(stderr, "wrong hash check ...\n");
	}
}

void YroEffectFactoryTest::checkup(YroRawEffectPlugin *efx) {
	for (int indice = 0; indice < efx->getPresetCount(); indice++) {
		checkup(efx, efx->getName(), indice);
	}
	fprintf(stdout, "Effect %s, successfully tested with %d preset(s)\n",
			efx->getName(), efx->getPresetCount());
}

void YroEffectFactoryTest::testAlienwah() {
	YroEffectPlugin *efx = new std::Alienwah();
	checkup(efx);
}
void YroEffectFactoryTest::testAnalogPhaser() {
	YroEffectPlugin *efx = new std::AnalogPhaser();
	checkup(efx);
}
void YroEffectFactoryTest::testArpie() {
	YroEffectPlugin *efx = new std::Arpie();
	checkup(efx);
}
void YroEffectFactoryTest::testChorus() {
	YroEffectPlugin *efx = new std::Chorus();
	checkup(efx);
}
void YroEffectFactoryTest::testCoilCrafter() {
	return;
	checkup(new std::CoilCrafter());
}
void YroEffectFactoryTest::testCompBand() {
	return;
	YroEffectPlugin *efx = new std::CompBand();
	checkup(efx);
}
void YroEffectFactoryTest::testCompressor() {
	checkup(new std::Compressor());
}
void YroEffectFactoryTest::testConvolotron() {
	return;
	YroEffectPlugin *efx = new std::Convolotron(
			YroParamHelper::instance()->getConvolotronDownsample(),
			YroParamHelper::instance()->getConvolotronUpQuality(),
			YroParamHelper::instance()->getConvolotronDownQuality());
	checkup(efx);
}
void YroEffectFactoryTest::testDualFlanger() {
	YroEffectPlugin *efx = new std::DualFlanger();
	checkup(efx);
}
void YroEffectFactoryTest::testDynamicFilter() {
	return;
	YroEffectPlugin *efx = new std::DynamicFilter();
	checkup(efx);
}
void YroEffectFactoryTest::testEcho() {
	YroEffectPlugin *efx = new std::Echo();
	checkup(efx);
}
void YroEffectFactoryTest::testEchotron() {
	return;
	YroEffectPlugin *efx = new std::Echotron();
	checkup(efx);
}
void YroEffectFactoryTest::testEQ() {
	return;
	YroEffectPlugin *efx = new std::EQ();
	checkup(efx);
}
void YroEffectFactoryTest::testExciter() {
	return;
	YroEffectPlugin *efx = new std::Exciter();
	checkup(efx);
}
void YroEffectFactoryTest::testGate() {
	YroRawEffectPlugin *efx = new std::Gate();
	checkup(efx);
}
void YroEffectFactoryTest::testHarmonizer() {
	return;
	YroEffectPlugin *efx = new std::Harmonizer(
			YroParamHelper::instance()->getHarmonizerQuality(),
			YroParamHelper::instance()->getHarmonizerDownsample(),
			YroParamHelper::instance()->getHarmonizerUpQuality(),
			YroParamHelper::instance()->getHarmonizerDownQuality());
	checkup(efx);
}
void YroEffectFactoryTest::testLooper() {
	return;
	YroEffectPlugin *efx = new std::Looper(1024);
	checkup(efx);
}
void YroEffectFactoryTest::testMBDist() {
	YroEffectPlugin *efx = new std::MBDist();
	checkup(efx);
}
void YroEffectFactoryTest::testMBVvol() {
	YroEffectPlugin *efx = new std::MBVvol();
	checkup(efx);
}
void YroEffectFactoryTest::testMusicDelay() {
	YroEffectPlugin *efx = new std::MusicDelay();
	checkup(efx);
}
void YroEffectFactoryTest::testNewDist() {
	return;
	checkup(new std::NewDist());
}
void YroEffectFactoryTest::testOpticaltrem() {
	checkup(new std::Opticaltrem());
}
void YroEffectFactoryTest::testPan() {
	checkup(new std::Pan());
}
void YroEffectFactoryTest::testPhaser() {
	checkup(new std::Phaser());
}
void YroEffectFactoryTest::testRBEcho() {
	return;
	YroEffectPlugin *efx = new std::RBEcho();
	checkup(efx);
}
void YroEffectFactoryTest::testReverb() {
	return;
	YroEffectPlugin *efx = new std::Reverb();
	checkup(efx);
}
void YroEffectFactoryTest::testReverbtron() {
	return;
	YroEffectPlugin *efx = new std::Reverbtron(
			YroParamHelper::instance()->getReverbtronDownsample(),
			YroParamHelper::instance()->getReverbtronUpQuality(),
			YroParamHelper::instance()->getReverbtronDownQuality());
	checkup(efx);
}
void YroEffectFactoryTest::testRing() {
	YroEffectPlugin *efx = new std::Ring();
	checkup(efx);
}
void YroEffectFactoryTest::testRyanWah() {
	YroEffectPlugin *efx = new std::RyanWah();
	checkup(efx);
}
void YroEffectFactoryTest::testSequence() {
	return;
	YroEffectPlugin *efx = new std::Sequence(
			YroParamHelper::instance()->getHarmonizerQuality(),
			YroParamHelper::instance()->getSequenceDownsample(),
			YroParamHelper::instance()->getSequenceUpQuality(),
			YroParamHelper::instance()->getSequenceDownQuality());
	checkup(efx);
}
void YroEffectFactoryTest::testShelfBoost() {
	return;
	YroEffectPlugin *efx = new std::ShelfBoost();
	checkup(efx);
}
void YroEffectFactoryTest::testShifter() {
	return;
	YroEffectPlugin *efx = new std::Shifter(
			YroParamHelper::instance()->getHarmonizerQuality(),
			YroParamHelper::instance()->getShifterDownsample(),
			YroParamHelper::instance()->getShifterUpQuality(),
			YroParamHelper::instance()->getShifterDownQuality());
	checkup(efx);
}
void YroEffectFactoryTest::testShuffle() {
	YroEffectPlugin *efx = new std::Shuffle();
	checkup(efx);
}
void YroEffectFactoryTest::testStereoHarm() {
	return;
	YroEffectPlugin *efx = new std::StereoHarm(
			YroParamHelper::instance()->getStereoHarmQuality(),
			YroParamHelper::instance()->getStereoHarmDownsample(),
			YroParamHelper::instance()->getStereoHarmUpQuality(),
			YroParamHelper::instance()->getStereoHarmDownQuality());
	checkup(efx);
}
void YroEffectFactoryTest::testStompBox() {
	return;
	YroEffectPlugin *efx = new std::StompBox();
	checkup(efx);
}
void YroEffectFactoryTest::testSustainer() {
	return;
	YroEffectPlugin *efx = new std::Sustainer();
	checkup(efx);
}
void YroEffectFactoryTest::testSynthfilter() {
	return;
	YroEffectPlugin *efx = new std::Synthfilter();
	checkup(efx);
}
void YroEffectFactoryTest::testValve() {
	return;
	YroEffectPlugin *efx = new std::Valve();
	checkup(efx);
}
void YroEffectFactoryTest::testVibe() {
	return;
	YroEffectPlugin *efx = new std::Vibe();
	checkup(efx);
}
void YroEffectFactoryTest::testVocoder() {
	return;
	YroEffectPlugin *efx = new std::Vocoder(0, 0,
			YroParamHelper::instance()->getVocoderDownsample(),
			YroParamHelper::instance()->getVocoderUpQuality(),
			YroParamHelper::instance()->getVocoderDownQuality());
	checkup(efx);
}

void YroEffectFactoryTest::testYroScope() {
	return;
	YroParamHelper::instance()->setIntegerPeriod(3);
	YroParamHelper::instance()->setIntegerSampleRate(9);

	std::YroEffectFactory *factory = std::YroEffectFactory::instance();
	factory->unload(0);
	factory->addEffect("scope#2", new YroScope());

	jack_default_audio_sample_t *in1, *in2, *out1, *out2;

	/**
	 * first render simulation
	 */
	jack_nframes_t nframes = YroParamHelper::instance()->getIntegerPeriod();
	in1 = new jack_default_audio_sample_t[nframes];
	in2 = new jack_default_audio_sample_t[nframes];
	out1 = new jack_default_audio_sample_t[nframes];
	out2 = new jack_default_audio_sample_t[nframes];

	init(nframes, in1, in2);
	init(nframes, out1, out2);

	dump("left:", YroParamHelper::instance()->getIntegerSampleRate(),
			YroAudioSampleFactory::instance()->allocate(
					YroParamHelper::instance()->getIntegerSampleRate(), 0,
					"extern:left"));
	dump("right:", YroParamHelper::instance()->getIntegerSampleRate(),
			YroAudioSampleFactory::instance()->allocate(
					YroParamHelper::instance()->getIntegerSampleRate(), 0,
					"extern:right"));

	factory->render(in1, in2, out1, out2);

	dump("in1:", nframes, in1);
	dump("in2:", nframes, in2);
	dump("out1:", nframes, out1);
	dump("out2:", nframes, out2);
	dump("left:", YroParamHelper::instance()->getIntegerSampleRate(),
			YroAudioSampleFactory::instance()->allocate(
					YroParamHelper::instance()->getIntegerSampleRate(), 0,
					"extern:left"));
	dump("right:", YroParamHelper::instance()->getIntegerSampleRate(),
			YroAudioSampleFactory::instance()->allocate(
					YroParamHelper::instance()->getIntegerSampleRate(), 0,
					"extern:right"));

	factory->render(in1, in2, out1, out2);
	factory->render(in1, in2, out1, out2);
	factory->render(in1, in2, out1, out2);

	dump("in1:", nframes, in1);
	dump("in2:", nframes, in2);
	dump("out1:", nframes, out1);
	dump("out2:", nframes, out2);
	dump("left:", YroParamHelper::instance()->getIntegerSampleRate(),
			YroAudioSampleFactory::instance()->allocate(
					YroParamHelper::instance()->getIntegerSampleRate(), 0,
					"extern:left"));
	dump("right:", YroParamHelper::instance()->getIntegerSampleRate(),
			YroAudioSampleFactory::instance()->allocate(
					YroParamHelper::instance()->getIntegerSampleRate(), 0,
					"extern:right"));

	float verif1[16] = { -0.001284, -0.003113, -0.003914, -0.004067, -0.004036,
			-0.003816, -0.003551, -0.003246, -0.002938, -0.002620, -0.002223,
			-0.001692, -0.000171, -0.000027, 0.000070, 0.000134 };
	compare(nframes, out1, verif1);
	float verif2[16] = { -0.324965, -0.787524, -0.990300, -1.029034, -1.021163,
			-0.965348, -0.898507, -0.821213, -0.743281, -0.662865, -0.562343,
			-0.428202, -0.043322, -0.006895, 0.017683, 0.033956 };
	compare(nframes, out2, verif2);
}

void YroEffectFactoryTest::testExpander() {
	return;
	YroParamHelper::instance()->setIntegerPeriod(3);
	YroParamHelper::instance()->setIntegerSampleRate(9);

	std::YroEffectFactory *factory = std::YroEffectFactory::instance();
	factory->unload(0);
	factory->addEffect("Expander#2", new Expander());

	jack_default_audio_sample_t *in1, *in2, *out1, *out2;

	/**
	 * first render simulation
	 */
	jack_nframes_t nframes = YroParamHelper::instance()->getIntegerPeriod();
	in1 = new jack_default_audio_sample_t[nframes];
	in2 = new jack_default_audio_sample_t[nframes];
	out1 = new jack_default_audio_sample_t[nframes];
	out2 = new jack_default_audio_sample_t[nframes];

	init(nframes, in1, in2);
	init(nframes, out1, out2);

	dump("left:", YroParamHelper::instance()->getIntegerSampleRate(),
			YroAudioSampleFactory::instance()->allocate(
					YroParamHelper::instance()->getIntegerSampleRate(), 0,
					"extern:left"));
	dump("right:", YroParamHelper::instance()->getIntegerSampleRate(),
			YroAudioSampleFactory::instance()->allocate(
					YroParamHelper::instance()->getIntegerSampleRate(), 0,
					"extern:right"));

	factory->render(in1, in2, out1, out2);

	dump("in1:", nframes, in1);
	dump("in2:", nframes, in2);
	dump("out1:", nframes, out1);
	dump("out2:", nframes, out2);

	float verif1[16] = { -0.001284, -0.003113, -0.003914, -0.004067, -0.004036,
			-0.003816, -0.003551, -0.003246, -0.002938, -0.002620, -0.002223,
			-0.001692, -0.000171, -0.000027, 0.000070, 0.000134 };
	compare(nframes, out1, verif1);
	float verif2[16] = { -0.324965, -0.787524, -0.990300, -1.029034, -1.021163,
			-0.965348, -0.898507, -0.821213, -0.743281, -0.662865, -0.562343,
			-0.428202, -0.043322, -0.006895, 0.017683, 0.033956 };
	compare(nframes, out2, verif2);
}

} /* namespace test */
