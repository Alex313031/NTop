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

#include "util.h"
#include <stdio.h>
#include <windows.h>

void StrCopyN(TCHAR *Dest, size_t DestSize, const TCHAR *Src, size_t Count)
{
	size_t i;

	if(DestSize == 0)
		return;

	for(i = 0; i < Count && i + 1 < DestSize && Src[i]; i++) {
		Dest[i] = Src[i];
	}
	Dest[i] = 0;
}

void StrCopy(TCHAR *Dest, size_t DestSize, const TCHAR *Src)
{
	StrCopyN(Dest, DestSize, Src, (size_t)-1);
}

void StrCat(TCHAR *Dest, size_t DestSize, const TCHAR *Src)
{
	size_t Len = _tcslen(Dest);

	if(Len >= DestSize)
		return;

	StrCopyN(Dest + Len, DestSize - Len, Src, (size_t)-1);
}

int StrVPrintf(TCHAR *Dest, size_t DestSize, const TCHAR *Fmt, va_list VaList)
{
	int Len;

	if(DestSize == 0)
		return 0;

	Len = _vsntprintf(Dest, DestSize, Fmt, VaList);
	if(Len < 0 || (size_t)Len >= DestSize) {
		Dest[DestSize - 1] = 0;
		Len = (int)_tcslen(Dest);
	}

	return Len;
}

int StrPrintf(TCHAR *Dest, size_t DestSize, const TCHAR *Fmt, ...)
{
	va_list VaList;
	int Len;

	va_start(VaList, Fmt);
	Len = StrVPrintf(Dest, DestSize, Fmt, VaList);
	va_end(VaList);

	return Len;
}

NORETURN void Die(TCHAR *Fmt, ...)
{
	TCHAR Buffer[1024];
	va_list VaList;

	va_start(VaList, Fmt);
	StrVPrintf(Buffer, _countof(Buffer), Fmt, VaList);
	va_end(VaList);

	system("cls");

	HANDLE ErrorHandle = GetStdHandle(STD_ERROR_HANDLE);
	DWORD Written;

	if(!WriteConsole(ErrorHandle, Buffer, (DWORD)_tcslen(Buffer), &Written, 0)) {
		/* stderr is redirected to a file or pipe: fall back to raw bytes */
		WriteFile(ErrorHandle, Buffer, (DWORD)(sizeof(*Buffer) * _tcslen(Buffer)), &Written, 0);
	}
	exit(EXIT_FAILURE);
}

extern HANDLE ConsoleHandle;

/*
 * GetTickCount64 only exists on Vista and later. Resolve it dynamically so
 * that we can still run on XP, where we fall back to the 32-bit GetTickCount.
 */
typedef ULONGLONG (WINAPI *gettickcount64_fn_t)(void);

ULONGLONG CompatGetTickCount64(void)
{
	static gettickcount64_fn_t GetTickCount64Fn;
	static BOOL Resolved;

	if(!Resolved) {
		GetTickCount64Fn = (gettickcount64_fn_t)GetProcAddress(GetModuleHandle(_T("kernel32.dll")), "GetTickCount64");
		Resolved = TRUE;
	}

	if(GetTickCount64Fn) {
		return GetTickCount64Fn();
	}

	return (ULONGLONG)GetTickCount();
}

/*
 * Instant-fail memory allocators as I believe that helps keep the code clean
 */

void *xmalloc(size_t size)
{
	void *m = malloc(size);

	if(!m)
		Die(_T("malloc"));

	return m;
}

void *xrealloc(void *ptr, size_t size)
{
	void *m = realloc(ptr, size);

	if(!m)
		Die(_T("realloc"));

	return m;
}

void *xcalloc(size_t num, size_t size)
{
	void *m = calloc(num, size);

	if(!m)
		Die(_T("calloc"));

	return m;
}
