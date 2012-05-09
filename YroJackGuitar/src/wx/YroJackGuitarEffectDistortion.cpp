#include <jack/YroEffectFactory.h>
#include "YroJackGuitarEffectDistortion.h"

using namespace std;

YroJackGuitarEffectDistortion::YroJackGuitarEffectDistortion(wxWindow* parent) :
		EffectDistortion(parent) {
	/**
	 * TODO
	effect = (std::YroDistortion *) YroEffectFactory::instance()->getEffect("distortion#1");
	m_Pdrive->SetRange(0,100);
	m_Pdrive->SetValue(effect->getPdrive());
	m_Preset->SetSelection(effect->getPreset());
	 */
}

void YroJackGuitarEffectDistortion::OnSpinCtrl(wxSpinEvent& event) {
	effect->setPdrive(m_Pdrive->GetValue());
}

void YroJackGuitarEffectDistortion::OnSpinCtrlText(wxCommandEvent& event) {
	effect->setPdrive(m_Pdrive->GetValue());
}

void YroJackGuitarEffectDistortion::OnChoice(wxCommandEvent& event) {
	effect->setPreset(m_Preset->GetSelection());
}
