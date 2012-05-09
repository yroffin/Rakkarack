#include "YroJackGuitarMainWindow.h"

using namespace std;

YroJackGuitarMainWindow *YroJackGuitarMainWindow::__instance = 0;

YroJackGuitarMainWindow::YroJackGuitarMainWindow(wxWindow* parent) :
		MainWindow(parent) {
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

YroJackGuitarMainWindow::~YroJackGuitarMainWindow() {
}

void YroJackGuitarMainWindow::OnConnect(wxCommandEvent& event) {
	YroJackDriver::instance()->initialize();
}

void YroJackGuitarMainWindow::OnClose(wxCloseEvent& event) {
	delete YroJackDriver::instance();
	this->Close(false);
}

void std::YroJackGuitarMainWindow::OnJackNewAudioSample() {
	if(mySignalFrame != 0) {
		mySignalFrame->OnJackNewAudioSample();
	}
}
