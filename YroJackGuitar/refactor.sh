#!/bin/bash

typeset -a tMember
typeset -i iMember=0
typeset -i iGetpar=0
typeset -i iGetparDeb=0
typeset -i iGetparFin=0
typeset -i iPreset=0
typeset -i iGetPreset=0
typeset -i iGetPresetDeb=0
typeset -i iGetPresetFin=0

typeset -a tAtt
typeset -i iAtt=0
typeset -a tAttMin
typeset -i iAttMin=0
typeset -a tAttIndex
typeset -i iAttIndex=0

searchMember() {
	effect=$1
	FILESED=$2
	FILECPP=$3
	FILEHPP=$4

	[ ! -f $FILECPP ] && {
		exit 1
	}

	typeset -i balise=`grep -n ::changepar $FILECPP | cut -f1 -d:`
	printf "Detecting changpar at line %d\n" $balise
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

	typeset -i balise=`grep -n ::setpreset $FILECPP | cut -f1 -d:`
	printf "Detecting setpreset at line %d\n" $balise
	for iNbLigne in `grep -n :: $FILECPP | cut -f1 -d':'`
	do
		[ ${iNbLigne} == $balise ] && {
			iGetPreset=${iPreset}
		}
		iPreset=${iPreset}+1
	done
	iGetPresetDeb=${tMember[$iGetPreset]}+1
	iGetPresetFin=${tMember[${iGetPreset}+1]}-$iGetPresetDeb
	iGetPresetDeb=${tMember[${iGetPreset}+1]}-2

	printf "Detecting members/changepar from %d (head %d)\n" $iGetparDeb $iGetparFin
	printf "Detecting members/setpreset from %d (head %d)\n" $iGetPresetDeb $iGetPresetFin
}

extractCase() {
	effect=$1
	FILESED=$2
	FILECPP=$3
	FILEHPP=$4

	typeset -i incase=0
	printf "Extract case from %d, %d\n" $iGetparDeb $iGetparFin
	head -n $iGetparDeb $FILECPP | tail -n $iGetparFin | while read line
	do
		printf "$line" | grep case >/dev/null && {
			incase=1
			printf "void $effect::set%s(int value) {\n" `printf "$line" | cut -f2 -d' '`
			continue
		}
		printf "$line" | grep 'break;' >/dev/null && {
			incase=0
			printf "}\n"
			continue
		}
		[ xxx`printf "$line" | sed 's! !!g;s!\t!!g'` == xxx ] && {
			continue
		}
		[ $incase == 1 ] && {
			printf "$line\n"
		}
	done
}

extractPreset() {
	effect=$1
	FILESED=$2
	FILECPP=$3
	FILEHPP=$4

	typeset -i bracket

	typeset -i inbrace=0
	export inbrace
	printf "Extract preset from %d, %d\n" $iGetPresetDeb $iGetPresetFin
	for token in `
	head -n $iGetPresetDeb $FILECPP | tail -n $iGetPresetFin | while read line
	do
		$(printf "$line" | grep '\[NUM_PRESETS\]\[PRESET_SIZE\]' >/dev/null)
		[ $? -eq 0 ] && {
			inbrace=1
			continue
		}
		$(printf "$line" | grep '};' >/dev/null)
		[ $? -eq 0 ] && {
			printf "$line"
			inbrace=2
			continue
		}
		[ $inbrace == 1 ] && {
			printf "$line"
		}
	done | sed 's!}! } !g;s!{! { !g'
	`
	do
		[ "$token" == "{" ] && {
			bracket=1
			printf ": "
			continue
		}
		[ "$token" == "}" ] && {
			bracket=0
			printf ";\"\n"
			continue
		}
		printf "%s" $token
	done
}

