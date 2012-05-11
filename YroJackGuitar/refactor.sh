#!/bin/bash

for effect in `cat ../rakarrack-0.6.1/listeEffect.txt`; do cp ../rakarrack-0.6.1/src/$effect.C src/plugins/effect/$effect.cpp; done
for effect in `cat ../rakarrack-0.6.1/listeEffect.txt`; do cp ../rakarrack-0.6.1/src/$effect.h src/plugins/effect/$effect.h; done

normalize() {
	set -x
	mv src/plugins/effect/$1.cpp src/plugins/effect/$2.cpp
	mv src/plugins/effect/$1.h src/plugins/effect/$2.h
	FILE=src/plugins/effect/$2.cpp
	FILESED=src/plugins/effect/$2.sed
	cat $FILE | sed "s!$3!$2!g" > $FILESED
	mv -f $FILESED $FILE
	FILE=src/plugins/effect/$2.h
	FILESED=src/plugins/effect/$2.sed
	cat $FILE | sed "s!$3!$2!g" > $FILESED
	mv -f $FILESED $FILE
	FILE=src/plugins/effect/$2.cpp
	FILESED=src/plugins/effect/$2.sed
	cat $FILE | sed "s!$4!$2!g" > $FILESED
	mv -f $FILESED $FILE
	FILE=src/plugins/effect/$2.h
	FILESED=src/plugins/effect/$2.sed
	cat $FILE | sed "s!$4!$2!g" > $FILESED
	mv -f $FILESED $FILE
	set +x
}

# $1 le nom du fichier original
# $2 le nom du fichier cible
# $3 replace interne vers $2
# $4 replace interne vers $2

#normalize Dual_Flange DualFlanger Dflange Dual_Flange

inject_include() {
	echo inject include : $*
	effect=$1
	include=$2
	FILE=src/plugins/effect/$effect.h
	FILESED=src/plugins/effect/$effect.sed
	grep $include $FILE >/dev/null
	[ $? = 0 ] && {
		cat $FILE | sed "s!#include <plugins/YroEffectPlugin.h>!#include <plugins/YroEffectPlugin.h>\n#include <$3.h>!g" > $FILESED
		mv -f $FILESED $FILE
	}
}

