#include "YroJackGuitarResource.h"

YroJackGuitarResource::YroJackGuitarResource(wxWindow* parent) :
		Resource(parent) {

}

wxMenu* YroJackGuitarResource::getPopup1() const {
	return m_popup1;
}

