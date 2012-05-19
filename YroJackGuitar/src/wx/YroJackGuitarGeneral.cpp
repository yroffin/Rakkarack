#include <wx/YroJackGuitarGeneral.h>

YroJackGuitarGeneral::YroJackGuitarGeneral(wxWindow* parent, YroAmpli *_inputAmpli, YroAmpli *_outputAmpli) :
		General(parent) {
	inputAmpli = _inputAmpli;
	outputAmpli = _outputAmpli;
	inputAmpli->subscribe(YroAmpli::_factor, m_input);
	outputAmpli->subscribe(YroAmpli::_factor, m_output);
}

void YroJackGuitarGeneral::onUpdateInputAmpli(wxScrollEvent& event) {
	inputAmpli->onChange(m_input);
}

void YroJackGuitarGeneral::onUpdateOutputAmpli(wxScrollEvent& event) {
	outputAmpli->onChange(m_output);
}
