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
 
 YroRawEffectPlugin.cpp
 */

#include <plugins/YroRawEffectPlugin.h>

namespace std {

void YroRawEffectPlugin::initialize(const char* _name) {
	/**
	 * fix name of this effect
	 */
	name = strdup(_name);
	/**
	 * fix audio sample factory
	 */
	audioSampleFactory = YroAudioSampleFactory::instance();
	/**
	 * default initialization
	 */
	forcedCleanup = 0;
	iPERIOD = helper->getIntegerPeriod();
	iSAMPLE_RATE = helper->getIntegerSampleRate();
	fPERIOD = helper->getFloatPeriod();
	fSAMPLE_RATE = helper->getFloatSampleRate();
	cSAMPLE_RATE = 1.0f / fSAMPLE_RATE;
	/**
	 * misc members
	 */
	forcedCleanup = 0;
	preset = 0;
	active = 0;
}

YroRawEffectPlugin::YroRawEffectPlugin(const char *_name) {
	initialize(_name);
	presets = new YroPresets("");
}

/**
 * default constructor
 * @param const char *_name, name of this effect
 * @param const char *_preset, preset value
 */
YroRawEffectPlugin::YroRawEffectPlugin(const char *_name, const char *_presets) {
	initialize(_name);
	/**
	 * discover presets
	 */
	presets = new YroPresets(_presets);
	preset = 0;
	setPreset(preset);
}

YroRawEffectPlugin::~YroRawEffectPlugin() {
	free((void *) name);
}

/**
 * general setPReset method based
 * on getn member, be carefull about the declaration order
 */
void YroRawEffectPlugin::setPreset(int _preset, int forceCleanup) {
	if (_preset < presets->size()) {
		int size = presets->get(_preset)->size();
		for (int i = 0; i < size; i++) {
			/**
			 * setPreset is always (and must)
			 * the first getter/setter for on effect plugin
			 */
			this->setInt(i + 1, presets->get(_preset)->get(i));
		}
		preset = _preset;
		if (forceCleanup || forcedCleanup) {
			LOG->debug("Effect %s, fix preset to %d and cleanup", name,
					_preset);
			cleanup();
		} else {
			LOG->debug("Effect %s, fix preset to %d", name, _preset);
		}
	}

}

/**
 * link this widget to this field
 * using map for indexing one way, and the other
 */
void YroRawEffectPlugin::subscribe(int index, wxSpinCtrl* widget) {
	mapSpinCtrlIndex[widget] = index;
	mapIndexSpinCtrl[index] = widget;
	widget->SetRange(0, 65535);
	widget->SetValue(this->getInt(index));
}

/**
 * wxSpinCtrl advertise for change
 */
void YroRawEffectPlugin::onChange(wxSpinCtrl* widget) {
	setInt(mapSpinCtrlIndex[widget], widget->GetValue());
}

/**
 * link this widget to this field
 * using map for indexing one way, and the other
 */
void YroRawEffectPlugin::subscribe(int index, wxChoice* widget) {
	mapChoiceIndex[widget] = index;
	mapIndexChoice[index] = widget;
	widget->SetSelection(this->getInt(index));
}

/**
 * wxSpinCtrl advertise for change
 */
void YroRawEffectPlugin::onChange(wxChoice* widget) {
	setInt(mapChoiceIndex[widget], widget->GetSelection());
}

/**
 * link this widget to this field
 * using map for indexing one way, and the other
 */
void YroRawEffectPlugin::subscribe(int index, wxSlider* widget) {
	mapSliderIndex[widget] = index;
	mapIndexSlider[index] = widget;
	widget->SetValue(this->getInt(index));
}

/**
 * wxSpinCtrl advertise for change
 */
void YroRawEffectPlugin::onChange(wxSlider* widget) {
	LOG->debug("onChange(wxSlider 0x%08x) => set %d", widget,
			widget->GetValue());
	setInt(mapSliderIndex[widget], widget->GetValue());
}

/**
 * link this widget to this field
 * using map for indexing one way, and the other
 */
void YroRawEffectPlugin::subscribe(int index, wxCheckBox* widget) {
	mapCheckBoxIndex[widget] = index;
	mapIndexCheckBox[index] = widget;
	widget->SetValue(this->getInt(index));
}

/**
 * wxSpinCtrl advertise for change
 */
void YroRawEffectPlugin::onChange(wxCheckBox* widget) {
	setInt(mapCheckBoxIndex[widget], widget->GetValue());
}

/**
 * Effect advertise for change
 */
void YroRawEffectPlugin::onChange(int index) {
	/**
	 * TODO LOG->debug("onChange(%d) => set %d", index, getInt(index));
	 */
	if (mapIndexSpinCtrl.find(index) != mapIndexSpinCtrl.end()) {
		mapIndexSpinCtrl[index]->SetValue(getInt(index));
		return;
	}
	if (mapIndexChoice.find(index) != mapIndexChoice.end()) {
		mapIndexChoice[index]->SetSelection(getInt(index));
		return;
	}
	if (mapIndexSlider.find(index) != mapIndexSlider.end()) {
		mapIndexSlider[index]->SetValue(getInt(index));
		return;
	}
	if (mapIndexCheckBox.find(index) != mapIndexCheckBox.end()) {
		mapIndexCheckBox[index]->SetValue(getInt(index));
		return;
	}
}

/**
 * generic getter
 */
int YroRawEffectPlugin::getInt(int index) {
	switch (index) {
	case 0:
		return get0();
	case 1:
		return get1();
	case 2:
		return get2();
	case 3:
		return get3();
	case 4:
		return get4();
	case 5:
		return get5();
	case 6:
		return get6();
	case 7:
		return get7();
	case 8:
		return get8();
	case 9:
		return get9();
	case 10:
		return get10();
	case 11:
		return get11();
	case 12:
		return get12();
	case 13:
		return get13();
	case 14:
		return get14();
	case 15:
		return get15();
	case 16:
		return get16();
	case 17:
		return get17();
	case 18:
		return get18();
	case 19:
		return get19();
	}
	return 0;
}

/**
 * generic setter
 */
void YroRawEffectPlugin::setInt(int index, int value) {
	switch (index) {
	case 0:
		set0(value);
		break;
	case 1:
		set1(value);
		break;
	case 2:
		set2(value);
		break;
	case 3:
		set3(value);
		break;
	case 4:
		set4(value);
		break;
	case 5:
		set5(value);
		break;
	case 6:
		set6(value);
		break;
	case 7:
		set7(value);
		break;
	case 8:
		set8(value);
		break;
	case 9:
		set9(value);
		break;
	case 10:
		set10(value);
		break;
	case 11:
		set11(value);
		break;
	case 12:
		set12(value);
		break;
	case 13:
		set13(value);
		break;
	case 14:
		set14(value);
		break;
	case 15:
		set15(value);
		break;
	case 16:
		set16(value);
		break;
	case 17:
		set17(value);
		break;
	case 18:
		set18(value);
		break;
	case 19:
		set19(value);
		break;
	}
}

/**
 * active / inative status
 * and toggle member
 */
int YroRawEffectPlugin::getActive() const {
	return active;
}

int YroRawEffectPlugin::getBehaviour() {
	return _applyEffectOnInput;
}

const char* YroRawEffectPlugin::getName() {
	return name;
}

void YroRawEffectPlugin::setActive(int active) {
	this->active = active;
}
void std::YroRawEffectPlugin::toggle() {
	if (getActive() == 1) {
		setActive(0);
	} else {
		setActive(1);
	}
}
/**
 * processing
 */
void YroRawEffectPlugin::render(jack_nframes_t nframes, float* inLeft, float* inRight) {
	getLogger()->error("Not implemented");
}
void YroRawEffectPlugin::render(float* inLeft, float* inRight) {
	render(iPERIOD,inLeft,inRight);
}

} /* namespace std */
