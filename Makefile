CXX = g++
CXXSTD = c++17
CXXFLAG = -Wall -Wextra -O0 -g -std=${CXXSTD}
LIBS = -lSDL2main -lSDL2 -lSDL2_ttf
TESTDIR = ./test

ifeq (${OS},Windows_NT)
MINGW = -lmingw32
endif

test.exe: ${TESTDIR}/a.cpp wSDL.hpp
	${CXX} -I. ${CXXFLAG} $< -o $@ ${MINGW} ${LIBS}

clean:
	rm test.exe

install-sdl:
	sudo apt update
	sudo apt install libsdl2-dev
	sudo apt install libsdl2-ttf-dev
	sudo apt install libsdl2-image-dev

actions: install-sdl test.exe

.PHONY: clean actions
