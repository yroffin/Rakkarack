/*
 ZynAddSubFX - a software synthesizer
 
 Copyright (C) 2002-2005 Nasca Octavian Paul
 Author: Nasca Octavian Paul

 Modified for rakarrack by Josep Andreu
 Modified for YroJackGuitar by Yannick Roffin

 This program is free software; you can redistribute it and/or modify
 it under the terms of version 2 of the GNU General Public License 
 as published by the Free Software Foundation.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License (version 2) for more details.

 You should have received a copy of the GNU General Public License (version 2)
 along with this program; if not, write to the Free Software Foundation,
 Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
 
 WxMainTimer.cpp
 */

#include <plugins/YroEffectPlugin.h>
#include <wx/YroJackGuitarMainWindow.h>
#include <wx/WxMainTimer.h>

namespace std {

WxMainTimer::WxMainTimer() {
}

WxMainTimer::~WxMainTimer() {
}

void WxMainTimer::setNotified(wxFrame *_mainWindow) {
	mainWindow = _mainWindow;
}

void WxMainTimer::Notify() {
	((YroJackGuitarMainWindow *) mainWindow)->OnJackNewAudioSample();
}

}
/* namespace std */
