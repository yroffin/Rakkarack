#include <wx/YroJackGuitarMainWindow.h>
#include <wx/YroJackGuitarEffectDistortion.h>

using namespace std;

YroJackGuitarMainWindow *YroJackGuitarMainWindow::__instance = 0;

YroJackGuitarMainWindow::YroJackGuitarMainWindow(wxWindow* parent) :
		MainWindow(parent) {
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

	/**
	 * Display them
	 */
	mySignalFrame->Show(true);
	myDistortionFrame->Show(true);
}

