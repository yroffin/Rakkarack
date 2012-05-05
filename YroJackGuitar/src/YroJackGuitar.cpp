//============================================================================
// Name        : YroJackGuitar.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include "jack/YroJackDriver.h"
#include "utils/YroLogger.h"

using namespace std;

int main() {
	YroLogger::instance()->info("YroJackStart ...");
	YroJackDriver::instance()->initialize();
	cout << "!!!Hello World!!!" << endl; // prints !!!Hello World!!!
	getchar();
	delete YroJackDriver::instance();
	return 0;
}
