#include <wx/YroJackGuitarEffectCtrl.h>

using namespace std;

extern wxFont _defaultFont;

YroJackGuitarEffectCtrl::YroJackGuitarEffectCtrl(wxWindow* parent,
		YroEffectPlugin *_effect, YroJackGuitarEffectCtrl *_previous, wxFrame *_frame) :
		EffectCtrl(parent) {
	/**
	 * fix default font for application
	 */
	SetFont( _defaultFont );
	LOG = YroLogger::instance();
	effect = _effect;
	previous = _previous;
	frame = _frame;
	m_effectName->SetLabel(wxString::FromUTF8(effect->getName()));
	/**
	 * fix position according to previous
	 * effect
	 */
	if (_previous != 0) {
		int x, y;
		_previous->GetPosition(&x, &y);
		int w, h;
		_previous->GetBestSize(&w, &h);
		wxPoint previousEffectPosition(x + w + 20, y);
		this->SetPosition(previousEffectPosition);
	}
}

void YroJackGuitarEffectCtrl::onLeftDown(wxMouseEvent& event) {
	/**
	 * enter in drag mode
	 * capture current mouse position and compute
	 * adjustement
	 */
	event.Skip();
}

void YroJackGuitarEffectCtrl::onLeftUp(wxMouseEvent& event) {
	/**
	 * leave drag mode
	 */
	event.Skip();
}

/**
 * handle mouse event
 * @param wxMouseEvent& event
 */
void YroJackGuitarEffectCtrl::onMouseEvents(wxMouseEvent& event) {
	/**
	 * detect initial dragging
	 */
	if(event.LeftDown()) {
		mouseOnScreenOnBeginDragging = GetParent()->ScreenToClient( wxGetMousePosition() );
		wxPoint pos = GetPosition();
		mouseOnScreenOnBeginDragging.x = mouseOnScreenOnBeginDragging.x - pos.x;
		mouseOnScreenOnBeginDragging.y = mouseOnScreenOnBeginDragging.y - pos.y;
		return;
	}
	/**
	 * handle dragging
	 */
	if(event.Dragging()) {
		wxPoint mouseOnScreen = wxGetMousePosition();
		mouseOnScreen.x = mouseOnScreen.x - mouseOnScreenOnBeginDragging.x;
		mouseOnScreen.y = mouseOnScreen.y - mouseOnScreenOnBeginDragging.y;
		this->Move(GetParent()->ScreenToClient( mouseOnScreen ) );
		this->GetParent()->Refresh(true);
		return;
	}
	event.Skip();
}

void YroJackGuitarEffectCtrl::onActiveEffect(wxCommandEvent& event) {
	/**
	 * active or not this effect
	 */
	if(m_enableEffect->GetValue()) {
		effect->setActive(1);
	} else {
		effect->setActive(0);
	}
}

void YroJackGuitarEffectCtrl::onToggleEffectProperty( wxCommandEvent& event ) {
	if(m_toggleWindow->GetValue()) {
		m_toggleWindow->SetLabel(wxT("Property on"));
		frame->Show(true);
		return;
	} else {
		m_toggleWindow->SetLabel(wxT("Property off"));
		frame->Show(false);
		return;
	}
}

void YroJackGuitarEffectCtrl::paintLink(wxPaintDC &dc) {
	if(previous != 0) {
		wxColour color;
		color.Set(wxT("#000000"));
		dc.SetPen(wxPen(color, 3, wxSOLID));
		wxPoint from = getInputPosition();
		wxPoint to = previous->getOutputPosition();
		dc.DrawLine(from,to);
	}
}

wxPoint YroJackGuitarEffectCtrl::getInputPosition() {
	wxPoint from = GetPosition();
	wxSize size = GetSize();
	from.y += size.y / 2;
	return from;
}

wxPoint YroJackGuitarEffectCtrl::getOutputPosition() {
	wxPoint from = GetPosition();
	wxSize size = GetSize();
	from.x += size.x;
	from.y += size.y / 2;
	return from;
}



