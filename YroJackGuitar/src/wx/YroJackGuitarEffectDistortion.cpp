#include <jack/YroEffectFactory.h>
#include "YroJackGuitarEffectDistortion.h"

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

void YroJackGuitarEffectDistortion::OnChoice(wxCommandEvent& event) {
	effect->onChange(m_preset);
}
