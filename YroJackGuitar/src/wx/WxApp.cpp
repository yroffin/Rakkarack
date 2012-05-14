///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Dec 21 2009)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "WxApp.h"

///////////////////////////////////////////////////////////////////////////

MainWindow::MainWindow( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	m_menubar1 = new wxMenuBar( 0 );
	m_menu2 = new wxMenu();
	wxMenuItem* m_menuItem2;
	m_menuItem2 = new wxMenuItem( m_menu2, wxID_ANY, wxString( wxT("Connect") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu2->Append( m_menuItem2 );
	
	m_menubar1->Append( m_menu2, wxT("MyMenu") );
	
	this->SetMenuBar( m_menubar1 );
	
	
	// Connect Events
	this->Connect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( MainWindow::OnClose ) );
	this->Connect( m_menuItem2->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainWindow::OnConnect ) );
}

MainWindow::~MainWindow()
{
	// Disconnect Events
	this->Disconnect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( MainWindow::OnClose ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainWindow::OnConnect ) );
}

SignalFrame::SignalFrame( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* bSizer1;
	bSizer1 = new wxBoxSizer( wxVERTICAL );
	
	m_spinCtrl1 = new wxSpinCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 10, 0 );
	bSizer1->Add( m_spinCtrl1, 0, wxALL, 5 );
	
	this->SetSizer( bSizer1 );
	this->Layout();
	
	// Connect Events
	this->Connect( wxEVT_PAINT, wxPaintEventHandler( SignalFrame::OnPaint ) );
}

SignalFrame::~SignalFrame()
{
	// Disconnect Events
	this->Disconnect( wxEVT_PAINT, wxPaintEventHandler( SignalFrame::OnPaint ) );
}

