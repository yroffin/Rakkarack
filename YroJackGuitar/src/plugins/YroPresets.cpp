/*
 YroJackGuitar - a software synthesizer based on excelent work
 of Rakkarack team
 
 Copyright (C) 2002-2005 Yannick Roffin
 Author: Yannick Roffin

 This program is free software; you can redistribute it and/or modify
 it under the terms of version 2 of the GNU General Public License 
 as published by the Free Software Foundation.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License (version 2) for more details.

 You should have received a copy of the GNU General Public License (version 2)
 along with this program; if not, write to the Free Software Foundation,
 Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
 
 YroPresets.cpp
 */

#include <plugins/YroPresets.h>

namespace std {

YroPresets::YroPresets(const char *_data) {
	char _preset[16384];
	char _value[16384];
	int iPreset;
	int iValue;
	iPreset=0;
	memset(_preset,0,sizeof(_preset));
	int inPresetName = 1;
	int inDataValue = 0;
	for(int i=0;_data[i]!=0;i++) {
		switch(_data[i]) {
			case ':':
				/**
				 * add this current preset
				 */
				inPresetName = 0;
				inDataValue = 1;
				i++;
				/**
				 * reset value
				 */
				iValue=0;
				memset(_value,0,sizeof(_value));
				break;
			case ';':
				/**
				 * add this value to current preset
				 */
				_presets[strdup(_preset)] = new YroPreset(_value);
				inPresetName = 1;
				inDataValue = 0;
				/**
				 * reset preset
				 */
				iPreset=0;
				memset(_preset,0,sizeof(_preset));
				break;
			default:
				if(inPresetName) {
					/**
					 * add this data to accu
					 */
					_preset[iPreset++] = _data[i];
				}
				if(inDataValue) {
					/**
					 * add this data to accu
					 */
					if(_data[i]==' ') break;
					_value[iValue++] = _data[i];
				}
				break;
		}
	}
}

/**
 * toString method
 */
void YroPresets::toStringCompute() {
	toStringInit("presets = {");
	map<const char *, YroPreset *, cmp_str>::iterator it;
	for(it=_presets.begin(); it!=_presets.end(); it++) {
		toStringCat("%s: %s", it->first, it->second->toString());
	}
	toStringCat("}");
}

YroPresets::~YroPresets() {
}

const YroPreset* YroPresets::get(int index) {
	map<const char *, YroPreset *, cmp_str>::iterator it;
	int i=0;
	for(it=_presets.begin(); it!=_presets.end(); it++,i++) {
		if(i == index) return it->second;
	}
	return 0;
}

}
/* namespace std */
