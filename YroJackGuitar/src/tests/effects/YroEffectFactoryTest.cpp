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
	YroParamHelper::instance()->setIntegerPeriod(1024);
	YroParamHelper::instance()->setIntegerSampleRate(22050);
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
	} else
		cout << "Unable to open file";

	xml.Parse(data.c_str());
	fprintf(stderr, "Loading %s ok ...\n", filename);
	return 0;
}

/**
 * load this xml chunk
 */
int compareContexts(TiXmlDocument &reference, TiXmlDocument &current) {
	TiXmlNode* element = 0;
	while ((element =
			reference.FirstChildElement("attributes")->IterateChildren(element))
			!= 0) {
		TiXmlNode* cmp = 0;
		while ((cmp =
				current.FirstChildElement("attributes")->IterateChildren(cmp))
				!= 0) {
			if(strcmp(element->ToElement()->Attribute("name"),cmp->ToElement()->Attribute("name"))==0) {
				/**
				 * find attribute, then compare
				 */
				if(strcmp(element->ToElement()->Attribute("value"),cmp->ToElement()->Attribute("value"))!=0) {
					fprintf(stderr,"Attribute %s in error, expected %s, receive %s\n",
							element->ToElement()->Attribute("name"),
							element->ToElement()->Attribute("value"),
							cmp->ToElement()->Attribute("value"));
				}
			}
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
	const char *name = strdup(
			xml.FirstChildElement("elements")->Attribute("effect"));
	preset = atoi(xml.FirstChildElement("elements")->Attribute("preset"));
	const char *hashil = strdup(
			xml.FirstChildElement("elements")->Attribute(hashl));
	const char *hashir = strdup(
			xml.FirstChildElement("elements")->Attribute(hashr));
	cout << "name: " << name << "/" << preset << endl;
	cout << hashl << " : " << hashil << endl;
	cout << hashr << " : " << hashir << endl;
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
	cout << "computed left hash: " << hashilComputed << endl;
	int checkl = (strcmp(hashil, hashilComputed) == 0);
	const char *hashirComputed = myMd5->digestMemory((BYTE *) right,
			real * sizeof(float));
	cout << "computed right hash: " << hashirComputed << endl;
	int checkr = (strcmp(hashir, hashirComputed) == 0);
	return checkl && checkr;
}

/**
 * stress test basic
 */
void YroEffectFactoryTest::testBasic() {
	jack_default_audio_sample_t *left, *right;
	jack_nframes_t nframes = YroParamHelper::instance()->getIntegerPeriod();
	left = new jack_default_audio_sample_t[nframes];
	right = new jack_default_audio_sample_t[nframes];
	int preset = 0;
	TiXmlDocument xmlBuffer, xmlContextReference, xmlContext;
	load(xmlBuffer, "src/tests/data/Distorsion-00000000-buffer.xml");
	load(xmlContextReference, "src/tests/data/Distorsion-00000000-context.xml");
	unsigned int real = 0;
	if (extract(xmlBuffer, left, right, nframes, real, preset, "hashil",
			"hashir", "ileft", "iright")) {
		float *oleft = new jack_default_audio_sample_t[nframes];
		float *oright = new jack_default_audio_sample_t[nframes];
		float *oleftCheck = new jack_default_audio_sample_t[nframes];
		float *orightCheck = new jack_default_audio_sample_t[nframes];
		Distortion *efx = new std::Distortion();
		efx->setPreset(4);
		efx->setOutLeft(oleft);
		efx->setOutRight(oright);
		efx->render(nframes, left, right);
		unsigned int real = 0;
		if (extract(xmlBuffer, oleftCheck, orightCheck, nframes, real, preset,
				"hashol", "hashor", "oleft", "oright")) {
			/**
			 * check result
			 */
			fprintf(stderr, "Checking contexts ...\n");
			xmlContext.Parse(efx->toXml());
			if (compareContexts(xmlContextReference, xmlContext) != 0) {
				fprintf(stderr, "Contexts in errror ...\n");
			}
			fprintf(stderr, "Checking %d bytes ...\n", real);
			for (unsigned int i = 0; i < real; i++) {
				if (oleftCheck[i] != oleft[i]) {
					fprintf(stderr,
							"Error, while checking index %d, assert %f and was %f\n",
							i, oleftCheck[i], oleft[i]);
				}
				if (orightCheck[i] != oright[i]) {
					fprintf(stderr,
							"Error, while checking index %d, assert %f and was %f\n",
							i, orightCheck[i], oright[i]);
				}
			}
		}
	} else {
		cout << "wrong hash check ..." << endl;
	}
}

void YroEffectFactoryTest::testDistortion() {
	return;
	std::YroEffectFactory *factory = std::YroEffectFactory::instance();
	factory->unload(0);
	Distortion *eff = (Distortion *) factory->addEffect("distortion#2",
			new Distortion());

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

	factory->render(in1, in2, out1, out2);

	dump("in1:", nframes, in1);
	dump("in2:", nframes, in2);
	dump("out1:", nframes, out1);
	dump("out2:", nframes, out2);

	for (jack_nframes_t x = 0; x < nframes; x++) {
		int c = (out1[x] != 0. && out2[x] != 0.);
		CPPUNIT_ASSERT_EQUAL(1, c);
	}
}

void YroEffectFactoryTest::testChorus() {
	return;
	std::YroEffectFactory *factory = std::YroEffectFactory::instance();
	factory->unload(0);
	Chorus *eff = (Chorus *) factory->addEffect("chorus#2", new Chorus());

	eff->setPlrcross(0);
	eff->setPpanning(0);

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
