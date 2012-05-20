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
	this->SetFont( wxFont( 8, 77, 90, 90, false, wxT("Ubuntu Condensed") ) );
	
	m_menubar1 = new wxMenuBar( 0 );
	m_menu2 = new wxMenu();
	wxMenuItem* m_menuItem2;
	m_menuItem2 = new wxMenuItem( m_menu2, wxID_ANY, wxString( wxT("Connect") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu2->Append( m_menuItem2 );
	
	m_menubar1->Append( m_menu2, wxT("MyMenu") );
	
	this->SetMenuBar( m_menubar1 );
	
	wxGridSizer* gSizer1;
	gSizer1 = new wxGridSizer( 1, 1, 0, 0 );
	
	m_notebook = new wxNotebook( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	m_panelEffects = new wxScrolledWindow( m_notebook, wxID_ANY, wxDefaultPosition, wxSize( 800,600 ), wxHSCROLL|wxVSCROLL );
	m_panelEffects->SetScrollRate( 5, 5 );
	m_notebook->AddPage( m_panelEffects, wxT("Effects"), false );
	m_panel6 = new wxPanel( m_notebook, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_notebook->AddPage( m_panel6, wxT("a page"), false );
	
	gSizer1->Add( m_notebook, 1, wxEXPAND | wxALL, 5 );
	
	this->SetSizer( gSizer1 );
	this->Layout();
	gSizer1->Fit( this );
	
	this->Centre( wxBOTH );
	
	// Connect Events
	this->Connect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( MainWindow::OnClose ) );
	this->Connect( m_menuItem2->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainWindow::OnConnect ) );
	m_panelEffects->Connect( wxEVT_PAINT, wxPaintEventHandler( MainWindow::onPaintEffects ), NULL, this );
}

MainWindow::~MainWindow()
{
	// Disconnect Events
	this->Disconnect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( MainWindow::OnClose ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainWindow::OnConnect ) );
	m_panelEffects->Disconnect( wxEVT_PAINT, wxPaintEventHandler( MainWindow::onPaintEffects ), NULL, this );
}

SignalFrame::SignalFrame( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* bSizer1;
	bSizer1 = new wxBoxSizer( wxVERTICAL );
	
	m_toggle = new wxToggleButton( this, wxID_ANY, wxT("Off"), wxDefaultPosition, wxDefaultSize, 0 );
	m_toggle->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_ACTIVECAPTION ) );
	
	bSizer1->Add( m_toggle, 0, wxALL, 5 );
	
	this->SetSizer( bSizer1 );
	this->Layout();
	
	// Connect Events
	this->Connect( wxEVT_PAINT, wxPaintEventHandler( SignalFrame::OnPaint ) );
	m_toggle->Connect( wxEVT_COMMAND_TOGGLEBUTTON_CLICKED, wxCommandEventHandler( SignalFrame::onActivate ), NULL, this );
}

SignalFrame::~SignalFrame()
{
	// Disconnect Events
	this->Disconnect( wxEVT_PAINT, wxPaintEventHandler( SignalFrame::OnPaint ) );
	m_toggle->Disconnect( wxEVT_COMMAND_TOGGLEBUTTON_CLICKED, wxCommandEventHandler( SignalFrame::onActivate ), NULL, this );
}

