#include <wx/YroJackGuitarEffectExpander.h>

using namespace std;

extern wxFont _defaultFont;

YroJackGuitarEffectExpander::YroJackGuitarEffectExpander(wxWindow* parent, Expander *_effect) :
		EffectExpander(parent) {
	/**
	 * fix default font for application
	 */
	SetFont( _defaultFont );
	effect = _effect;
	effect->subscribe(Expander::_preset, m_preset);
	effect->subscribe(Expander::_attack, m_atime);
	effect->subscribe(Expander::_decay, m_rtime);
	effect->subscribe(Expander::_shape, m_shape);
	effect->subscribe(Expander::_threshold, m_thrhold);
	effect->subscribe(Expander::_level, m_level);
	effect->subscribe(Expander::_lpf, m_lpf);
	effect->subscribe(Expander::_hpf, m_hpf);
}

void YroJackGuitarEffectExpander::onActivate(wxCommandEvent& event) {
	effect->toggle();
	if(m_toggle->GetValue()) m_toggle->SetLabel(wxT("On"));
	else  m_toggle->SetLabel(wxT("Off"));
}

void YroJackGuitarEffectExpander::onChangePreset(wxCommandEvent& event) {
	effect->onChange(m_preset);
}

void YroJackGuitarEffectExpander::onChangeAttack(wxScrollEvent& event) {
	effect->onChange(m_atime);
}

void YroJackGuitarEffectExpander::onChangeRelease(wxScrollEvent& event) {
	effect->onChange(m_rtime);
}

void YroJackGuitarEffectExpander::onChangeShape(wxScrollEvent& event) {
	effect->onChange(m_shape);
}

void YroJackGuitarEffectExpander::onChangeThreshold(wxScrollEvent& event) {
	effect->onChange(m_thrhold);
}

void YroJackGuitarEffectExpander::onChangeLevel(wxScrollEvent& event) {
	effect->onChange(m_level);
}

void YroJackGuitarEffectExpander::onChangeLpf(wxScrollEvent& event) {
	effect->onChange(m_lpf);
}

void YroJackGuitarEffectExpander::onChangeHpf(wxScrollEvent& event) {
	effect->onChange(m_hpf);
}
