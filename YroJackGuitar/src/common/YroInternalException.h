/*
 YroJackGuitar - a software synthesizer based on excelent work
 of Rakkarack team

 Copyright (C) 2002-2005 Yannick Roffin
 Author: Yannick Roffin

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
 */


#ifndef YROINTERNALEXCEPTION_H_
#define YROINTERNALEXCEPTION_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

#include <exception>

class YroInternalException: public std::exception {
public:
	YroInternalException(const char *message);
	YroInternalException(const char *format, ...);
	virtual const char* what() const throw () {
		return message;
	}
private:
	const char *message;
};

#endif /* YROINTERNALEXCEPTION_H_ */
