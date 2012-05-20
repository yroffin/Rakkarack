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
#include <wx/scrolwin.h>
#include <wx/panel.h>
#include <wx/notebook.h>
#include <wx/sizer.h>
#include <wx/frame.h>
#include <wx/tglbtn.h>
#include <wx/stattext.h>
#include <wx/choice.h>
#include <wx/slider.h>
#include <wx/checkbox.h>
#include <wx/statbmp.h>

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
		wxNotebook* m_notebook;
		wxScrolledWindow* m_panelEffects;
		wxPanel* m_panel6;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnClose( wxCloseEvent& event ) { event.Skip(); }
		virtual void OnConnect( wxCommandEvent& event ) { event.Skip(); }
		virtual void onPaintEffects( wxPaintEvent& event ) { event.Skip(); }
		
	
	public:
		
		MainWindow( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( -1,-1 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );
		~MainWindow();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class SignalFrame
///////////////////////////////////////////////////////////////////////////////
class SignalFrame : public wxFrame 
{
	private:
	
	protected:
		wxToggleButton* m_toggle;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnPaint( wxPaintEvent& event ) { event.Skip(); }
		virtual void onActivate( wxCommandEvent& event ) { event.Skip(); }
		
	
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
		wxToggleButton* m_toggle;
		wxStaticText* m_staticText1;
		wxStaticText* m_staticText3;
		wxStaticText* m_staticText11;
		wxStaticText* m_staticText21;
		wxStaticText* m_staticText211;
		wxStaticText* m_staticText2111;
		wxStaticText* m_staticText21111;
		wxStaticText* m_staticText21112;
		wxStaticText* m_staticText21113;
		wxChoice* m_preset;
		wxSlider* m_wetdry;
		wxSlider* m_lrcross;
		wxSlider* m_drive;
		wxSlider* m_level;
		wxChoice* m_type;
		wxSlider* m_panning;
		wxSlider* m_suboctave;
		wxSlider* m_lpf;
		wxSlider* m_hpf;
		
		wxCheckBox* m_negate;
		wxCheckBox* m_prefilter;
		wxCheckBox* m_stereo;
		
		// Virtual event handlers, overide them in your derived class
		virtual void onActivate( wxCommandEvent& event ) { event.Skip(); }
		virtual void onChangePreset( wxCommandEvent& event ) { event.Skip(); }
		virtual void onChangeWetDry( wxScrollEvent& event ) { event.Skip(); }
		virtual void onChangeLRCross( wxScrollEvent& event ) { event.Skip(); }
		virtual void onChangeDrive( wxScrollEvent& event ) { event.Skip(); }
		virtual void onChangeLevel( wxScrollEvent& event ) { event.Skip(); }
		virtual void onChangeType( wxCommandEvent& event ) { event.Skip(); }
		virtual void onChangePanning( wxScrollEvent& event ) { event.Skip(); }
		virtual void onChangeSubOctave( wxScrollEvent& event ) { event.Skip(); }
		virtual void onChangeLpf( wxScrollEvent& event ) { event.Skip(); }
		virtual void onChangeHpf( wxScrollEvent& event ) { event.Skip(); }
		virtual void onChangeNegate( wxCommandEvent& event ) { event.Skip(); }
		virtual void onChangePrefilter( wxCommandEvent& event ) { event.Skip(); }
		virtual void onChangeStereo( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		
		EffectDistortion( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Distortion"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( -1,-1 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );
		~EffectDistortion();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class EffectChorus
///////////////////////////////////////////////////////////////////////////////
class EffectChorus : public wxFrame 
{
	private:
	
	protected:
		wxToggleButton* m_toggle;
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
		
		
		wxStaticText* m_staticText2111;
		wxSlider* m_panning;
		
		wxStaticText* m_staticText21111;
		wxSlider* m_suboctave;
		
		wxStaticText* m_staticText21112;
		wxSlider* m_lpf;
		
		wxStaticText* m_staticText21113;
		wxSlider* m_hpf;
		
		// Virtual event handlers, overide them in your derived class
		virtual void onActivate( wxCommandEvent& event ) { event.Skip(); }
		virtual void onChangePreset( wxCommandEvent& event ) { event.Skip(); }
		virtual void onChangeWetDry( wxScrollEvent& event ) { event.Skip(); }
		virtual void onChangeLRCross( wxScrollEvent& event ) { event.Skip(); }
		virtual void onChangeDrive( wxScrollEvent& event ) { event.Skip(); }
		virtual void onChangeLevel( wxScrollEvent& event ) { event.Skip(); }
		virtual void onChangeType( wxCommandEvent& event ) { event.Skip(); }
		virtual void onChangeNegate( wxCommandEvent& event ) { event.Skip(); }
		virtual void onChangePanning( wxScrollEvent& event ) { event.Skip(); }
		virtual void onChangeSubOctave( wxScrollEvent& event ) { event.Skip(); }
		virtual void onChangeLpf( wxScrollEvent& event ) { event.Skip(); }
		virtual void onChangeHpf( wxScrollEvent& event ) { event.Skip(); }
		
	
	public:
		
		EffectChorus( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Chorus"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 500,600 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );
		~EffectChorus();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class EffectExpander
///////////////////////////////////////////////////////////////////////////////
class EffectExpander : public wxFrame 
{
	private:
	
	protected:
		wxToggleButton* m_toggle;
		wxStaticText* m_staticText1;
		wxStaticText* m_staticText3;
		wxStaticText* m_staticText11;
		wxStaticText* m_staticText21;
		wxStaticText* m_staticText2111;
		wxStaticText* m_staticText21112;
		wxStaticText* m_staticText21113;
		wxChoice* m_preset;
		wxSlider* m_atime;
		wxSlider* m_rtime;
		wxSlider* m_shape;
		wxSlider* m_thrhold;
		wxSlider* m_level;
		wxSlider* m_lpf;
		wxSlider* m_hpf;
		
		// Virtual event handlers, overide them in your derived class
		virtual void onActivate( wxCommandEvent& event ) { event.Skip(); }
		virtual void onChangePreset( wxCommandEvent& event ) { event.Skip(); }
		virtual void onChangeAttack( wxScrollEvent& event ) { event.Skip(); }
		virtual void onChangeRelease( wxScrollEvent& event ) { event.Skip(); }
		virtual void onChangeShape( wxScrollEvent& event ) { event.Skip(); }
		virtual void onChangeThreshold( wxScrollEvent& event ) { event.Skip(); }
		virtual void onChangeLevel( wxScrollEvent& event ) { event.Skip(); }
		virtual void onChangeLpf( wxScrollEvent& event ) { event.Skip(); }
		virtual void onChangeHpf( wxScrollEvent& event ) { event.Skip(); }
		
	
	public:
		
		EffectExpander( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Expander"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( -1,-1 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );
		~EffectExpander();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class General
///////////////////////////////////////////////////////////////////////////////
class General : public wxFrame 
{
	private:
	
	protected:
		wxStaticText* m_staticText29;
		wxSlider* m_input;
		
		// Virtual event handlers, overide them in your derived class
		virtual void onUpdateInputAmpli( wxScrollEvent& event ) { event.Skip(); }
		
	
	public:
		
		General( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( -1,-1 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );
		~General();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class Resource
///////////////////////////////////////////////////////////////////////////////
class Resource : public wxFrame 
{
	private:
	
	protected:
		wxMenuBar* m_menubar2;
		wxMenu* m_popup1;
	
	public:
		
		Resource( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 500,300 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );
		~Resource();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class EffectCtrl
///////////////////////////////////////////////////////////////////////////////
class EffectCtrl : public wxPanel 
{
	private:
	
	protected:
		wxStaticBitmap* m_bitmap1;
		wxCheckBox* m_enableEffect;
		wxStaticText* m_effectName;
		wxToggleButton* m_toggleWindow;
		
		// Virtual event handlers, overide them in your derived class
		virtual void onLeftDown( wxMouseEvent& event ) { event.Skip(); }
		virtual void onLeftUp( wxMouseEvent& event ) { event.Skip(); }
		virtual void onMouseEvents( wxMouseEvent& event ) { event.Skip(); }
		virtual void onActiveEffect( wxCommandEvent& event ) { event.Skip(); }
		virtual void onToggleEffectProperty( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		
		EffectCtrl( wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( -1,-1 ), long style = wxRAISED_BORDER );
		~EffectCtrl();
	
};

#endif //__WxApp__
