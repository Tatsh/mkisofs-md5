/* @(#)mgw32.c	1.0 04/06/19 Copyright (c)2004 by Alex Kopylov */
/*
 *	Win32/Mingw32 stuff
 *
 *	Copyright (c)2004 by Alex Kopylov
 */
/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program; see the file COPYING.  If not, write to the Free Software
 * Foundation, 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#ifdef	__MINGW32__

#include <windows.h>
#include <limits.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <time.h>

#ifndef	__MINGW32__
struct	timezone {
	int		tz_minuteswest;
	int		tz_dsttime;
};

int gettimeofday(struct timeval *tp, struct timezone *tzp) {

	SYSTEMTIME SystemTime;
	struct tm t;
	time_t tv_sec;

	GetSystemTime(&SystemTime);

	t.tm_sec = SystemTime.wSecond;
	t.tm_sec -= _timezone;
	t.tm_sec -= (_daylight > 0 ? -60 : 0);
	t.tm_min = SystemTime.wMinute;
	t.tm_hour = SystemTime.wHour;
	t.tm_mday = SystemTime.wDay;
	t.tm_mon = SystemTime.wMonth - 1;
	t.tm_year = SystemTime.wYear - 1900;
	t.tm_wday = 0;
	t.tm_yday = 0;
	t.tm_isdst = 0;

	if(tp) {

		tp->tv_sec = mktime(&t);
		tp->tv_usec = (long)SystemTime.wMilliseconds * 1000l;
	}
	
	if(tzp) {

		tzp->tz_minuteswest = _timezone / 60;
		tzp->tz_dsttime = _daylight;
	}

	return (0);
}
#endif	/* MINGW Internal Defines */

int __mgw32__stat(const char *path, struct stat *buf){

	char tmppath[PATH_MAX+1];

	strncpy(tmppath, path, PATH_MAX);
	if(tmppath[strlen(tmppath)-1] == '/') {
		tmppath[strlen(tmppath)-1] = '\0';
	}

	return (stat((const char *)tmppath, buf));
}

struct tm* __mgw32__gmtime(const time_t *t) {

	time_t min_t = 0;
	time_t max_t = 0x7fffffff;

	if (!t)
		return (gmtime(&min_t));
	
	if (*t < 0)
		return (gmtime(&max_t));

	return (gmtime(t));
}

struct tm* __mgw32__localtime(const time_t *t) {

	time_t min_t = 0;
	time_t max_t = 0x7fffffff;

	if (!t)
		return (localtime(&min_t));
	
	if (*t < 0)
		return (localtime(&max_t));

	return (localtime(t));
}

int __mgw32__cursormove(int x, int y) {

	COORD Coord;
	
	Coord.X = x;
	Coord.Y = y;
	
	return (SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Coord)?0:-1);
}

char *__mgw32__icharset() {

	static char cpname[8];
	
	sprintf(cpname, "cp%u", GetACP());
	
	return (cpname);
}

char *__mgw32__ocharset() {

	static char cpname[8];
	
	sprintf(cpname, "cp%u", GetOEMCP());
	
	return (cpname);
}

char *__mgw32__strbs2s(char *s) {

	char	*p = s;

	if (p) {
		while (*p) {
			if (*p == '\\')
				*p = '/';
			p++;
		}
	}

	return (p);
}

void __mgw32__init(char *av0) {

	/* Get time zone information from operating system */
	_tzset();

	if (av0)
		/* '\' -> '/' */
		__mgw32__strbs2s(av0);
}

#endif
