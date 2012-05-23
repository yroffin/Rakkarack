#!/bin/bash

set -a tMember
typeset -i iMember=0
typeset -i iGetpar=0
typeset -i iGetparDeb=0
typeset -i iGetparFin=0

set -a tAtt
typeset -i iAtt=0
set -a tAttMin
typeset -i iAttMin=0
set -a tAttIndex
typeset -i iAttIndex=0

searchMember() {
	typeset -i balise=`grep -n ::changepar $FILECPP | cut -f1 -d:`
	for iNbLigne in `grep -n :: $FILECPP | cut -f1 -d':'`
	do
		[ ${iNbLigne} == $balise ] && {
			iGetpar=${iMember}
		}
		tMember[${iMember}]=${iNbLigne}
		iMember=${iMember}+1
	done
	iGetparDeb=${tMember[$iGetpar]}+1
	iGetparFin=${tMember[${iGetpar}+1]}-$iGetparDeb
	iGetparDeb=${tMember[${iGetpar}+1]}-2
}

extractPar() {
	printf "Extract from %d, %d\n" $iGetparDeb $iGetparFin
	for att in `head -n $iGetparDeb $FILECPP | tail -n $iGetparFin | grep set | sed 's!lfo.set!setLfo!g;s!\t!!g;s!set!!g' | cut -f1 -d '('`
	do
		tAtt[${iAtt}]=`echo ${att} | cut -b1 | tr [:lower:] [:upper:]``echo ${att} | cut -b2-`
		iAtt=${iAtt}+1
		tAttMin[${iAttMin}]=${att}
		iAttMin=${iAttMin}+1
	done
	for att in `head -n $iGetparDeb $FILECPP | tail -n $iGetparFin | grep case | sed 's! !!g;s!\t!!g;s!case!!g' | cut -f1 -d':'`
	do
		tAttIndex[${iAttIndex}]=${att}
		iAttIndex=${iAttIndex}+1
	done
	typeset -i iCpt=0
	while [ $iCpt -lt $iAtt ]
	do
		printf "Attribute[%s]: %s\n" ${tAttIndex[$iCpt]} ${tAtt[$iCpt]}
		[ "0${tAttIndex[$iCpt]}" -ne $iCpt ] && {
			echo "Rupture d'index"
			exit 1
		}
		iCpt=$iCpt+1
	done
	printf "/**\n* member declaration\n*/\nenum functions {_preset\n"
	typeset -i iCpt=0
	while [ $iCpt -lt $iAtt ]
	do
		printf ", _%s\n" ${tAttMin[$iCpt]}
		iCpt=$iCpt+1
	done
	printf "};\n\n/**\n* setter and getter map\n*/\n"
	printf "int  get0() {return getPreset();}\n"
	printf "void set0(int value) {setPreset(value);}\n"
	typeset -i iCpt=0
	while [ $iCpt -lt $iAtt ]
	do
		typeset -i occur=${tAttIndex[$iCpt]}+1
		printf "int  get%s() {return get%s();}\n" $occur ${tAtt[$iCpt]}
		printf "void set%s(int value) {set%s(value);}\n" $occur ${tAtt[$iCpt]}
		iCpt=$iCpt+1
	done
	printf "\n"
	printf "/**\n* setter and getter\n*/\n"
	typeset -i iCpt=0
	while [ $iCpt -lt $iAtt ]
	do
		printf "int  get%s();\n" ${tAtt[$iCpt]}
		printf "void set%s(int value);\n" ${tAtt[$iCpt]}
		iCpt=$iCpt+1
	done
	printf "\n"
	typeset -i iCpt=0
	while [ $iCpt -lt $iAtt ]
	do
		printf "int  %s::get%s() {return P%s;}\n" $effect ${tAtt[$iCpt]} ${tAttMin[$iCpt]} | sed 's!PLfo!lfo.get!g;s!getPrandomness!getPrandomness()!g;s!getPfreq!getPfreq()!g;s!getPstereo!getPstereo()!g;s!getPlfOtype!getPlfOtype()!g'
		iCpt=$iCpt+1
	done
	printf "\n"
	typeset -i iCpt=0
	while [ $iCpt -lt $iAtt ]
	do
		printf "void %s::set%s(int value) {P%s = value;}\n" $effect ${tAtt[$iCpt]} ${tAttMin[$iCpt]} | sed 's!PLfo!lfo.set!g;s!setPrandomness = value!setPrandomness(value)!g;s!setPfreq = value!setPfreq(value)!g;s!setPlfOtype = value!setPlfOtype(value)!g;s!setPstereo = value!setPstereo(value)!g'
		iCpt=$iCpt+1
	done
	printf "\n"
}

normalizeCpp() {
	echo replace cpp: $*
	FILE=src/plugins/effect/$effect.cpp
	FILESED=src/plugins/effect/$effect.sed
	# menage
	#cat $FILE | sed "s!#include <math.h>!!g" > $FILESED
	#mv -f $FILESED $FILE
}

normalizeHpp() {
	echo replace hpp: $*
}

for effect in Shuffle
do
	export effect
	export FILESED=~/git/Rakkarack/YroJackGuitar/src/plugins/effect/$effect.sed
	export FILECPP=~/git/Rakkarack/YroJackGuitar/src/plugins/effect/$effect.cpp
	export FILEHPP=~/git/Rakkarack/YroJackGuitar/src/plugins/effect/$effect.h
	searchMember
	extractPar

	normalizeCpp $effect
	normalizeHpp $effect
done
exit 0

