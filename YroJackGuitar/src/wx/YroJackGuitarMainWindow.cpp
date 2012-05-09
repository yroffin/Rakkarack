#include "YroJackGuitarMainWindow.h"

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
	 * signal frame
	 */
	mySignalFrame = new YroJackGuitarSignalFrame(this);
	mySignalFrame->Show(true);
	/**
	 * signal frame
	 */
	myDistortionFrame = new YroJackGuitarEffectDistortion(this);
	myDistortionFrame->Show(true);
}

