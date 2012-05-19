/*
 * YroEffectPlugin.cpp
 *
 *  Created on: 5 mai 2012
 *      Author: yannick
 */

#include <plugins/YroEffectPlugin.h>

using namespace std;

YroEffectPlugin::YroEffectPlugin(const char *_name) {
	name = strdup(_name);
	iPERIOD = helper->getIntegerPeriod();
	iSAMPLE_RATE = helper->getIntegerSampleRate();
	fPERIOD = helper->getFloatPeriod();
	fSAMPLE_RATE = helper->getFloatPeriod();
	cSAMPLE_RATE = 1.0f / fSAMPLE_RATE;
	preset = 0;
	audioSampleFactory = YroAudioSampleFactory::instance();
	active = 0;
}

/**
 * default constructor
 * @param const char *_name, name of this effect
 * @param const char *_preset, preset value
 */
YroEffectPlugin::YroEffectPlugin(const char *_name, const char *_preset) {
	/**
	 * discover presets
	 */
	presets = new YroPresets(_preset);
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
	iPERIOD = helper->getIntegerPeriod();
	iSAMPLE_RATE = helper->getIntegerSampleRate();
	fPERIOD = helper->getFloatPeriod();
	fSAMPLE_RATE = helper->getFloatPeriod();
	cSAMPLE_RATE = 1.0f / fSAMPLE_RATE;
	preset = 0;
}

YroEffectPlugin::~YroEffectPlugin() {
	free((void *) name);
}

/**
 * general setPReset method based
 * on getn member, be carefull about the declaration order
 */
void YroEffectPlugin::setPreset(int _preset) {
 	if (_preset < presets->size()) {
 		int size = presets->get(_preset)->size();
 		for(int i=0;i<size;i++) {
 			/**
 			 * setPreset is always (and must)
 			 * the first getter/setter for on effect plugin
 			 */
 			this->setInt(i+1, presets->get(_preset)->get(i));
 		}
 		preset = _preset;
 		cleanup();
 	}
}

/**
 * getter
 */
float *YroEffectPlugin::getOutLeft() {
	return efxoutl;
}

float *YroEffectPlugin::getOutRight() {
	return efxoutr;
}

void YroEffectPlugin::setOutLeft(float *value) {
	efxoutl = value;
}

void YroEffectPlugin::setOutRight(float *value) {
	efxoutr = value;
}

/**
 * link this widget to this field
 * using map for indexing one way, and the other
 */
void YroEffectPlugin::subscribe(int index, wxSpinCtrl *widget) {
	mapSpinCtrlIndex[widget] = index;
	mapIndexSpinCtrl[index] = widget;
	widget->SetRange(0, 65535);
	widget->SetValue(this->getInt(index));
}

/**
 * wxSpinCtrl advertise for change
 */
void YroEffectPlugin::onChange(wxSpinCtrl *widget) {
	setInt(mapSpinCtrlIndex[widget], widget->GetValue());
}

/**
 * link this widget to this field
 * using map for indexing one way, and the other
 */
void YroEffectPlugin::subscribe(int index, wxChoice *widget) {
	mapChoiceIndex[widget] = index;
	mapIndexChoice[index] = widget;
	widget->SetSelection(this->getInt(index));
}

/**
 * wxSpinCtrl advertise for change
 */
void YroEffectPlugin::onChange(wxChoice *widget) {
	setInt(mapChoiceIndex[widget], widget->GetSelection());
}

/**
 * link this widget to this field
 * using map for indexing one way, and the other
 */
void YroEffectPlugin::subscribe(int index, wxSlider *widget) {
	mapSliderIndex[widget] = index;
	mapIndexSlider[index] = widget;
	widget->SetValue(this->getInt(index));
}

/**
 * wxSpinCtrl advertise for change
 */
void YroEffectPlugin::onChange(wxSlider *widget) {
	LOG->debug("onChange(wxSlider 0x%08x) => set %d", widget, widget->GetValue());
	setInt(mapSliderIndex[widget], widget->GetValue());
}

/**
 * link this widget to this field
 * using map for indexing one way, and the other
 */
void YroEffectPlugin::subscribe(int index, wxCheckBox *widget) {
	mapCheckBoxIndex[widget] = index;
	mapIndexCheckBox[index] = widget;
	widget->SetValue(this->getInt(index));
}

/**
 * wxSpinCtrl advertise for change
 */
void YroEffectPlugin::onChange(wxCheckBox *widget) {
	setInt(mapCheckBoxIndex[widget], widget->GetValue());
}

/**
 * Effect advertise for change
 */
void YroEffectPlugin::onChange(int index) {
	LOG->debug("onChange(%d) => set %d", index, getInt(index));
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
int YroEffectPlugin::getInt(int index) {
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
void YroEffectPlugin::setInt(int index, int value) {
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

int YroEffectPlugin::getActive() const {
	return active;
}

void std::YroEffectPlugin::toggle() {
	if(getActive() == 1) {
		setActive(0);
	} else {
		setActive(1);
	}
}

void YroEffectPlugin::setActive(int active) {
	this->active = active;
}

