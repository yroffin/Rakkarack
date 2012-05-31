#!/bin/bash

extractMember() {
	effect=$1
	FILESED=$2
	FILECPP=$3
	FILEHPP=$4

	printf "Processing $4\n"
	[ ! -f $FILEHPP ] && {
		exit 1
	}

	printf "char _toXml[4096];\n"
	printf "const char *toXml() {\n"
		printf "        char _buffer[256];\n"
		printf "        char _formatd[] = {\"<attribute name=%s%%s%s value=%s%%d%s />\"};\n" '\"' '\"' '\"' '\"'
		printf "        char _formatf[] = {\"<attribute name=%s%%s%s value=%s%%f%s />\"};\n" '\"' '\"' '\"' '\"'
		printf "        strcpy(_toXml,%s<attributes>%s);\n" '"' '"'
	for member in `grep 'int ' $FILEHPP | grep -v '(' | cut -f2- -d' ' | cut -f1 -d';' | sed 's!,! !g' | sort`
	do
		printf "$member" | grep -v '*' >/dev/null || continue
		printf "        sprintf(_buffer,_formatd,%s$member%s,$member);\n" '"' '"'
		printf "        strcat(_toXml,_buffer);\n"
	done
	for member in `grep 'float ' $FILEHPP | grep -v '(' | cut -f2- -d' ' | cut -f1 -d';' | sed 's!,! !g' | sort`
	do
		printf "$member" | grep -v '*' >/dev/null || continue
		printf "        sprintf(_buffer,_formatf,%s$member%s,$member);\n" '"' '"'
		printf "        strcat(_toXml,_buffer);\n"
	done
		printf "        strcat(_toXml,%s</attributes>%s);\n" '"' '"'
		printf "        return _toXml;\n"
		printf "}\n"
}

for effect in $*
do
	export effect
	export FILESED=~/git/Rakkarack/YroJackGuitar/src/plugins/distortion/$effect.sed
	export FILECPP=~/git/Rakkarack/YroJackGuitar/src/plugins/distortion/$effect.cpp
	export FILEHPP=~/git/Rakkarack/YroJackGuitar/src/plugins/distortion/$effect.h
	extractMember $effect $FILESED $FILECPP $FILEHPP
done
exit 0

