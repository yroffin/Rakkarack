#ifndef __YroJackGuitarMainWindow__
#define __YroJackGuitarMainWindow__

/**
 @file
 Subclass of MainWindow, which is generated by wxFormBuilder.
 */

#include "WxApp.h"

#include <utils/YroLogger.h>
#include <jack/YroJackDriver.h>
#include <jack/YroEffectFactory.h>
#include <wx/WxMainTimer.h>
#include <wx/YroJackGuitarGeneral.h>
#include <wx/YroJackGuitarSignalFrame.h>
#include <wx/YroJackGuitarEffectDistortion.h>
#include <wx/YroJackGuitarEffectExpander.h>

namespace std {

/** Implementing MainWindow */
class YroJackGuitarMainWindow: public MainWindow {
protected:
	// Handlers for MainWindow events.
	void OnClose(wxCloseEvent& event);
	void OnConnect(wxCommandEvent& event);
public:
	/** Constructor */
	static YroJackGuitarMainWindow *instance() {
		if (__instance == 0) {
			__instance = new YroJackGuitarMainWindow(0);
		}
		return __instance;
	}
	virtual ~YroJackGuitarMainWindow();
	void OnJackNewAudioSample();
	void OnJackConnect();
private:
	YroJackGuitarMainWindow(wxWindow* parent);
	static YroJackGuitarMainWindow *__instance;
	YroJackGuitarGeneral *myYroGeneral;
	YroJackGuitarSignalFrame *mySignalFrame;
	YroJackGuitarEffectDistortion *myDistortionFrame;
	YroJackGuitarEffectExpander *myExpanderFrame;
	WxMainTimer timer;
};

}
#endif // __YroJackGuitarMainWindow__
