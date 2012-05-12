#include "YroJackGuitarSignalFrame.h"

using namespace std;

YroJackGuitarSignalFrame::YroJackGuitarSignalFrame(wxWindow* parent) :
		SignalFrame(parent) {
	this->Centre(0);
	audioSampleFactory = YroAudioSampleFactory::instance();
}

void YroJackGuitarSignalFrame::OnPaint(wxPaintEvent& event) {
	jack_default_audio_sample_t *left = audioSampleFactory->get("extern:left");

	wxPaintDC dc(this);
	dc.DrawText(wxT("min:"), 40, 60);
	dc.DrawText(wxT("max:"), 40, 80);

	if(left != 0) {
		wxCoord y = 0;
		wxSize size = this->GetSize();

		for (wxCoord x = 0; x < 1000; x++) {
			dc.DrawPoint(x % size.x, 1 % size.y);
			dc.DrawPoint(x % size.x, ((wxCoord) (left[y]*150)) % size.y + size.y / 2);
		}
	}
}

/**
 * internal jack event
 * produced when new bulk of data is transformed
 */
void std::YroJackGuitarSignalFrame::OnJackNewAudioSample() {
	return;
	this->Refresh(true,0);
}
