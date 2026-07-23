@echo off

call "C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\bin\vcvars32.bat"

IF NOT EXIST build mkdir build
pushd build

IF "%NTOP_VERSION%"=="" SET NTOP_VERSION=dev

IF "%~1"=="-release" (
	REM Release build
  echo NTop Release build
	rc /nologo /fo ntop.res ..\src\ntop.rc
	cl /DNTOP_VER="%NTOP_VERSION%" -W4 /GA /MT /O2 ..\src\ntop.c ..\src\util.c ..\src\vi.c ntop.res Advapi32.lib User32.lib
) else (
  REM Debug build
  echo NTop Debug build
  rc /nologo /D_DEBUG /fo ntop.res ..\src\ntop.rc
  cl /DNTOP_VER=%NTOP_VERSION% -W4 /GA /MT /Z7 ..\src\ntop.c ..\src\util.c ..\src\vi.c ntop.res Advapi32.lib User32.lib
)

echo Built NTop version %NTOP_VERSION%!

popd