replace_cpp() {
	echo replace cpp: $*
	FILE=src/plugins/effect/$effect.cpp
	FILESED=src/plugins/effect/$effect.sed
	# menage
	cat $FILE | sed "s!#include <math.h>!!g" > $FILESED
	mv -f $FILESED $FILE
	cat $FILE | sed "s!#include <stdio.h>!!g" > $FILESED
	mv -f $FILESED $FILE
	cat $FILE | sed "s!efxoutl = efxoutl_;!!g" > $FILESED
	mv -f $FILESED $FILE
	cat $FILE | sed "s!efxoutr = efxoutr_;!!g" > $FILESED
	mv -f $FILESED $FILE
	# default include
	cat $FILE | sed "s!#include \"$effect.h\"!#include <plugins/effect/$effect.h>\nusing namespace std;!g" > $FILESED
	mv -f $FILESED $FILE
	cat $FILE | sed "s!$effect::$effect!CONSTRUCTOR\n$effect::$effect!g" > $FILESED
	mv -f $FILESED $FILE
	grep -v $effect::$effect $FILE > $FILESED
	mv -f $FILESED $FILE
	cat $FILE | sed "s!CONSTRUCTOR!$effect::$effect() : YroEffectPlugin(\"$effect\")!g" > $FILESED
	mv -f $FILESED $FILE
	# lfo
	cat $FILE | sed "s!$effect::out (!$effect::render(jack_nframes_t nframes,!g" > $FILESED
	mv -f $FILESED $FILE
	cat $FILE | sed "s!effectlfoout (!render(nframes,!g" > $FILESED
	mv -f $FILESED $FILE
	cat $FILE | sed "s!lfo.Pfreq = value!lfo.setPfreq(value)!g;s!lfo.Prandomness = value!lfo.setPrandomness(value)!g;s!lfo.PLFOtype = value!lfo.setPlfOtype(value)!g;s!lfo.Pstereo = value!lfo.setPstereo(value)!g" > $FILESED
	mv -f $FILESED $FILE
	cat $FILE | sed "s!(lfo.Pfreq)!lfo.getPfreq()!g;s!(lfo.Prandomness)!lfo.getPrandomness()!g;s!(lfo.PLFOtype)!lfo.getPlfOtype()!g;s!(lfo.Pstereo)!lfo.getPstereo()!g" > $FILESED
	mv -f $FILESED $FILE
	cat $FILE | sed "s!dlfo.updateparams ();!!g" > $FILESED
	mv -f $FILESED $FILE
	cat $FILE | sed "s!lfo.updateparams ();!!g" > $FILESED
	mv -f $FILESED $FILE
	cat $FILE | sed "s! PERIOD! iPERIOD!g" > $FILESED
	mv -f $FILESED $FILE
	# lfo1
	cat $FILE | sed "s!lfo1.Pfreq = value!lfo1.setPfreq(value)!g;s!lfo1.Prandomness = value!lfo1.setPrandomness(value)!g;s!lfo1.PLFOtype = value!lfo1.setPlfOtype(value)!g;s!lfo1.Pstereo = value!lfo1.setPstereo(value)!g" > $FILESED
	mv -f $FILESED $FILE
	cat $FILE | sed "s!(lfo1.Pfreq)!lfo1.getPfreq()!g;s!(lfo1.Prandomness)!lfo1.getPrandomness()!g;s!(lfo1.PLFOtype)!lfo1.getPlfOtype()!g;s!(lfo1.Pstereo)!lfo1.getPstereo()!g" > $FILESED
	mv -f $FILESED $FILE
	cat $FILE | sed "s!dlfo1.updateparams ();!!g" > $FILESED
	mv -f $FILESED $FILE
	cat $FILE | sed "s!lfo1.updateparams ();!!g" > $FILESED
	mv -f $FILESED $FILE
	# lfo2
	cat $FILE | sed "s!lfo2.Pfreq = value!lfo2.setPfreq(value)!g;s!lfo2.Prandomness = value!lfo2.setPrandomness(value)!g;s!lfo2.PLFOtype = value!lfo2.setPlfOtype(value)!g;s!lfo2.Pstereo = value!lfo2.setPstereo(value)!g" > $FILESED
	mv -f $FILESED $FILE
	cat $FILE | sed "s!(lfo2.Pfreq)!lfo2.getPfreq()!g;s!(lfo2.Prandomness)!lfo2.getPrandomness()!g;s!(lfo2.PLFOtype)!lfo2.getPlfOtype()!g;s!(lfo2.Pstereo)!lfo2.getPstereo()!g" > $FILESED
	mv -f $FILESED $FILE
	cat $FILE | sed "s!dlfo2.updateparams ();!!g" > $FILESED
	mv -f $FILESED $FILE
	cat $FILE | sed "s!lfo2.updateparams ();!!g" > $FILESED
	mv -f $FILESED $FILE
	cat $FILE | sed "s! PERIOD! iPERIOD!g" > $FILESED
	mv -f $FILESED $FILE
	cat $FILE | sed "s!(PERIOD!(iPERIOD!g" > $FILESED
	mv -f $FILESED $FILE
	cat $FILE | sed "s! SAMPLE_RATE! iSAMPLE_RATE!g" > $FILESED
	mv -f $FILESED $FILE
	cat $FILE | sed "s!(SAMPLE_RATE!(iSAMPLE_RATE!g" > $FILESED
	mv -f $FILESED $FILE
	cat $FILE | sed "s!error_num=!helper->setErrorNumber(!g" > $FILESED
	mv -f $FILESED $FILE
	# filterout
	cat $FILE | sed "s!->filterout(h!->filterout(iPERIOD,fPERIOD,h!g" > $FILESED
	mv -f $FILESED $FILE
	cat $FILE | sed "s!->filterout(l!->filterout(iPERIOD,fPERIOD,l!g" > $FILESED
	mv -f $FILESED $FILE
	cat $FILE | sed "s!->filterout(m!->filterout(iPERIOD,fPERIOD,m!g" > $FILESED
	mv -f $FILESED $FILE
	cat $FILE | sed "s!->filterout(e!->filterout(iPERIOD,fPERIOD,e!g" > $FILESED
	mv -f $FILESED $FILE
	cat $FILE | sed "s!->filterout(input!->filterout(iPERIOD,fPERIOD,input!g" > $FILESED
	mv -f $FILESED $FILE
}

