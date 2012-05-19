#include <wx/YroJackGuitarGeneral.h>

YroJackGuitarGeneral::YroJackGuitarGeneral(wxWindow* parent, YroAmpli *_inputAmpli, YroAmpli *_outputAmpli) :
		General(parent) {
	/**
	 * input
	 */
	inputAmpli = _inputAmpli;
	inputAmpli->setActive(1);
	inputAmpli->subscribe(YroAmpli::_factor, m_input);
	/**
	 * output
	 */
	outputAmpli = _outputAmpli;
	outputAmpli->setActive(1);
	outputAmpli->subscribe(YroAmpli::_factor, m_output);
}

void YroJackGuitarGeneral::onUpdateInputAmpli(wxScrollEvent& event) {
	inputAmpli->onChange(m_input);
}

void YroJackGuitarGeneral::onUpdateOutputAmpli(wxScrollEvent& event) {
	outputAmpli->onChange(m_output);
}
