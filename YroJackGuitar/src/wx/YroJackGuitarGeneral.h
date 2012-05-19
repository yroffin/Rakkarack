#ifndef __YroJackGuitarGeneral__
#define __YroJackGuitarGeneral__

/**
@file
Subclass of General, which is generated by wxFormBuilder.
*/

#include <wx/WxApp.h>
#include <jack/YroEffectFactory.h>

using namespace std;

/** Implementing General */
class YroJackGuitarGeneral : public General
{
protected:
	// Handlers for General events.
	void onUpdateInputAmpli( wxScrollEvent& event );
	void onUpdateOutputAmpli( wxScrollEvent& event );
	
public:
	/** Constructor */
	YroJackGuitarGeneral( wxWindow* parent, YroAmpli *inputAmpli, YroAmpli *outputAmpli);
private:
	YroAmpli *inputAmpli;
	YroAmpli *outputAmpli;
};

#endif // __YroJackGuitarGeneral__