replace_hpp() {
	echo replace hpp: $*
	FILE=src/plugins/effect/$effect.h
	FILESED=src/plugins/effect/$effect.sed
	cat $FILE | sed "s!class $effect!\nnamespace std {\n\nclass $effect : public YroEffectPlugin\n!g" > $FILESED
	mv -f $FILESED $FILE
	cat $FILE | sed "s!EffectLFO!YroLowfrequencyOscillation!g" > $FILESED
	mv -f $FILESED $FILE
	cat $FILE | sed "s!EffectLFO!YroLowfrequencyOscillation!g" > $FILESED
	mv -f $FILESED $FILE
	cat $FILE | sed "s!#include \"global.h\"!#include <plugins/YroEffectPlugin.h>!g" > $FILESED
	mv -f $FILESED $FILE
	cat $FILE | sed "s!#include \"YroLowfrequencyOscillation.h\"!!g" > $FILESED
	mv -f $FILESED $FILE
	cat $FILE | sed "s!#endif!}\n#endif!g" > $FILESED
	mv -f $FILESED $FILE
	# menage
	cat $FILE | sed "s!#include \"HarmonicEnhancer.h\"!!g" > $FILESED
	mv -f $FILESED $FILE
	cat $FILE | sed "s!#include \"AnalogFilter.h\"!!g" > $FILESED
	mv -f $FILESED $FILE
	cat $FILE | sed "s!$effect (!CONSTRUCTOR\nTODELETE(!g" > $FILESED
	mv -f $FILESED $FILE
	grep -v "\~$effect" $FILE > $FILESED
	mv -f $FILESED $FILE
	grep -v "$effect (" $FILE > $FILESED
	mv -f $FILESED $FILE
	grep -v "TODELETE" $FILE > $FILESED
	mv -f $FILESED $FILE
	cat $FILE | sed "s!CONSTRUCTOR!$effect();!g" > $FILESED
	mv -f $FILESED $FILE
	# render
	cat $FILE | sed "s!void out (!void render(jack_nframes_t nframes,!g" > $FILESED
	mv -f $FILESED $FILE
	cat $FILE | sed "s!class FPreset \*Fpre;!!g" > $FILESED
	mv -f $FILESED $FILE
	cat $FILE | sed "s!class AnalogFilter !AnalogFilter !g" > $FILESED
	mv -f $FILESED $FILE
	cat $FILE | sed "s!class Resample !Resample !g" > $FILESED
	mv -f $FILESED $FILE
}

for effect in `cat refactor.txt`
do
	replace_cpp $effect
	replace_hpp $effect
	inject_include $effect YroLowfrequencyOscillation plugins/misc/YroLowfrequencyOscillation
	inject_include $effect HarmEnhancer plugins/filter/HarmEnhancer
	inject_include $effect AnalogFilter plugins/filter/AnalogFilter
	inject_include $effect Resample plugins/misc/Resample
done
exit 0
#include <plugins/YroEffectPlugin.h>

namespace std {

class YroChorus : public YroEffectPlugin {

