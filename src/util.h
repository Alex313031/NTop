/* 
 * NTop - an htop clone for Windows
 * Copyright (c) 2019 Gian Sass
 * 
 * This program is free software: you can redistribute it and/or modify  
 * it under the terms of the GNU General Public License as published by  
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but 
 * WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU 
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License 
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef UTIL_H
#define UTIL_H

#include <windows.h>
#include <tchar.h>
#include <stdarg.h>

#ifdef _MSC_VER
	#define NORETURN __declspec(noreturn)
#elif defined(__GNUC__) && defined(__MINGW32__)
	#define NORETURN __attribute__((noreturn))
#else
	#define NORETURN
#endif

NORETURN void Die(TCHAR *Fmt, ...);
ULONGLONG CompatGetTickCount64(void);

/*
 * Truncating, always NUL-terminating replacements for the secure CRT string
 * functions, which are missing from msvcrt.dll on XP. Argument order matches
 * the _s originals (_tcscpy_s -> StrCopy, _tcsncpy_s -> StrCopyN, etc.).
 */
void StrCopy(TCHAR *Dest, size_t DestSize, const TCHAR *Src);
void StrCopyN(TCHAR *Dest, size_t DestSize, const TCHAR *Src, size_t Count);
void StrCat(TCHAR *Dest, size_t DestSize, const TCHAR *Src);
int StrPrintf(TCHAR *Dest, size_t DestSize, const TCHAR *Fmt, ...);
int StrVPrintf(TCHAR *Dest, size_t DestSize, const TCHAR *Fmt, va_list VaList);

void *xmalloc(size_t size);
void *xrealloc(void *ptr, size_t size);
void *xcalloc(size_t num, size_t size);

#ifdef UNICODE
	/*
	 * Zero extension is correct for converting any legal ASCII char
	 * to its corresponding UTF-16LE code unit
	 */
	#define TCharFromAscii(c) ((TCHAR)(unsigned char)c)
#else
	#define TCharFromAscii(c) (c)
#endif

/*
 * Use these zero-extending functions to defend against stdlib undefined behavior
 * specified by the is*-family of ctype functions
 */
#define IntFromTChar(t) ((int)(unsigned int)t)
#define IntFromChar(c) ((int)(unsigned int)c)

#endif
