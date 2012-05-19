#include <wx/YroJackGuitarEffectDistortion.h>

using namespace std;

YroJackGuitarEffectDistortion::YroJackGuitarEffectDistortion(wxWindow* parent, Distortion * _effect) :
		EffectDistortion(parent) {
	effect = _effect;
	effect->subscribe(Distortion::_preset, m_preset);
	effect->subscribe(Distortion::_type, m_type);
	effect->subscribe(Distortion::_lrcross, m_lrcross);
	effect->subscribe(Distortion::_drive, m_drive);
	effect->subscribe(Distortion::_level, m_level);
	effect->subscribe(Distortion::_negate, m_negate);
	effect->subscribe(Distortion::_prefiltering, m_prefilter);
	effect->subscribe(Distortion::_stereo, m_stereo);
	effect->subscribe(Distortion::_panning, m_panning);
	effect->subscribe(Distortion::_octave, m_suboctave);
	effect->subscribe(Distortion::_lpf, m_lpf);
	effect->subscribe(Distortion::_hpf, m_hpf);
}

void YroJackGuitarEffectDistortion::onActivate(wxCommandEvent& event) {
	effect->toggle();
	if(m_toggle->GetValue()) m_toggle->SetLabel(wxT("On"));
	else  m_toggle->SetLabel(wxT("Off"));
}

void YroJackGuitarEffectDistortion::onChangePreset(wxCommandEvent& event) {
	effect->onChange(m_preset);
}

void YroJackGuitarEffectDistortion::onChangeWetDry(wxScrollEvent& event) {
}

void YroJackGuitarEffectDistortion::onChangeLRCross(wxScrollEvent& event) {
	effect->onChange(m_lrcross);
}

void YroJackGuitarEffectDistortion::onChangeDrive(wxScrollEvent& event) {
	effect->onChange(m_drive);
}

void YroJackGuitarEffectDistortion::onChangeLevel(wxScrollEvent& event) {
	effect->onChange(m_level);
}

void YroJackGuitarEffectDistortion::onChangeType(wxCommandEvent& event) {
	effect->onChange(m_type);
}

void YroJackGuitarEffectDistortion::onChangeNegate(wxCommandEvent& event) {
	effect->onChange(m_negate);
}

void YroJackGuitarEffectDistortion::onChangePrefilter(wxCommandEvent& event) {
	effect->onChange(m_prefilter);
}

void YroJackGuitarEffectDistortion::onChangeStereo(wxCommandEvent& event) {
	effect->onChange(m_stereo);
}

void YroJackGuitarEffectDistortion::onChangePanning(wxScrollEvent& event) {
	effect->onChange(m_panning);
}

void YroJackGuitarEffectDistortion::onChangeSubOctave(wxScrollEvent& event) {
	effect->onChange(m_suboctave);
}

void YroJackGuitarEffectDistortion::onChangeLpf(wxScrollEvent& event) {
	effect->onChange(m_lpf);
}

void YroJackGuitarEffectDistortion::onChangeHpf(wxScrollEvent& event) {
	effect->onChange(m_hpf);
}

