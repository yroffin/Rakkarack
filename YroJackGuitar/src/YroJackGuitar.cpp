//============================================================================
// Name        : YroJackGuitar.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <wx/wx.h>

#include "jack/YroJackDriver.h"
#include "utils/YroLogger.h"
#include "wx/YroJackGuitarMainWindow.h"

using namespace std;

class MyApp: public wxApp {
public:
	virtual bool OnInit();
};

bool MyApp::OnInit() {
	YroJackGuitarMainWindow *myWnd = YroJackGuitarMainWindow::instance();
	myWnd->Show(true);
	return true;
}

#define __CPPUNIT
#ifndef __CPPUNIT
IMPLEMENT_APP(MyApp)
#else

#include <iostream>
#include <cppunit/CompilerOutputter.h>
#include <cppunit/TextOutputter.h>
#include <cppunit/XmlOutputter.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TestRunner.h>
#include <tests/YroCompilerOutputter.h>

#include <tests/effects/YroEffectFactoryTest.h>
#include <tests/utils/YroUtilTest.h>

CPPUNIT_TEST_SUITE_REGISTRATION( test::YroUtilTest );
CPPUNIT_TEST_SUITE_REGISTRATION( test::YroEffectFactoryTest );

using namespace std;

int main(int argc, char* argv[]) {
	// Get the top level suite from the registry
	CppUnit::Test *suite =
			CppUnit::TestFactoryRegistry::getRegistry().makeTest();

	// Adds the test to the list of test to run
	CppUnit::TextUi::TestRunner runner;
	runner.addTest(suite);

	// Change the default outputter to a compiler error format outputter
	runner.setOutputter(
			new CppUnit::XmlOutputter(&runner.result(), std::cerr));
	// Run the tests.
	bool wasSucessful = runner.run();

	// Return error code 1 if the one of test failed.
	return wasSucessful ? 0 : 1;
}

#endif
