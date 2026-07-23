cmake_minimum_required(VERSION 3.13)
project(NTop C)

option(ENABLE_UNICODE "Compile with Unicode support" ON)
set(NTOP_VER "0.3.22" CACHE STRING "NTop version string")

add_executable(NTop src/ntop.c src/ntop.rc src/util.c src/vi.c)

# Target Windows XP and later. PSAPI_VERSION=1 keeps GetProcessMemoryInfo an
# import from psapi.dll (present on every Windows version); the default on
# newer SDKs resolves it to K32GetProcessMemoryInfo, which is Win7+ only.
target_compile_definitions(NTop PRIVATE
	NTOP_VER=${NTOP_VER}
	_WIN32_WINNT=0x0501
	WINVER=0x0501
	PSAPI_VERSION=1
)

target_link_libraries(NTop kernel32 advapi32 user32 psapi)

if(ENABLE_UNICODE)
	target_compile_definitions(NTop PRIVATE UNICODE _UNICODE)
endif()

if(MSVC)
	# Match the MinGW flags: SSE2 float math and an XP-loadable subsystem
	# version (5.01 is the x86 minimum, 5.02 the x64 minimum).
	if(CMAKE_SIZEOF_VOID_P EQUAL 4)
		# x64 cl has SSE2 on by default and rejects /arch:SSE2
		target_compile_options(NTop PRIVATE /arch:SSE2)
		target_link_options(NTop PRIVATE /SUBSYSTEM:CONSOLE,5.01)
	else()
		target_link_options(NTop PRIVATE /SUBSYSTEM:CONSOLE,5.02)
	endif()
endif()

if(MINGW)
	# Same flags as the MinGW Makefile
	target_compile_options(NTop PRIVATE -Wall -msse2 -mfpmath=sse)
	# Keep the PE subsystem version at 5.01 so the binary still loads on XP.
	target_link_options(NTop PRIVATE -static-libgcc -Wl,--subsystem,console:5.01)
	if(ENABLE_UNICODE)
		# -municode selects the wide (wmain) CRT startup that _tmain expands to
		target_compile_options(NTop PRIVATE -municode)
		target_link_options(NTop PRIVATE -municode)
	endif()
endif()
