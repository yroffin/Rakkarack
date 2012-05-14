#include "YroJackGuitarSignalFrame.h"

using namespace std;

YroJackGuitarSignalFrame::YroJackGuitarSignalFrame(wxWindow* parent) :
		SignalFrame(parent) {
	this->Centre(0);
	audioSampleFactory = YroAudioSampleFactory::instance();
}

void YroJackGuitarSignalFrame::OnPaint(wxPaintEvent& event) {
	wxPaintDC dc(this);
	dc.DrawText(wxT("min:"), 40, 60);
	dc.DrawText(wxT("max:"), 40, 80);

	jack_nframes_t nframes = YroParamHelper::instance()->getIntegerPeriod();
	jack_default_audio_sample_t *left = audioSampleFactory->allocate(nframes,0,"extern:left");

	if(left != 0) {
		wxCoord y = 0;
		wxSize size = this->GetSize();

		for (wxCoord x = 0; x < (wxCoord) nframes; x++) {
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
	this->Refresh(true);
}
