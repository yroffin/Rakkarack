#include <wx/YroJackGuitarGeneral.h>

extern wxFont _defaultFont;

YroJackGuitarGeneral::YroJackGuitarGeneral(wxWindow* parent, YroAmpli *_inputAmpli) :
		General(parent) {
	/**
	 * fix default font for application
	 */
	SetFont( _defaultFont );
	/**
	 * input
	 */
	inputAmpli = _inputAmpli;
	inputAmpli->setActive(1);
	inputAmpli->subscribe(YroAmpli::_factor, m_input);
}

void YroJackGuitarGeneral::onUpdateInputAmpli(wxScrollEvent& event) {
	inputAmpli->onChange(m_input);
}
