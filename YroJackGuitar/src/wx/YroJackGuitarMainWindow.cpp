#include <wx/YroJackGuitarMainWindow.h>

using namespace std;

YroJackGuitarMainWindow *YroJackGuitarMainWindow::__instance = 0;

YroJackGuitarMainWindow::YroJackGuitarMainWindow(wxWindow* parent) :
		MainWindow(parent) {
	mySignalFrame = 0;
	myDistortionFrame = 0;
	myExpanderFrame = 0;
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
	if(mySignalFrame != 0) {
		mySignalFrame->OnJackNewAudioSample();
	}
}

void YroJackGuitarMainWindow::OnJackConnect() {
	/**
	 * Create resources
	 */
	mySignalFrame = new YroJackGuitarSignalFrame(this);
	myDistortionFrame = new YroJackGuitarEffectDistortion(this, (Distortion *) YroEffectFactory::instance()->getEffect("#2.distortion"));
	myExpanderFrame = new YroJackGuitarEffectExpander(this, (Expander *) YroEffectFactory::instance()->getEffect("#3.expander"));
	myYroGeneral = new YroJackGuitarGeneral(
			this,
			(YroAmpli *) YroEffectFactory::instance()->getEffect("#1.ampli"),
			(YroAmpli *) YroEffectFactory::instance()->getEffect("#4.ampli"));

	timer.setNotified(this);
	timer.Start(100);

	/**
	 * Display them
	 */
	mySignalFrame->Show(true);
	myDistortionFrame->Show(true);
	myExpanderFrame->Show(true);
	myYroGeneral->Show(true);

	/**
	 * store effects
	 * ask on plugin factory to retrieve all effects
	 */
	map<const char*, YroEffectPlugin*, cmp_str> effects = YroJackDriver::instance()->getEffectFactory()->getEffects();
	map<const char *,YroEffectPlugin *>::iterator effect;
	WxEffect *prec = 0;
	for(effect=effects.begin(); effect!=effects.end(); effect++) {
		YroEffectPlugin *data = effect->second;
		prec = new WxEffect(this, data, prec);
		myEffects.push_back(prec);
	}
}

