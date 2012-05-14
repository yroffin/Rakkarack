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
#include <wx/spinctrl.h>
#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/choice.h>
#include <wx/slider.h>
#include <wx/checkbox.h>

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
		wxSpinCtrl* m_spinCtrl1;
		
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
		wxStaticText* m_staticText2;
		wxChoice* m_preset;
		wxStaticText* m_staticText1;
		wxSlider* m_wetdry;
		wxStaticText* m_staticText3;
		wxSlider* m_lrcross;
		wxStaticText* m_staticText11;
		wxSlider* m_drive;
		wxStaticText* m_staticText21;
		wxSlider* m_level;
		wxStaticText* m_staticText211;
		wxChoice* m_type;
		wxCheckBox* m_negate;
		wxCheckBox* m_prefilter;
		wxCheckBox* m_stereo;
		
		wxStaticText* m_staticText2111;
		wxSlider* m_panning;
		wxStaticText* m_staticText21111;
		wxSlider* m_suboctave;
		wxStaticText* m_staticText21112;
		wxSlider* m_lpf;
		wxStaticText* m_staticText21113;
		wxSlider* m_hpf;
		
		// Virtual event handlers, overide them in your derived class
		virtual void onChangePreset( wxCommandEvent& event ) { event.Skip(); }
		virtual void onChangeWetDry( wxScrollEvent& event ) { event.Skip(); }
		virtual void onChangeLRCross( wxScrollEvent& event ) { event.Skip(); }
		virtual void onChangeDrive( wxScrollEvent& event ) { event.Skip(); }
		virtual void onChangeLevel( wxScrollEvent& event ) { event.Skip(); }
		virtual void onChangeType( wxCommandEvent& event ) { event.Skip(); }
		virtual void onChangeNegate( wxCommandEvent& event ) { event.Skip(); }
		virtual void onChangePrefilter( wxCommandEvent& event ) { event.Skip(); }
		virtual void onChangeStereo( wxCommandEvent& event ) { event.Skip(); }
		virtual void onChangePanning( wxScrollEvent& event ) { event.Skip(); }
		virtual void onChangeSubOctave( wxScrollEvent& event ) { event.Skip(); }
		virtual void onChangeLpf( wxScrollEvent& event ) { event.Skip(); }
		virtual void onChangeHpf( wxScrollEvent& event ) { event.Skip(); }
		
	
	public:
		
		EffectDistortion( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Distortion"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 500,600 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );
		~EffectDistortion();
	
};

#endif //__WxApp__
