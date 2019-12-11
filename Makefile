CXX = g++
CXXSTD := c++17
CXXFLAG = -Wall -Wextra -O0 -g -std=${CXXSTD}
LIBS = -lSDL2main -lSDL2 -lSDL2_ttf
TESTDIR = ./test

test.exe: ${TESTDIR}/a.cpp wSDL.hpp
	${CXX} -I. ${CXXFLAG} $< -o $@ ${LIBS}

clean:
	rm test.exe

before-install:
	sudo apt update
	sudo apt install mercurial

install-%:
	hg clone https://hg.libsdl.org/% %
	cd % && mkdir build && cd build && ../configure && make && sudo make install

actions: install-SDL install-SDL_ttf install-SDL_image test.exe

.PHONY: clean install-% actions
