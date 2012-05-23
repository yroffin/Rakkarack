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
 
 YroPreset.h
 */

#ifndef YROPRESETS_H_
#define YROPRESETS_H_

#include <common/YroObject.h>
#include <plugins/YroPreset.h>

namespace std {

class YroPresets: public YroObject {
public:
	YroPresets(const char *_data);
	virtual ~YroPresets();
	void toStringCompute();
	int size() {
		return _presets.size();
	}
	const YroPreset *get(int index);
private:
	map<const char *, YroPreset *, cmp_str> _presets;
};

} /* namespace std */
#endif /* YROPRESET_H_ */
