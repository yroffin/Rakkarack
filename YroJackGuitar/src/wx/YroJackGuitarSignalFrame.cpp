#include "YroJackGuitarSignalFrame.h"

using namespace std;

YroJackGuitarSignalFrame::YroJackGuitarSignalFrame(wxWindow* parent) :
		SignalFrame(parent) {
	this->Centre(0);
	audioSampleFactory = YroAudioSampleFactory::instance();
}

void YroJackGuitarSignalFrame::onActivate(wxCommandEvent& event) {
	/*
	 * TODO toggle effect
	effect->toggle();
	if(m_toggle->GetValue()) m_toggle->SetLabel(wxT("On"));
	else  m_toggle->SetLabel(wxT("Off"));
	*/
}

void YroJackGuitarSignalFrame::OnPaint(wxPaintEvent& event) {
	wxPaintDC dc(this);

	jack_nframes_t nframes = YroParamHelper::instance()->getIntegerSampleRate();
	jack_default_audio_sample_t *left = audioSampleFactory->allocate(nframes,0,"extern:left");

	float minl = 0.;
	float maxl = 0.;
	if(left != 0) {
		wxCoord y = 0;
		wxSize size = this->GetSize();

		int ratio = nframes / size.x;

		for (wxCoord x = 0; x < size.x; x++) {
			if(left[y]<minl) minl = left[y];
			if(left[y]>maxl) maxl = left[y];
			dc.DrawPoint(x, ((wxCoord) (left[x*ratio]*850)) + size.y / 2);
		}
	}

	char buffer[128];
	sprintf(buffer,"min: %f",minl);
	wxString s = wxString::FromAscii(buffer);
	dc.DrawText(s, 40, 60);
	sprintf(buffer,"max: %f",maxl);
	s = wxString::FromAscii(buffer);
	dc.DrawText(s, 40, 80);
}

/**
 * internal jack event
 * produced when new bulk of data is transformed
 */
void std::YroJackGuitarSignalFrame::OnJackNewAudioSample() {
	this->Refresh(true);
}