EffectDistortion::EffectDistortion( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	this->SetFont( wxFont( 8, 75, 90, 90, false, wxEmptyString ) );
	
	wxFlexGridSizer* fgSizer3;
	fgSizer3 = new wxFlexGridSizer( 2, 2, 0, 0 );
	fgSizer3->SetFlexibleDirection( wxBOTH );
	fgSizer3->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	fgSizer3->SetMinSize( wxSize( 100,100 ) ); 
	m_staticText2 = new wxStaticText( this, wxID_ANY, wxT("Preset"), wxDefaultPosition, wxSize( 150,-1 ), 0 );
	m_staticText2->Wrap( -1 );
	fgSizer3->Add( m_staticText2, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	wxString m_presetChoices[] = { wxT("Overdrive 1"), wxT("Overdrive 2"), wxT("Distorsion 1"), wxT("Distorsion 2"), wxT("Distortion 3"), wxT("Guitar Amp") };
	int m_presetNChoices = sizeof( m_presetChoices ) / sizeof( wxString );
	m_preset = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_presetNChoices, m_presetChoices, 0 );
	m_preset->SetSelection( 0 );
	m_preset->SetToolTip( wxT("Select preset type") );
	
	fgSizer3->Add( m_preset, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_staticText1 = new wxStaticText( this, wxID_ANY, wxT("Wet/Dry"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText1->Wrap( -1 );
	fgSizer3->Add( m_staticText1, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_wetdry = new wxSlider( this, wxID_ANY, 0, -64, 63, wxDefaultPosition, wxSize( 200,-1 ), wxSL_AUTOTICKS|wxSL_HORIZONTAL|wxSL_LABELS );
	fgSizer3->Add( m_wetdry, 0, wxALL, 0 );
	
	m_staticText3 = new wxStaticText( this, wxID_ANY, wxT("L/R cross"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText3->Wrap( -1 );
	fgSizer3->Add( m_staticText3, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_lrcross = new wxSlider( this, wxID_ANY, 0, -64, 63, wxDefaultPosition, wxSize( 200,-1 ), wxSL_AUTOTICKS|wxSL_HORIZONTAL|wxSL_LABELS );
	fgSizer3->Add( m_lrcross, 0, wxALL, 5 );
	
	m_staticText11 = new wxStaticText( this, wxID_ANY, wxT("Drive"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText11->Wrap( -1 );
	fgSizer3->Add( m_staticText11, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_drive = new wxSlider( this, wxID_ANY, 0, 0, 127, wxDefaultPosition, wxSize( 200,-1 ), wxSL_AUTOTICKS|wxSL_HORIZONTAL|wxSL_LABELS );
	fgSizer3->Add( m_drive, 0, wxALL, 5 );
	
	m_staticText21 = new wxStaticText( this, wxID_ANY, wxT("Level"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText21->Wrap( -1 );
	fgSizer3->Add( m_staticText21, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_level = new wxSlider( this, wxID_ANY, 0, 0, 127, wxDefaultPosition, wxSize( 200,-1 ), wxSL_AUTOTICKS|wxSL_HORIZONTAL|wxSL_LABELS );
	fgSizer3->Add( m_level, 0, wxALL, 5 );
	
	m_staticText211 = new wxStaticText( this, wxID_ANY, wxT("Type"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText211->Wrap( -1 );
	fgSizer3->Add( m_staticText211, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	wxString m_typeChoices[] = { wxT("Atan"), wxT("Asym1"), wxT("Pow"), wxT("Sine"), wxT("Qnts"), wxT("Zigzg"), wxT("Lmt"), wxT("LmtU"), wxT("LmtL"), wxT("ILmt"), wxT("Clip"), wxT("Asym2"), wxT("Pow2"), wxT("Sgm"), wxT("Crunch"), wxT("Hard Crunch"), wxT("Dirty Octave+"), wxT("M.Square"), wxT("M.Saw"), wxT("Compress"), wxT("Overdrive"), wxT("Soft"), wxT("Super Soft"), wxT("Hard Compress"), wxT("Lmt-NoGain"), wxT("FET"), wxT("DynoFET"), wxT("Valve 1"), wxT("Valve 2"), wxT("Diode clipper") };
	int m_typeNChoices = sizeof( m_typeChoices ) / sizeof( wxString );
	m_type = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_typeNChoices, m_typeChoices, 0 );
	m_type->SetSelection( 0 );
	fgSizer3->Add( m_type, 0, wxALL, 5 );
	
	m_negate = new wxCheckBox( this, wxID_ANY, wxT("Negate"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer3->Add( m_negate, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_prefilter = new wxCheckBox( this, wxID_ANY, wxT("Pre-filtering"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer3->Add( m_prefilter, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_stereo = new wxCheckBox( this, wxID_ANY, wxT("Stereo"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer3->Add( m_stereo, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	
	fgSizer3->Add( 0, 0, 1, wxEXPAND, 5 );
	
	m_staticText2111 = new wxStaticText( this, wxID_ANY, wxT("Panning"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText2111->Wrap( -1 );
	fgSizer3->Add( m_staticText2111, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_panning = new wxSlider( this, wxID_ANY, 0, -64, 63, wxDefaultPosition, wxSize( 200,-1 ), wxSL_AUTOTICKS|wxSL_HORIZONTAL|wxSL_LABELS );
	fgSizer3->Add( m_panning, 0, wxALL, 5 );
	
	m_staticText21111 = new wxStaticText( this, wxID_ANY, wxT("Sub octave"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText21111->Wrap( -1 );
	fgSizer3->Add( m_staticText21111, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_suboctave = new wxSlider( this, wxID_ANY, 0, 0, 127, wxDefaultPosition, wxSize( 200,-1 ), wxSL_AUTOTICKS|wxSL_HORIZONTAL|wxSL_LABELS );
	fgSizer3->Add( m_suboctave, 0, wxALL, 5 );
	
	m_staticText21112 = new wxStaticText( this, wxID_ANY, wxT("LPF"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText21112->Wrap( -1 );
	fgSizer3->Add( m_staticText21112, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_lpf = new wxSlider( this, wxID_ANY, 20, 20, 26000, wxDefaultPosition, wxSize( 200,-1 ), wxSL_AUTOTICKS|wxSL_HORIZONTAL|wxSL_LABELS );
	fgSizer3->Add( m_lpf, 0, wxALL, 5 );
	
	m_staticText21113 = new wxStaticText( this, wxID_ANY, wxT("HPF"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText21113->Wrap( -1 );
	fgSizer3->Add( m_staticText21113, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_hpf = new wxSlider( this, wxID_ANY, 20, 20, 20000, wxDefaultPosition, wxSize( 200,-1 ), wxSL_AUTOTICKS|wxSL_HORIZONTAL|wxSL_LABELS );
	fgSizer3->Add( m_hpf, 0, wxALL, 5 );
	
	this->SetSizer( fgSizer3 );
	this->Layout();
	
	this->Centre( wxBOTH );
	
	// Connect Events
	m_preset->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( EffectDistortion::onChangePreset ), NULL, this );
	m_wetdry->Connect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( EffectDistortion::onChangeWetDry ), NULL, this );
	m_lrcross->Connect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( EffectDistortion::onChangeLRCross ), NULL, this );
	m_drive->Connect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( EffectDistortion::onChangeDrive ), NULL, this );
	m_level->Connect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( EffectDistortion::onChangeLevel ), NULL, this );
	m_type->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( EffectDistortion::onChangeType ), NULL, this );
	m_negate->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( EffectDistortion::onChangeNegate ), NULL, this );
	m_prefilter->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( EffectDistortion::onChangePrefilter ), NULL, this );
	m_stereo->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( EffectDistortion::onChangeStereo ), NULL, this );
	m_panning->Connect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( EffectDistortion::onChangePanning ), NULL, this );
	m_suboctave->Connect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( EffectDistortion::onChangeSubOctave ), NULL, this );
	m_lpf->Connect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( EffectDistortion::onChangeLpf ), NULL, this );
	m_hpf->Connect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( EffectDistortion::onChangeHpf ), NULL, this );
}

EffectDistortion::~EffectDistortion()
{
	// Disconnect Events
	m_preset->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( EffectDistortion::onChangePreset ), NULL, this );
	m_wetdry->Disconnect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( EffectDistortion::onChangeWetDry ), NULL, this );
	m_lrcross->Disconnect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( EffectDistortion::onChangeLRCross ), NULL, this );
	m_drive->Disconnect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( EffectDistortion::onChangeDrive ), NULL, this );
	m_level->Disconnect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( EffectDistortion::onChangeLevel ), NULL, this );
	m_type->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( EffectDistortion::onChangeType ), NULL, this );
	m_negate->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( EffectDistortion::onChangeNegate ), NULL, this );
	m_prefilter->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( EffectDistortion::onChangePrefilter ), NULL, this );
	m_stereo->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( EffectDistortion::onChangeStereo ), NULL, this );
	m_panning->Disconnect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( EffectDistortion::onChangePanning ), NULL, this );
	m_suboctave->Disconnect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( EffectDistortion::onChangeSubOctave ), NULL, this );
	m_lpf->Disconnect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( EffectDistortion::onChangeLpf ), NULL, this );
	m_hpf->Disconnect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( EffectDistortion::onChangeHpf ), NULL, this );
}
