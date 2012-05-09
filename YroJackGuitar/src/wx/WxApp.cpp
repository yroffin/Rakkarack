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
	
	wxFlexGridSizer* fgSizer3;
	fgSizer3 = new wxFlexGridSizer( 2, 2, 0, 0 );
	fgSizer3->SetFlexibleDirection( wxBOTH );
	fgSizer3->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText1 = new wxStaticText( this, wxID_ANY, wxT("Input amplification"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText1->Wrap( -1 );
	fgSizer3->Add( m_staticText1, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_Pdrive = new wxSpinCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 10, 0 );
	fgSizer3->Add( m_Pdrive, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_staticText2 = new wxStaticText( this, wxID_ANY, wxT("Preset"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText2->Wrap( -1 );
	fgSizer3->Add( m_staticText2, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	wxString m_PresetChoices[] = { wxT("Overdrive 1"), wxT("Overdrive 2"), wxT("Distorsion 1"), wxT("Distorsion 2"), wxT("Distortion 3"), wxT("Guitar Amp") };
	int m_PresetNChoices = sizeof( m_PresetChoices ) / sizeof( wxString );
	m_Preset = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_PresetNChoices, m_PresetChoices, 0 );
	m_Preset->SetSelection( 0 );
	m_Preset->SetToolTip( wxT("Select preset type") );
	
	fgSizer3->Add( m_Preset, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	this->SetSizer( fgSizer3 );
	this->Layout();
	
	// Connect Events
	m_Pdrive->Connect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( EffectDistortion::OnSpinCtrl ), NULL, this );
	m_Pdrive->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( EffectDistortion::OnSpinCtrlText ), NULL, this );
	m_Preset->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( EffectDistortion::OnChoice ), NULL, this );
}

EffectDistortion::~EffectDistortion()
{
	// Disconnect Events
	m_Pdrive->Disconnect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( EffectDistortion::OnSpinCtrl ), NULL, this );
	m_Pdrive->Disconnect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( EffectDistortion::OnSpinCtrlText ), NULL, this );
	m_Preset->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( EffectDistortion::OnChoice ), NULL, this );
}
