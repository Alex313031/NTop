cmake_minimum_required(VERSION 3.10)
project(NTop C)

option(ENABLE_UNICODE "Compile with Unicode support" OFF)

if(ENABLE_UNICODE)
	add_definitions(-DUNICODE -D_UNICODE)
endif()

add_executable(NTop src/ntop.c src/ntop.rc src/util.c src/vi.c)
