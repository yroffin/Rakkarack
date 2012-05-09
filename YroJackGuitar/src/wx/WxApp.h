///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Dec 21 2009)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __WxApp__
#define __WxApp__

#include <wx/string.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/menu.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/frame.h>
#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/spinctrl.h>
#include <wx/choice.h>

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class MainWindow
///////////////////////////////////////////////////////////////////////////////
class MainWindow : public wxFrame 
{
	private:
	
	protected:
		wxMenuBar* m_menubar1;
		wxMenu* m_menu2;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnClose( wxCloseEvent& event ) { event.Skip(); }
		virtual void OnConnect( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		
		MainWindow( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 500,300 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );
		~MainWindow();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class SignalFrame
///////////////////////////////////////////////////////////////////////////////
class SignalFrame : public wxFrame 
{
	private:
	
	protected:
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnPaint( wxPaintEvent& event ) { event.Skip(); }
		
	
	public:
		
		SignalFrame( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Signal Window"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 500,300 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );
		~SignalFrame();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class EffectDistortion
///////////////////////////////////////////////////////////////////////////////
class EffectDistortion : public wxFrame 
{
	private:
	
	protected:
		wxStaticText* m_staticText1;
		wxSpinCtrl* m_Pdrive;
		wxStaticText* m_staticText2;
		wxChoice* m_Preset;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnSpinCtrl( wxSpinEvent& event ) { event.Skip(); }
		virtual void OnSpinCtrlText( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnChoice( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		
		EffectDistortion( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Distortion"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 500,300 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );
		~EffectDistortion();
	
};

#endif //__WxApp__
