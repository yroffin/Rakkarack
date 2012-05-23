#ifndef __YroJackGuitarEffectCtrl__
#define __YroJackGuitarEffectCtrl__

/**
@file
Subclass of EffectCtrl, which is generated by wxFormBuilder.
*/

#include <plugins/YroEffectPlugin.h>
#include <wx/WxApp.h>

namespace std {
class YroEffectPlugin;

/** Implementing EffectCtrl */
class YroJackGuitarEffectCtrl : public EffectCtrl
{
public:
	/** Constructor */
	YroJackGuitarEffectCtrl( wxWindow* parent, YroEffectPlugin *_effect, YroJackGuitarEffectCtrl *_previous, wxFrame *_frame );
	void onLeftDown( wxMouseEvent& event );
	void onLeftUp( wxMouseEvent& event );
	void onMouseEvents( wxMouseEvent& event );
	void onActiveEffect( wxCommandEvent& event );
	void onToggleEffectProperty( wxCommandEvent& event );
	void paintLink(wxPaintDC &dc);
	wxPoint getInputPosition();
	wxPoint getOutputPosition();
protected:
	YroLogger *LOG;
private:
	YroEffectPlugin *effect;
	wxFrame *frame;
	YroJackGuitarEffectCtrl *previous;
	wxPoint mouseOnScreenOnBeginDragging;
};

}
#endif // __YroJackGuitarEffectCtrl__