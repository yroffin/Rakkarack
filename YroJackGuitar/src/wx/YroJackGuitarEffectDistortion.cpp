#include <jack/YroEffectFactory.h>
#include "YroJackGuitarEffectDistortion.h"

using namespace std;

YroJackGuitarEffectDistortion::YroJackGuitarEffectDistortion(wxWindow* parent, Distortion * _effect) :
		EffectDistortion(parent) {
	effect = _effect;
	effect->subscribe(Distortion::preset, m_Preset);
	effect->subscribe(Distortion::drive, m_Pdrive);
}

void YroJackGuitarEffectDistortion::OnSpinCtrl(wxSpinEvent& event) {
	effect->onChange(m_Pdrive);
}

void YroJackGuitarEffectDistortion::OnSpinCtrlText(wxCommandEvent& event) {
	effect->onChange(m_Pdrive);
}

void YroJackGuitarEffectDistortion::OnChoice(wxCommandEvent& event) {
	effect->onChange(m_Preset);
}
