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
}

YroEffectPlugin::~YroEffectPlugin() {
	free((void *) name);
}

void YroEffectPlugin::setPreset(int npreset) {
	preset = npreset;
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
	widget->SetRange(0,65535);
	widget->SetValue(this->getInt(index));
}

/**
 * wxSpinCtrl advertise for change
 */
void YroEffectPlugin::onChange(wxSpinCtrl *widget) {
	setInt(mapSpinCtrlIndex[widget],widget->GetValue());
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
	setInt(mapChoiceIndex[widget],widget->GetSelection());
}

/**
 * Effect advertise for change
 */
void YroEffectPlugin::onChange(int index) {
	if(mapIndexSpinCtrl.find(index) != mapIndexSpinCtrl.end()) {
		mapIndexSpinCtrl[index]->SetValue(getInt(index));
		return;
	}
	if(mapIndexChoice.find(index) != mapIndexChoice.end()) {
		mapIndexChoice[index]->SetSelection(getInt(index));
		return;
	}
}

/**
 * generic getter
 */
int YroEffectPlugin::getInt(int index) {
	switch(index) {
		case 0: return get0();
	}
	return 0;
}

/**
 * generic setter
 */
void YroEffectPlugin::setInt(int index, int value) {
	switch(index) {
		case 0: set0(value); break;
	}
}
