#include <wx/YroJackGuitarMainWindow.h>
#include <wx/YroJackGuitarEffectCtrl.h>

using namespace std;

YroJackGuitarMainWindow *YroJackGuitarMainWindow::__instance = 0;
wxFont _defaultFont = wxFont( 8, 77, 90, 90, false, wxT("Ubuntu Condensed"));

YroJackGuitarMainWindow::YroJackGuitarMainWindow(wxWindow* parent) :
		MainWindow(parent) {
	/**
	 * fix default font for application
	 */
	_defaultFont = wxFont( 8, 77, 90, 90, false, wxT("Ubuntu Condensed"));
	SetFont( _defaultFont );
	/**
	 * add png handler capacity
	 */
	wxImage::AddHandler(new wxPNGHandler);

	/**
	 * TODO analyze command lone and add on new option
	 * --auto to force direct connect on jack system
	 */
	wxCommandEvent event;
	OnConnect(event);
}

YroJackGuitarMainWindow::~YroJackGuitarMainWindow() {
}

void YroJackGuitarMainWindow::OnConnect(wxCommandEvent& event) {
	YroJackDriver::instance()->initialize();
	OnJackConnect();
}

void YroJackGuitarMainWindow::OnClose(wxCloseEvent& event) {
	delete YroJackDriver::instance();
	this->Close(false);
}

void YroJackGuitarMainWindow::OnJackNewAudioSample() {
	/**
	 * TODO observer pattern to implement
	if(mySignalFrame != 0) {
		mySignalFrame->OnJackNewAudioSample();
	}
	 */
}

void YroJackGuitarMainWindow::OnJackConnect() {
	/**
	 * synchronize with the effect factory
	 */
	synchronizeWithPluginFactory();

	/**
	 * fix timer
	 */
	timer.setNotified(this);
	timer.Start(100);

	/**
	 * store effects
	 * ask on plugin factory to retrieve all effects
	 */
	map<const char*, YroEffectPlugin*, cmp_str> effects = YroJackDriver::instance()->getEffectFactory()->getEffects();
	map<const char *,YroEffectPlugin *>::iterator effect;
	YroJackGuitarEffectCtrl *previous = 0;
	for(effect=effects.begin(); effect!=effects.end(); effect++) {
		YroEffectPlugin *data = effect->second;
		previous = new YroJackGuitarEffectCtrl(m_panelEffects, data, previous, getFrame(data));
		myEffects.push_back(previous);
	}
	m_panelEffects->Refresh(true);
}

void std::YroJackGuitarMainWindow::synchronizeWithPluginFactory() {
	/**
	 * all effects must have a propertie
	 * frame
	 */
	map<const char*, YroEffectPlugin*, cmp_str> effects = YroJackDriver::instance()->getEffectFactory()->getEffects();
	map<const char *,YroEffectPlugin *>::iterator effect;
	for(effect=effects.begin(); effect!=effects.end(); effect++) {
		if(myJackEffects.find(effect->second) != myJackEffects.end()) {
			continue;
		}
		/**
		 * jask effect is not register
		 */
		switch(effect->second->klass) {
			case YroEffectPlugin::_Default:
				break;
			case YroEffectPlugin::_Distortion:
				myJackEffects[effect->second] = new YroJackGuitarEffectDistortion(m_panelEffects, (Distortion *) effect->second);
				break;
			case YroEffectPlugin::_Expander:
				myJackEffects[effect->second] = new YroJackGuitarEffectExpander(m_panelEffects, (Expander *) effect->second);
				break;
			case YroEffectPlugin::_YroScope:
				myJackEffects[effect->second] = new YroJackGuitarSignalFrame(m_panelEffects, (YroScope *) effect->second);
				break;
			case YroEffectPlugin::_YroAmpli:
				myJackEffects[effect->second] = new YroJackGuitarGeneral(m_panelEffects, (YroAmpli *) effect->second);
				break;
		}
		myJackEffects[effect->second]->SetFont( GetFont() );
	}
}

wxFrame* std::YroJackGuitarMainWindow::getFrame(YroEffectPlugin* plugin) {
	return myJackEffects[plugin];
}

void std::YroJackGuitarMainWindow::onPaintEffects(wxPaintEvent& event) {
	wxPaintDC dc(m_panelEffects);
	vector<YroJackGuitarEffectCtrl *>::iterator effect;
	for(effect=myEffects.begin(); effect!=myEffects.end(); effect++) {
		YroJackGuitarEffectCtrl *data = *effect;
		data->paintLink(dc);
	}
}