EffectDistortion::EffectDistortion( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxFlexGridSizer* fgSizer3;
	fgSizer3 = new wxFlexGridSizer( 2, 10, 0, 0 );
	fgSizer3->SetFlexibleDirection( wxBOTH );
	fgSizer3->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	fgSizer3->SetMinSize( wxSize( 100,100 ) ); 
	m_toggle = new wxToggleButton( this, wxID_ANY, wxT("Off"), wxDefaultPosition, wxDefaultSize, 0 );
	m_toggle->SetValue( true ); 
	m_toggle->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_ACTIVECAPTION ) );
	
	fgSizer3->Add( m_toggle, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_staticText1 = new wxStaticText( this, wxID_ANY, wxT("Wet/Dry"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText1->Wrap( -1 );
	fgSizer3->Add( m_staticText1, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	m_staticText3 = new wxStaticText( this, wxID_ANY, wxT("L/R cross"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText3->Wrap( -1 );
	fgSizer3->Add( m_staticText3, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	m_staticText11 = new wxStaticText( this, wxID_ANY, wxT("Drive"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText11->Wrap( -1 );
	fgSizer3->Add( m_staticText11, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	m_staticText21 = new wxStaticText( this, wxID_ANY, wxT("Level"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText21->Wrap( -1 );
	fgSizer3->Add( m_staticText21, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_staticText211 = new wxStaticText( this, wxID_ANY, wxT("Type"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText211->Wrap( -1 );
	fgSizer3->Add( m_staticText211, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	m_staticText2111 = new wxStaticText( this, wxID_ANY, wxT("Panning"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText2111->Wrap( -1 );
	fgSizer3->Add( m_staticText2111, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	m_staticText21111 = new wxStaticText( this, wxID_ANY, wxT("Sub octave"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText21111->Wrap( -1 );
	fgSizer3->Add( m_staticText21111, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_staticText21112 = new wxStaticText( this, wxID_ANY, wxT("LPF"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText21112->Wrap( -1 );
	fgSizer3->Add( m_staticText21112, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_staticText21113 = new wxStaticText( this, wxID_ANY, wxT("HPF"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText21113->Wrap( -1 );
	fgSizer3->Add( m_staticText21113, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	wxString m_presetChoices[] = { wxT("Overdrive 1"), wxT("Overdrive 2"), wxT("Distorsion 1"), wxT("Distorsion 2"), wxT("Distortion 3"), wxT("Guitar Amp") };
	int m_presetNChoices = sizeof( m_presetChoices ) / sizeof( wxString );
	m_preset = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_presetNChoices, m_presetChoices, 0 );
	m_preset->SetSelection( 0 );
	m_preset->SetToolTip( wxT("Select preset type") );
	
	fgSizer3->Add( m_preset, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	m_wetdry = new wxSlider( this, wxID_ANY, 0, -64, 63, wxDefaultPosition, wxSize( 40,100 ), wxSL_AUTOTICKS|wxSL_INVERSE|wxSL_LABELS|wxSL_VERTICAL );
	m_wetdry->SetToolTip( wxT("Wet/Dry") );
	
	fgSizer3->Add( m_wetdry, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0 );
	
	m_lrcross = new wxSlider( this, wxID_ANY, 0, -64, 63, wxDefaultPosition, wxSize( 40,100 ), wxSL_AUTOTICKS|wxSL_INVERSE|wxSL_LABELS|wxSL_VERTICAL );
	fgSizer3->Add( m_lrcross, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_drive = new wxSlider( this, wxID_ANY, 0, 0, 127, wxDefaultPosition, wxSize( 40,100 ), wxSL_AUTOTICKS|wxSL_INVERSE|wxSL_LABELS|wxSL_VERTICAL );
	fgSizer3->Add( m_drive, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_level = new wxSlider( this, wxID_ANY, 0, 0, 127, wxDefaultPosition, wxSize( 40,100 ), wxSL_AUTOTICKS|wxSL_INVERSE|wxSL_LABELS|wxSL_VERTICAL );
	fgSizer3->Add( m_level, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );
	
	wxString m_typeChoices[] = { wxT("Atan"), wxT("Asym1"), wxT("Pow"), wxT("Sine"), wxT("Qnts"), wxT("Zigzg"), wxT("Lmt"), wxT("LmtU"), wxT("LmtL"), wxT("ILmt"), wxT("Clip"), wxT("Asym2"), wxT("Pow2"), wxT("Sgm"), wxT("Crunch"), wxT("Hard Crunch"), wxT("Dirty Octave+"), wxT("M.Square"), wxT("M.Saw"), wxT("Compress"), wxT("Overdrive"), wxT("Soft"), wxT("Super Soft"), wxT("Hard Compress"), wxT("Lmt-NoGain"), wxT("FET"), wxT("DynoFET"), wxT("Valve 1"), wxT("Valve 2"), wxT("Diode clipper") };
	int m_typeNChoices = sizeof( m_typeChoices ) / sizeof( wxString );
	m_type = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_typeNChoices, m_typeChoices, 0 );
	m_type->SetSelection( 5 );
	fgSizer3->Add( m_type, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	m_panning = new wxSlider( this, wxID_ANY, 0, -64, 63, wxDefaultPosition, wxSize( 40,100 ), wxSL_AUTOTICKS|wxSL_INVERSE|wxSL_LABELS|wxSL_VERTICAL );
	fgSizer3->Add( m_panning, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	m_suboctave = new wxSlider( this, wxID_ANY, 0, 0, 127, wxDefaultPosition, wxSize( 40,100 ), wxSL_AUTOTICKS|wxSL_INVERSE|wxSL_LABELS|wxSL_VERTICAL );
	fgSizer3->Add( m_suboctave, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	m_lpf = new wxSlider( this, wxID_ANY, 20, 20, 26000, wxDefaultPosition, wxSize( 40,100 ), wxSL_AUTOTICKS|wxSL_INVERSE|wxSL_LABELS|wxSL_VERTICAL );
	fgSizer3->Add( m_lpf, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	m_hpf = new wxSlider( this, wxID_ANY, 20, 20, 20000, wxDefaultPosition, wxSize( 40,100 ), wxSL_AUTOTICKS|wxSL_INVERSE|wxSL_LABELS|wxSL_VERTICAL );
	fgSizer3->Add( m_hpf, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	
	fgSizer3->Add( 0, 0, 1, wxEXPAND, 5 );
	
	m_negate = new wxCheckBox( this, wxID_ANY, wxT("Negate"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer3->Add( m_negate, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	m_prefilter = new wxCheckBox( this, wxID_ANY, wxT("Pre-filtering"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer3->Add( m_prefilter, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	m_stereo = new wxCheckBox( this, wxID_ANY, wxT("Stereo"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer3->Add( m_stereo, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	this->SetSizer( fgSizer3 );
	this->Layout();
	fgSizer3->Fit( this );
	
	this->Centre( wxBOTH );
	
	// Connect Events
	m_toggle->Connect( wxEVT_COMMAND_TOGGLEBUTTON_CLICKED, wxCommandEventHandler( EffectDistortion::onActivate ), NULL, this );
	m_preset->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( EffectDistortion::onChangePreset ), NULL, this );
	m_wetdry->Connect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( EffectDistortion::onChangeWetDry ), NULL, this );
	m_lrcross->Connect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( EffectDistortion::onChangeLRCross ), NULL, this );
	m_drive->Connect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( EffectDistortion::onChangeDrive ), NULL, this );
	m_level->Connect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( EffectDistortion::onChangeLevel ), NULL, this );
	m_type->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( EffectDistortion::onChangeType ), NULL, this );
	m_panning->Connect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( EffectDistortion::onChangePanning ), NULL, this );
	m_suboctave->Connect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( EffectDistortion::onChangeSubOctave ), NULL, this );
	m_lpf->Connect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( EffectDistortion::onChangeLpf ), NULL, this );
	m_hpf->Connect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( EffectDistortion::onChangeHpf ), NULL, this );
	m_negate->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( EffectDistortion::onChangeNegate ), NULL, this );
	m_prefilter->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( EffectDistortion::onChangePrefilter ), NULL, this );
	m_stereo->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( EffectDistortion::onChangeStereo ), NULL, this );
}

EffectDistortion::~EffectDistortion()
{
	// Disconnect Events
	m_toggle->Disconnect( wxEVT_COMMAND_TOGGLEBUTTON_CLICKED, wxCommandEventHandler( EffectDistortion::onActivate ), NULL, this );
	m_preset->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( EffectDistortion::onChangePreset ), NULL, this );
	m_wetdry->Disconnect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( EffectDistortion::onChangeWetDry ), NULL, this );
	m_lrcross->Disconnect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( EffectDistortion::onChangeLRCross ), NULL, this );
	m_drive->Disconnect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( EffectDistortion::onChangeDrive ), NULL, this );
	m_level->Disconnect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( EffectDistortion::onChangeLevel ), NULL, this );
	m_type->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( EffectDistortion::onChangeType ), NULL, this );
	m_panning->Disconnect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( EffectDistortion::onChangePanning ), NULL, this );
	m_suboctave->Disconnect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( EffectDistortion::onChangeSubOctave ), NULL, this );
	m_lpf->Disconnect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( EffectDistortion::onChangeLpf ), NULL, this );
	m_hpf->Disconnect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( EffectDistortion::onChangeHpf ), NULL, this );
	m_negate->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( EffectDistortion::onChangeNegate ), NULL, this );
	m_prefilter->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( EffectDistortion::onChangePrefilter ), NULL, this );
	m_stereo->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( EffectDistortion::onChangeStereo ), NULL, this );
}

EffectChorus::EffectChorus( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxFlexGridSizer* fgSizer3;
	fgSizer3 = new wxFlexGridSizer( 2, 3, 0, 0 );
	fgSizer3->SetFlexibleDirection( wxBOTH );
	fgSizer3->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	fgSizer3->SetMinSize( wxSize( 100,100 ) ); 
	m_toggle = new wxToggleButton( this, wxID_ANY, wxT("Off"), wxDefaultPosition, wxDefaultSize, 0 );
	m_toggle->SetValue( true ); 
	m_toggle->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_ACTIVECAPTION ) );
	
	fgSizer3->Add( m_toggle, 0, wxALL, 5 );
	
	m_staticText2 = new wxStaticText( this, wxID_ANY, wxT("Preset"), wxDefaultPosition, wxSize( 150,-1 ), 0 );
	m_staticText2->Wrap( -1 );
	fgSizer3->Add( m_staticText2, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	wxArrayString m_presetChoices;
	m_preset = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_presetChoices, 0 );
	m_preset->SetSelection( 0 );
	m_preset->SetToolTip( wxT("Select preset type") );
	
	fgSizer3->Add( m_preset, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_staticText1 = new wxStaticText( this, wxID_ANY, wxT("Wet/Dry"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText1->Wrap( -1 );
	fgSizer3->Add( m_staticText1, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_wetdry = new wxSlider( this, wxID_ANY, 0, -64, 63, wxDefaultPosition, wxSize( 200,-1 ), wxSL_AUTOTICKS|wxSL_HORIZONTAL|wxSL_LABELS );
	fgSizer3->Add( m_wetdry, 0, wxALL, 0 );
	
	
	fgSizer3->Add( 0, 0, 1, wxEXPAND, 5 );
	
	m_staticText3 = new wxStaticText( this, wxID_ANY, wxT("Pan"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText3->Wrap( -1 );
	fgSizer3->Add( m_staticText3, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_lrcross = new wxSlider( this, wxID_ANY, 0, -64, 63, wxDefaultPosition, wxSize( 200,-1 ), wxSL_AUTOTICKS|wxSL_HORIZONTAL|wxSL_LABELS );
	fgSizer3->Add( m_lrcross, 0, wxALL, 5 );
	
	
	fgSizer3->Add( 0, 0, 1, wxEXPAND, 5 );
	
	m_staticText11 = new wxStaticText( this, wxID_ANY, wxT("Tempo"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText11->Wrap( -1 );
	fgSizer3->Add( m_staticText11, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_drive = new wxSlider( this, wxID_ANY, 0, 0, 127, wxDefaultPosition, wxSize( 200,-1 ), wxSL_AUTOTICKS|wxSL_HORIZONTAL|wxSL_LABELS );
	fgSizer3->Add( m_drive, 0, wxALL, 5 );
	
	
	fgSizer3->Add( 0, 0, 1, wxEXPAND, 5 );
	
	m_staticText21 = new wxStaticText( this, wxID_ANY, wxT("Rnd"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText21->Wrap( -1 );
	fgSizer3->Add( m_staticText21, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_level = new wxSlider( this, wxID_ANY, 0, 0, 127, wxDefaultPosition, wxSize( 200,-1 ), wxSL_AUTOTICKS|wxSL_HORIZONTAL|wxSL_LABELS );
	fgSizer3->Add( m_level, 0, wxALL, 5 );
	
	
	fgSizer3->Add( 0, 0, 1, wxEXPAND, 5 );
	
	m_staticText211 = new wxStaticText( this, wxID_ANY, wxT("Type"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText211->Wrap( -1 );
	fgSizer3->Add( m_staticText211, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	wxString m_typeChoices[] = { wxT("Atan"), wxT("Asym1"), wxT("Pow"), wxT("Sine"), wxT("Qnts"), wxT("Zigzg"), wxT("Lmt"), wxT("LmtU"), wxT("LmtL"), wxT("ILmt"), wxT("Clip"), wxT("Asym2"), wxT("Pow2"), wxT("Sgm"), wxT("Crunch"), wxT("Hard Crunch"), wxT("Dirty Octave+"), wxT("M.Square"), wxT("M.Saw"), wxT("Compress"), wxT("Overdrive"), wxT("Soft"), wxT("Super Soft"), wxT("Hard Compress"), wxT("Lmt-NoGain"), wxT("FET"), wxT("DynoFET"), wxT("Valve 1"), wxT("Valve 2"), wxT("Diode clipper") };
	int m_typeNChoices = sizeof( m_typeChoices ) / sizeof( wxString );
	m_type = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_typeNChoices, m_typeChoices, 0 );
	m_type->SetSelection( 3 );
	fgSizer3->Add( m_type, 0, wxALL, 5 );
	
	
	fgSizer3->Add( 0, 0, 1, wxEXPAND, 5 );
	
	m_negate = new wxCheckBox( this, wxID_ANY, wxT("Substract"), wxDefaultPosition, wxDefaultSize, 0 );
	m_negate->SetValue(true); 
	fgSizer3->Add( m_negate, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	
	fgSizer3->Add( 0, 0, 1, wxEXPAND, 5 );
	
	
	fgSizer3->Add( 0, 0, 1, wxEXPAND, 5 );
	
	m_staticText2111 = new wxStaticText( this, wxID_ANY, wxT("St.Df."), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText2111->Wrap( -1 );
	fgSizer3->Add( m_staticText2111, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_panning = new wxSlider( this, wxID_ANY, 0, -64, 63, wxDefaultPosition, wxSize( 200,-1 ), wxSL_AUTOTICKS|wxSL_HORIZONTAL|wxSL_LABELS );
	fgSizer3->Add( m_panning, 0, wxALL, 5 );
	
	
	fgSizer3->Add( 0, 0, 1, wxEXPAND, 5 );
	
	m_staticText21111 = new wxStaticText( this, wxID_ANY, wxT("Depth"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText21111->Wrap( -1 );
	fgSizer3->Add( m_staticText21111, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_suboctave = new wxSlider( this, wxID_ANY, 0, 0, 127, wxDefaultPosition, wxSize( 200,-1 ), wxSL_AUTOTICKS|wxSL_HORIZONTAL|wxSL_LABELS );
	fgSizer3->Add( m_suboctave, 0, wxALL, 5 );
	
	
	fgSizer3->Add( 0, 0, 1, wxEXPAND, 5 );
	
	m_staticText21112 = new wxStaticText( this, wxID_ANY, wxT("Delay"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText21112->Wrap( -1 );
	fgSizer3->Add( m_staticText21112, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_lpf = new wxSlider( this, wxID_ANY, 20, 20, 26000, wxDefaultPosition, wxSize( 200,-1 ), wxSL_AUTOTICKS|wxSL_HORIZONTAL|wxSL_LABELS );
	fgSizer3->Add( m_lpf, 0, wxALL, 5 );
	
	
	fgSizer3->Add( 0, 0, 1, wxEXPAND, 5 );
	
	m_staticText21113 = new wxStaticText( this, wxID_ANY, wxT("L/R cross"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText21113->Wrap( -1 );
	fgSizer3->Add( m_staticText21113, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_hpf = new wxSlider( this, wxID_ANY, 20, 20, 20000, wxDefaultPosition, wxSize( 200,-1 ), wxSL_AUTOTICKS|wxSL_HORIZONTAL|wxSL_LABELS );
	fgSizer3->Add( m_hpf, 0, wxALL, 5 );
	
	this->SetSizer( fgSizer3 );
	this->Layout();
	
	this->Centre( wxBOTH );
	
	// Connect Events
	m_toggle->Connect( wxEVT_COMMAND_TOGGLEBUTTON_CLICKED, wxCommandEventHandler( EffectChorus::onActivate ), NULL, this );
	m_preset->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( EffectChorus::onChangePreset ), NULL, this );
	m_wetdry->Connect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( EffectChorus::onChangeWetDry ), NULL, this );
	m_lrcross->Connect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( EffectChorus::onChangeLRCross ), NULL, this );
	m_drive->Connect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( EffectChorus::onChangeDrive ), NULL, this );
	m_level->Connect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( EffectChorus::onChangeLevel ), NULL, this );
	m_type->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( EffectChorus::onChangeType ), NULL, this );
	m_negate->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( EffectChorus::onChangeNegate ), NULL, this );
	m_panning->Connect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( EffectChorus::onChangePanning ), NULL, this );
	m_suboctave->Connect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( EffectChorus::onChangeSubOctave ), NULL, this );
	m_lpf->Connect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( EffectChorus::onChangeLpf ), NULL, this );
	m_hpf->Connect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( EffectChorus::onChangeHpf ), NULL, this );
}

EffectChorus::~EffectChorus()
{
	// Disconnect Events
	m_toggle->Disconnect( wxEVT_COMMAND_TOGGLEBUTTON_CLICKED, wxCommandEventHandler( EffectChorus::onActivate ), NULL, this );
	m_preset->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( EffectChorus::onChangePreset ), NULL, this );
	m_wetdry->Disconnect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( EffectChorus::onChangeWetDry ), NULL, this );
	m_lrcross->Disconnect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( EffectChorus::onChangeLRCross ), NULL, this );
	m_drive->Disconnect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( EffectChorus::onChangeDrive ), NULL, this );
	m_level->Disconnect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( EffectChorus::onChangeLevel ), NULL, this );
	m_type->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( EffectChorus::onChangeType ), NULL, this );
	m_negate->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( EffectChorus::onChangeNegate ), NULL, this );
	m_panning->Disconnect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( EffectChorus::onChangePanning ), NULL, this );
	m_suboctave->Disconnect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( EffectChorus::onChangeSubOctave ), NULL, this );
	m_lpf->Disconnect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( EffectChorus::onChangeLpf ), NULL, this );
	m_hpf->Disconnect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( EffectChorus::onChangeHpf ), NULL, this );
}

EffectExpander::EffectExpander( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxFlexGridSizer* fgSizer5;
	fgSizer5 = new wxFlexGridSizer( 1, 8, 0, 0 );
	fgSizer5->SetFlexibleDirection( wxBOTH );
	fgSizer5->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_toggle = new wxToggleButton( this, wxID_ANY, wxT("Off"), wxDefaultPosition, wxDefaultSize, 0 );
	m_toggle->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_ACTIVECAPTION ) );
	
	fgSizer5->Add( m_toggle, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_staticText1 = new wxStaticText( this, wxID_ANY, wxT("A. Tim."), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText1->Wrap( -1 );
	fgSizer5->Add( m_staticText1, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	m_staticText3 = new wxStaticText( this, wxID_ANY, wxT("R. Tim."), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText3->Wrap( -1 );
	fgSizer5->Add( m_staticText3, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	m_staticText11 = new wxStaticText( this, wxID_ANY, wxT("Shp."), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText11->Wrap( -1 );
	fgSizer5->Add( m_staticText11, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	m_staticText21 = new wxStaticText( this, wxID_ANY, wxT("Thr."), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText21->Wrap( -1 );
	fgSizer5->Add( m_staticText21, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	m_staticText2111 = new wxStaticText( this, wxID_ANY, wxT("Level"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText2111->Wrap( -1 );
	fgSizer5->Add( m_staticText2111, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	m_staticText21112 = new wxStaticText( this, wxID_ANY, wxT("LPF"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText21112->Wrap( -1 );
	fgSizer5->Add( m_staticText21112, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	m_staticText21113 = new wxStaticText( this, wxID_ANY, wxT("HPF"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText21113->Wrap( -1 );
	fgSizer5->Add( m_staticText21113, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	wxString m_presetChoices[] = { wxT("Noise gate"), wxT("Boost gate"), wxT("Treble swel") };
	int m_presetNChoices = sizeof( m_presetChoices ) / sizeof( wxString );
	m_preset = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_presetNChoices, m_presetChoices, 0 );
	m_preset->SetSelection( 0 );
	m_preset->SetToolTip( wxT("Select preset type") );
	
	fgSizer5->Add( m_preset, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_atime = new wxSlider( this, wxID_ANY, 0, 10, 500, wxDefaultPosition, wxSize( 40,100 ), wxSL_AUTOTICKS|wxSL_INVERSE|wxSL_LABELS|wxSL_VERTICAL );
	m_atime->SetToolTip( wxT("Attack time") );
	
	fgSizer5->Add( m_atime, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_CENTER_HORIZONTAL, 0 );
	
	m_rtime = new wxSlider( this, wxID_ANY, 0, 10, 500, wxDefaultPosition, wxSize( 40,100 ), wxSL_AUTOTICKS|wxSL_INVERSE|wxSL_LABELS|wxSL_VERTICAL );
	m_rtime->SetToolTip( wxT("Release time") );
	
	fgSizer5->Add( m_rtime, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_shape = new wxSlider( this, wxID_ANY, 0, 1, 50, wxDefaultPosition, wxSize( 40,100 ), wxSL_AUTOTICKS|wxSL_INVERSE|wxSL_LABELS|wxSL_VERTICAL );
	m_shape->SetToolTip( wxT("Shape") );
	
	fgSizer5->Add( m_shape, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_thrhold = new wxSlider( this, wxID_ANY, 0, -80, 0, wxDefaultPosition, wxSize( 40,100 ), wxSL_AUTOTICKS|wxSL_INVERSE|wxSL_LABELS|wxSL_VERTICAL );
	m_thrhold->SetToolTip( wxT("Threshold") );
	
	fgSizer5->Add( m_thrhold, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	m_level = new wxSlider( this, wxID_ANY, 0, 1, 127, wxDefaultPosition, wxSize( 40,100 ), wxSL_AUTOTICKS|wxSL_INVERSE|wxSL_LABELS|wxSL_VERTICAL );
	fgSizer5->Add( m_level, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	m_lpf = new wxSlider( this, wxID_ANY, 20, 20, 26000, wxDefaultPosition, wxSize( 40,100 ), wxSL_AUTOTICKS|wxSL_INVERSE|wxSL_LABELS|wxSL_VERTICAL );
	fgSizer5->Add( m_lpf, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	m_hpf = new wxSlider( this, wxID_ANY, 20, 20, 20000, wxDefaultPosition, wxSize( 40,100 ), wxSL_AUTOTICKS|wxSL_INVERSE|wxSL_LABELS|wxSL_VERTICAL );
	fgSizer5->Add( m_hpf, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	this->SetSizer( fgSizer5 );
	this->Layout();
	fgSizer5->Fit( this );
	
	this->Centre( wxBOTH );
	
	// Connect Events
	m_toggle->Connect( wxEVT_COMMAND_TOGGLEBUTTON_CLICKED, wxCommandEventHandler( EffectExpander::onActivate ), NULL, this );
	m_preset->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( EffectExpander::onChangePreset ), NULL, this );
	m_atime->Connect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( EffectExpander::onChangeAttack ), NULL, this );
	m_rtime->Connect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( EffectExpander::onChangeRelease ), NULL, this );
	m_shape->Connect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( EffectExpander::onChangeShape ), NULL, this );
	m_thrhold->Connect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( EffectExpander::onChangeThreshold ), NULL, this );
	m_level->Connect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( EffectExpander::onChangeLevel ), NULL, this );
	m_lpf->Connect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( EffectExpander::onChangeLpf ), NULL, this );
	m_hpf->Connect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( EffectExpander::onChangeHpf ), NULL, this );
}

EffectExpander::~EffectExpander()
{
	// Disconnect Events
	m_toggle->Disconnect( wxEVT_COMMAND_TOGGLEBUTTON_CLICKED, wxCommandEventHandler( EffectExpander::onActivate ), NULL, this );
	m_preset->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( EffectExpander::onChangePreset ), NULL, this );
	m_atime->Disconnect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( EffectExpander::onChangeAttack ), NULL, this );
	m_rtime->Disconnect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( EffectExpander::onChangeRelease ), NULL, this );
	m_shape->Disconnect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( EffectExpander::onChangeShape ), NULL, this );
	m_thrhold->Disconnect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( EffectExpander::onChangeThreshold ), NULL, this );
	m_level->Disconnect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( EffectExpander::onChangeLevel ), NULL, this );
	m_lpf->Disconnect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( EffectExpander::onChangeLpf ), NULL, this );
	m_hpf->Disconnect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( EffectExpander::onChangeHpf ), NULL, this );
}

General::General( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxFlexGridSizer* fgSizer5;
	fgSizer5 = new wxFlexGridSizer( 2, 1, 0, 0 );
	fgSizer5->SetFlexibleDirection( wxBOTH );
	fgSizer5->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText29 = new wxStaticText( this, wxID_ANY, wxT("In."), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText29->Wrap( -1 );
	fgSizer5->Add( m_staticText29, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	m_input = new wxSlider( this, wxID_ANY, 100, 0, 2000, wxDefaultPosition, wxSize( 40,100 ), wxSL_AUTOTICKS|wxSL_INVERSE|wxSL_LABELS|wxSL_VERTICAL );
	m_input->SetToolTip( wxT("Input amplification (%)") );
	
	fgSizer5->Add( m_input, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	this->SetSizer( fgSizer5 );
	this->Layout();
	fgSizer5->Fit( this );
	
	// Connect Events
	m_input->Connect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( General::onUpdateInputAmpli ), NULL, this );
}

General::~General()
{
	// Disconnect Events
	m_input->Disconnect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( General::onUpdateInputAmpli ), NULL, this );
}

Resource::Resource( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	m_menubar2 = new wxMenuBar( 0 );
	m_popup1 = new wxMenu();
	wxMenuItem* m_menuItem2;
	m_menuItem2 = new wxMenuItem( m_popup1, wxID_ANY, wxString( wxT("MyMenuItem") ) , wxEmptyString, wxITEM_NORMAL );
	m_popup1->Append( m_menuItem2 );
	
	wxMenuItem* m_menuItem3;
	m_menuItem3 = new wxMenuItem( m_popup1, wxID_ANY, wxString( wxT("MyMenuItem") ) , wxEmptyString, wxITEM_NORMAL );
	m_popup1->Append( m_menuItem3 );
	
	wxMenuItem* m_menuItem4;
	m_menuItem4 = new wxMenuItem( m_popup1, wxID_ANY, wxString( wxT("MyMenuItem") ) , wxEmptyString, wxITEM_NORMAL );
	m_popup1->Append( m_menuItem4 );
	
	wxMenuItem* m_menuItem5;
	m_menuItem5 = new wxMenuItem( m_popup1, wxID_ANY, wxString( wxT("MyMenuItem") ) , wxEmptyString, wxITEM_NORMAL );
	m_popup1->Append( m_menuItem5 );
	
	m_menubar2->Append( m_popup1, wxT("MyMenu") );
	
	this->SetMenuBar( m_menubar2 );
	
}

Resource::~Resource()
{
}

EffectCtrl::EffectCtrl( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style ) : wxPanel( parent, id, pos, size, style )
{
	wxFlexGridSizer* fgSizer6;
	fgSizer6 = new wxFlexGridSizer( 2, 2, 0, 0 );
	fgSizer6->SetFlexibleDirection( wxBOTH );
	fgSizer6->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_bitmap1 = new wxStaticBitmap( this, wxID_ANY, wxBitmap( wxT("resources/Processus.png"), wxBITMAP_TYPE_ANY ), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer6->Add( m_bitmap1, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_enableEffect = new wxCheckBox( this, wxID_ANY, wxT("Active"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer6->Add( m_enableEffect, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_effectName = new wxStaticText( this, wxID_ANY, wxT("MyLabel"), wxDefaultPosition, wxDefaultSize, 0 );
	m_effectName->Wrap( -1 );
	fgSizer6->Add( m_effectName, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_toggleWindow = new wxToggleButton( this, wxID_ANY, wxT("Properties"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer6->Add( m_toggleWindow, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	this->SetSizer( fgSizer6 );
	this->Layout();
	fgSizer6->Fit( this );
	
	// Connect Events
	this->Connect( wxEVT_LEFT_DOWN, wxMouseEventHandler( EffectCtrl::onLeftDown ) );
	this->Connect( wxEVT_LEFT_UP, wxMouseEventHandler( EffectCtrl::onLeftUp ) );
	this->Connect( wxEVT_LEFT_DOWN, wxMouseEventHandler( EffectCtrl::onMouseEvents ) );
	this->Connect( wxEVT_LEFT_UP, wxMouseEventHandler( EffectCtrl::onMouseEvents ) );
	this->Connect( wxEVT_MIDDLE_DOWN, wxMouseEventHandler( EffectCtrl::onMouseEvents ) );
	this->Connect( wxEVT_MIDDLE_UP, wxMouseEventHandler( EffectCtrl::onMouseEvents ) );
	this->Connect( wxEVT_RIGHT_DOWN, wxMouseEventHandler( EffectCtrl::onMouseEvents ) );
	this->Connect( wxEVT_RIGHT_UP, wxMouseEventHandler( EffectCtrl::onMouseEvents ) );
	this->Connect( wxEVT_MOTION, wxMouseEventHandler( EffectCtrl::onMouseEvents ) );
	this->Connect( wxEVT_LEFT_DCLICK, wxMouseEventHandler( EffectCtrl::onMouseEvents ) );
	this->Connect( wxEVT_MIDDLE_DCLICK, wxMouseEventHandler( EffectCtrl::onMouseEvents ) );
	this->Connect( wxEVT_RIGHT_DCLICK, wxMouseEventHandler( EffectCtrl::onMouseEvents ) );
	this->Connect( wxEVT_LEAVE_WINDOW, wxMouseEventHandler( EffectCtrl::onMouseEvents ) );
	this->Connect( wxEVT_ENTER_WINDOW, wxMouseEventHandler( EffectCtrl::onMouseEvents ) );
	this->Connect( wxEVT_MOUSEWHEEL, wxMouseEventHandler( EffectCtrl::onMouseEvents ) );
	m_enableEffect->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( EffectCtrl::onActiveEffect ), NULL, this );
	m_toggleWindow->Connect( wxEVT_COMMAND_TOGGLEBUTTON_CLICKED, wxCommandEventHandler( EffectCtrl::onToggleEffectProperty ), NULL, this );
}

EffectCtrl::~EffectCtrl()
{
	// Disconnect Events
	this->Disconnect( wxEVT_LEFT_DOWN, wxMouseEventHandler( EffectCtrl::onLeftDown ) );
	this->Disconnect( wxEVT_LEFT_UP, wxMouseEventHandler( EffectCtrl::onLeftUp ) );
	this->Disconnect( wxEVT_LEFT_DOWN, wxMouseEventHandler( EffectCtrl::onMouseEvents ) );
	this->Disconnect( wxEVT_LEFT_UP, wxMouseEventHandler( EffectCtrl::onMouseEvents ) );
	this->Disconnect( wxEVT_MIDDLE_DOWN, wxMouseEventHandler( EffectCtrl::onMouseEvents ) );
	this->Disconnect( wxEVT_MIDDLE_UP, wxMouseEventHandler( EffectCtrl::onMouseEvents ) );
	this->Disconnect( wxEVT_RIGHT_DOWN, wxMouseEventHandler( EffectCtrl::onMouseEvents ) );
	this->Disconnect( wxEVT_RIGHT_UP, wxMouseEventHandler( EffectCtrl::onMouseEvents ) );
	this->Disconnect( wxEVT_MOTION, wxMouseEventHandler( EffectCtrl::onMouseEvents ) );
	this->Disconnect( wxEVT_LEFT_DCLICK, wxMouseEventHandler( EffectCtrl::onMouseEvents ) );
	this->Disconnect( wxEVT_MIDDLE_DCLICK, wxMouseEventHandler( EffectCtrl::onMouseEvents ) );
	this->Disconnect( wxEVT_RIGHT_DCLICK, wxMouseEventHandler( EffectCtrl::onMouseEvents ) );
	this->Disconnect( wxEVT_LEAVE_WINDOW, wxMouseEventHandler( EffectCtrl::onMouseEvents ) );
	this->Disconnect( wxEVT_ENTER_WINDOW, wxMouseEventHandler( EffectCtrl::onMouseEvents ) );
	this->Disconnect( wxEVT_MOUSEWHEEL, wxMouseEventHandler( EffectCtrl::onMouseEvents ) );
	m_enableEffect->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( EffectCtrl::onActiveEffect ), NULL, this );
	m_toggleWindow->Disconnect( wxEVT_COMMAND_TOGGLEBUTTON_CLICKED, wxCommandEventHandler( EffectCtrl::onToggleEffectProperty ), NULL, this );
}
