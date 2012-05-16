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
 
 YroPreset.cpp
 */

#include <plugins/YroPreset.h>

namespace std {

/**
 * default constructor
 */
YroPreset::YroPreset(const char *_data) {
	char _value[1024];
	int iValue;
	iValue=0;
	memset(_value,0,sizeof(_value));
	for(int i=0;_data[i]!=0;i++) {
		switch(_data[i]) {
			case ',':
				/**
				 * flush data
				 */
				values.push_back(atoi(_value));
				iValue=0;
				memset(_value,0,sizeof(_value));
				break;
			case ' ':
				break;
			default:
				_value[iValue++] = _data[i];
				break;
		}
	}
	if(iValue>0) {
		values.push_back(atoi(_value));
	}
}

/**
 * toString method
 */
void YroPreset::toStringCompute() {
	toStringInit("values = {");
	vector<int>::iterator it;
	int iCpt = 0;
	for(it=values.begin(); it!=values.end(); it++, iCpt++) {
		if(iCpt==0) {
			toStringCat("%d", *it);
		} else {
			toStringCat(", %d", *it);
		}
	}
	toStringCat("}");
}

YroPreset::~YroPreset() {
}

} /* namespace std */
