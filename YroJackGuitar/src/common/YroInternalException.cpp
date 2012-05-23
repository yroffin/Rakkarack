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

#include <common/YroInternalException.h>

YroInternalException::YroInternalException(const char * _message) {
	message = strdup(_message);
}

YroInternalException::YroInternalException(const char *format, ...) {
	message = new char[8192];
	va_list ap;
	va_start(ap, format);
	memset((void *) message, 0, sizeof(message));
	vsnprintf((char *) message, sizeof(message), format, ap);
	va_end(ap);
}
