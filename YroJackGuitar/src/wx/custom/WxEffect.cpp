/*
 YroJackGuitar - a software synthesizer based on excelent work
 of Rakkarack team
 
 Copyright (C) 2002-2005 Yannick Roffin
 Author: Yannick Roffin

 This program is free software; you can redistribute it and/or modify
 it under the terms of version 2 of the GNU General Public License 
 as published by the Free Software Foundation.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License (version 2) for more details.

 You should have received a copy of the GNU General Public License (version 2)
 along with this program; if not, write to the Free Software Foundation,
 Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
 
 WxEffect.cpp
 */

#include <wx/tooltip.h>
#include <wx/custom/WxEffect.h>

namespace std {

BEGIN_EVENT_TABLE(WxEffect, wxPanel)

    EVT_MOTION(WxEffect::mouseMoved)
    EVT_LEFT_DOWN(WxEffect::mouseDown)
    EVT_LEFT_UP(WxEffect::mouseReleased)
    EVT_RIGHT_DOWN(WxEffect::rightClick)
    EVT_LEAVE_WINDOW(WxEffect::mouseLeftWindow)
    EVT_KEY_DOWN(WxEffect::keyPressed)
    EVT_KEY_UP(WxEffect::keyReleased)
    EVT_MOUSEWHEEL(WxEffect::mouseWheelMoved)

    // catch paint events
    EVT_PAINT(WxEffect::paintEvent)
END_EVENT_TABLE()

WxEffect::WxEffect(wxWindow *parent, YroEffectPlugin *_effect,
		WxEffect *_previous,
		wxWindowID id, const wxPoint& pos,
		const wxSize& size, long style, const wxValidator& validator,
		const wxString& name) {
	effect = _effect;
	previous = _previous;
	wxSize s(320,200);
	Create(parent, id, pos, s, style, name);
	/**
	 * fix position according to previous
	 * effect
	 */
	if(previous != 0) {
		int x,y;
		previous->GetPosition(&x, &y);
		wxPoint previousEffectPosition(x + 100,y);
		this->SetPosition(previousEffectPosition);
	}

	text.sprintf(wxT("TODO: %s"), "todo");
	this->SetToolTip(text);
	defaultBitmap = new wxBitmap( wxT("resources/Processus.png"), wxBITMAP_TYPE_ANY );
}

WxEffect::~WxEffect() {
	delete defaultBitmap;
}

/*
 * Called by the system of by wxWidgets when the panel needs
 * to be redrawn. You can also trigger this call by
 * calling Refresh()/Update().
 */

void WxEffect::paintEvent(wxPaintEvent & evt)
{
    // depending on your system you may need to look at double-buffered dcs
    wxPaintDC dc(this);
    render(dc);
}

/*
 * Alternatively, you can use a clientDC to paint on the panel
 * at any time. Using this generally does not free you from
 * catching paint events, since it is possible that e.g. the window
 * manager throws away your drawing when the window comes to the
 * background, and expects you will redraw it when the window comes
 * back (by sending a paint event).
 */
void WxEffect::paintNow()
{
    // depending on your system you may need to look at double-buffered dcs
    wxClientDC dc(this);
    render(dc);
}

/*
 * Here we do the actual rendering. I put it in a separate
 * method so that it can work no matter what type of DC
 * (e.g. wxPaintDC or wxClientDC) is used.
 */
void WxEffect::render(wxDC&  dc)
{
    if (pressedDown)
        dc.SetBrush( *wxRED_BRUSH );
    else
        dc.SetBrush( *wxGREY_BRUSH );

    dc.DrawRectangle( 0, 0, 320, 200 );
    dc.DrawText( text, 20, 15 );
    dc.DrawBitmap(*defaultBitmap, 0, 0);
}

void WxEffect::mouseDown(wxMouseEvent& event)
{
    pressedDown = true;
    paintNow();
}
void WxEffect::mouseReleased(wxMouseEvent& event)
{
    pressedDown = false;
    paintNow();

    wxMessageBox( wxT("You pressed a custom button") );
}
void WxEffect::mouseLeftWindow(wxMouseEvent& event)
{
    if (pressedDown)
    {
        pressedDown = false;
        paintNow();
    }
}

// currently unused events
void WxEffect::mouseMoved(wxMouseEvent& event) {}
void WxEffect::mouseWheelMoved(wxMouseEvent& event) {}
void WxEffect::rightClick(wxMouseEvent& event) {}
void WxEffect::keyPressed(wxKeyEvent& event) {}
void WxEffect::keyReleased(wxKeyEvent& event) {}

} /* namespace std */
