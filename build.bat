@echo off

call "C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\bin\vcvars32.bat"

IF NOT EXIST build mkdir build
pushd build

IF "%NTOP_VERSION%"=="" SET NTOP_VERSION=dev

REM Same flags as the Makefile/CMake builds: target XP+, import
REM GetProcessMemoryInfo from psapi.dll, Unicode build, SSE2 float math.
REM Remove /DUNICODE /D_UNICODE for an ANSI build.
SET NTOP_FLAGS=/D_WIN32_WINNT=0x0501 /DWINVER=0x0501 /DPSAPI_VERSION=1 /DUNICODE /D_UNICODE -W4 /GA /MT /arch:SSE2
SET NTOP_LIBS=Kernel32.lib Advapi32.lib User32.lib Psapi.lib

IF "%~1"=="-release" (
	REM Release build
  echo NTop Release build
	rc /nologo /fo ntop.res ..\src\ntop.rc
	cl /DNTOP_VER="%NTOP_VERSION%" %NTOP_FLAGS% /O2 ..\src\ntop.c ..\src\util.c ..\src\vi.c ntop.res %NTOP_LIBS% /link /SUBSYSTEM:CONSOLE,5.01
) else (
  REM Debug build
  echo NTop Debug build
  rc /nologo /D_DEBUG /fo ntop.res ..\src\ntop.rc
  cl /DNTOP_VER=%NTOP_VERSION% %NTOP_FLAGS% /Z7 ..\src\ntop.c ..\src\util.c ..\src\vi.c ntop.res %NTOP_LIBS% /link /SUBSYSTEM:CONSOLE,5.01
)

echo Built NTop version %NTOP_VERSION%!

popd
