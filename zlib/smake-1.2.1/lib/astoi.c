/* @(#)astoi.c	1.9 06/09/13 Copyright 1985, 1995-2003 J. Schilling */
/*
 *	astoi() converts a string to int
 *	astol() converts a string to long
 *
 *	Leading tabs and spaces are ignored.
 *	Both return pointer to the first char that has not been used.
 *	Caller must check if this means a bad conversion.
 *
 *	leading "+" is ignored
 *	leading "0"  makes conversion octal (base 8)
 *	leading "0x" makes conversion hex   (base 16)
 *
 *	Copyright (c) 1985, 1995-2003 J. Schilling
 */
/*
 * The contents of this file are subject to the terms of the
 * Common Development and Distribution License, Version 1.0 only
 * (the "License").  You may not use this file except in compliance
 * with the License.
 *
 * See the file CDDL.Schily.txt in this distribution for details.
 *
 * When distributing Covered Code, include this CDDL HEADER in each
 * file and include the License file CDDL.Schily.txt from this distribution.
 */

#include <schily/standard.h>
#include <schily/schily.h>

#define	is_space(c)	 ((c) == ' ' || (c) == '\t')
#define	is_digit(c)	 ((c) >= '0' && (c) <= '9')
#define	is_hex(c)	(\
			((c) >= 'a' && (c) <= 'f') || \
			((c) >= 'A' && (c) <= 'F'))

#define	to_lower(c)	(((c) >= 'A' && (c) <= 'Z') ? (c) - 'A'+'a' : (c))

#ifdef	notdef
EXPORT int
atoi(s)
	char	*s;
{
	long	l;

	(void) astol(s, &l);
	return ((int) l);
}

EXPORT long
atol(s)
	char	*s;
{
	long	l;

	(void) astol(s, &l);
	return (l);
}
#endif

EXPORT char *
astoi(s, i)
	const char *s;
	int *i;
{
	long l;
	char *ret;

	ret = astol(s, &l);
	*i = l;
	return (ret);
}

EXPORT char *
astol(s, l)
	register const char *s;
	long *l;
{
	return (astolb(s, l, 0));
}

EXPORT char *
astolb(s, l, base)
	register const char *s;
	long *l;
	register int base;
{
	int neg = 0;
	register long ret = 0L;
	register int digit;
	register char c;

	while (is_space(*s))
		s++;

	if (*s == '+') {
		s++;
	} else if (*s == '-') {
		s++;
		neg++;
	}

	if (base == 0) {
		if (*s == '0') {
			base = 8;
			s++;
			if (*s == 'x' || *s == 'X') {
				s++;
				base = 16;
			}
		} else {
			base = 10;
		}
	}
	for (; (c = *s) != 0; s++) {

		if (is_digit(c)) {
			digit = c - '0';
		} else if (is_hex(c)) {
			digit = to_lower(c) - 'a' + 10;
		} else {
			break;
		}

		if (digit < base) {
			ret *= base;
			ret += digit;
		} else {
			break;
		}
	}
	if (neg)
		ret = -ret;
	*l = ret;
	return ((char *)s);
}
