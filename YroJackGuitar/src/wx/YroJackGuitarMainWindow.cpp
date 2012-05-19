#include <wx/YroJackGuitarMainWindow.h>

using namespace std;

YroJackGuitarMainWindow *YroJackGuitarMainWindow::__instance = 0;

YroJackGuitarMainWindow::YroJackGuitarMainWindow(wxWindow* parent) :
		MainWindow(parent) {
	mySignalFrame = 0;
	myDistortionFrame = 0;
	myExpanderFrame = 0;
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
	myDistortionFrame = new YroJackGuitarEffectDistortion(this, (Distortion *) YroEffectFactory::instance()->getEffect("distortion#1"));
	myExpanderFrame = new YroJackGuitarEffectExpander(this, (Expander *) YroEffectFactory::instance()->getEffect("expander#1"));
	myYroGeneral = new YroJackGuitarGeneral(
			this,
			(YroAmpli *) YroEffectFactory::instance()->getEffect("ampli#1"),
			(YroAmpli *) YroEffectFactory::instance()->getEffect("ampli#2"));

	timer.setNotified(this);
	timer.Start(100);

	/**
	 * Display them
	 */
	mySignalFrame->Show(true);
	myDistortionFrame->Show(true);
	myExpanderFrame->Show(true);
	myYroGeneral->Show(true);
}

