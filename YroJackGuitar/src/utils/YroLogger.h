/*
 * YroLogger.h
 *
 *  Created on: 4 mai 2012
 *      Author: yannick
 */

#ifndef YROLOGGER_H_
#define YROLOGGER_H_

#include <log4cxx/logger.h>
#include <log4cxx/basicconfigurator.h>
#include <log4cxx/helpers/exception.h>

namespace std {

using namespace log4cxx;
using namespace log4cxx::helpers;

class YroLogger {
public:
	static YroLogger *instance() {
		if (__instance == 0) {
			__instance = new YroLogger();
		}
		return __instance;
	}
	virtual ~YroLogger();
	/**
	 * log4cxx API
	 */
	void info(const char *format, ...);
	void error(const char *format, ...);
	void debug(const char *format, ...);
private:
	YroLogger();
	static YroLogger *__instance;
	LoggerPtr myLogger;
	char buffer[16384];
};

} /* namespace std */
#endif /* YROLOGGER_H_ */