extractPar() {
	effect=$1
	FILESED=$2
	FILECPP=$3
	FILEHPP=$4
	printf "Extract attributes from %d, %d\n" $iGetparDeb $iGetparFin
	for att in `head -n $iGetparDeb $FILECPP | tail -n $iGetparFin | grep set | grep '(value)' | sed 's!lfo.set!setLfo!g;s!\t!!g;s!set!!g' | cut -f1 -d '('`
	do
		minatt=`echo ${att} | tr [:upper:] [:lower:]`
		tAtt[${iAtt}]=`echo ${att} | cut -b1 | tr [:lower:] [:upper:]``echo ${minatt} | cut -b2-`
		iAtt=${iAtt}+1
		tAttMin[${iAttMin}]=$minatt
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
		printf "Attribute[%s]: %s\n" ${tAttIndex[$iCpt]} ${tAttMin[$iCpt]}
		[ "0${tAttIndex[$iCpt]}" -ne $iCpt ] && {
			echo "Rupture d'index"
			exit 1
		}
		iCpt=$iCpt+1
	done 
	printf "/**\n* member declaration\n*/\nenum functions {\n" > extractHeader.h
	printf "_preset\n" >> extractHeader.h
	typeset -i iCpt=0
	while [ $iCpt -lt $iAtt ]
	do
		printf ", _%s\n" ${tAttMin[$iCpt]}
		iCpt=$iCpt+1
	done  >> extractHeader.h
	printf "};\n\n/**\n* setter and getter map\n*/\n" >> extractHeader.h
	printf "int  get0() {return getPreset();}\n" >> extractHeader.h
	printf "void set0(int value) {setPreset(value);}\n" >> extractHeader.h
	typeset -i iCpt=0
	while [ $iCpt -lt $iAtt ]
	do
		typeset -i occur=${tAttIndex[$iCpt]}+1
		printf "int  get%s() {return get%s();}\n" $occur ${tAtt[$iCpt]}
		printf "void set%s(int value) {set%s(value);}\n" $occur ${tAtt[$iCpt]}
		iCpt=$iCpt+1
	done >> extractHeader.h
	printf "\n" >> extractHeader.h
	printf "/**\n* setter and getter\n*/\n" >> extractHeader.h
	typeset -i iCpt=0
	while [ $iCpt -lt $iAtt ]
	do
		printf "int  get%s();\n" ${tAtt[$iCpt]}
		printf "void set%s(int value);\n" ${tAtt[$iCpt]}
		iCpt=$iCpt+1
	done >> extractHeader.h
	printf "\n" >> extractHeader.h
	typeset -i iCpt=0
	while [ $iCpt -lt $iAtt ]
	do
		printf "int  %s::get%s() {return P%s;}\n" $effect ${tAtt[$iCpt]} ${tAttMin[$iCpt]} | sed 's!Plfo!lfo.get!g;s!getrandomness!getPrandomness()!g;s!getfreq!getPfreq()!g;s!getstereo!getPstereo()!g;s!gettype!getPlfOype()!g;s!::getLfot!::getLfoT!g;s!::getLfof!::getLfoF!g;s!::getLfor!::getLfoR!g;s!::getLfos!::getLfoS!g'
		iCpt=$iCpt+1
	done >> extractHeader.h
	printf "\n" >> extractHeader.h
	typeset -i iCpt=0
	while [ $iCpt -lt $iAtt ]
	do
		printf "void %s::set%s(int value) {P%s = value;}\n" $effect ${tAtt[$iCpt]} ${tAttMin[$iCpt]} | sed 's!Plfo!lfo.set!g;s!setrandomness = value!setPrandomness(value)!g;s!setfreq = value!setPfreq(value)!g;s!settype = value!setPlfOtype(value)!g;s!setstereo = value!setPstereo(value)!g'
		iCpt=$iCpt+1
	done >> extractHeader.h
	printf "\n" >> extractHeader.h
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

for effect in $*
do
	export effect
	export FILESED=~/git/Rakkarack/YroJackGuitar/src/plugins/effect/$effect.sed
	export FILECPP=~/git/Rakkarack/YroJackGuitar/src/plugins/effect/$effect.cpp
	export FILEHPP=~/git/Rakkarack/YroJackGuitar/src/plugins/effect/$effect.h
	searchMember $effect $FILESED $FILECPP $FILEHPP
	extractCase  $effect $FILESED $FILECPP $FILEHPP
	extractPreset  $effect $FILESED $FILECPP $FILEHPP > extractPreset.cpp
	extractPar   $effect $FILESED $FILECPP $FILEHPP
done
exit 0

