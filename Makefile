CXX = g++
CXXSTD := c++17
CXXFLAG = -Wall -Wextra -O0 -g -std=${CXXSTD}
LIBS = -lSDL2main -lSDL2 -lSDL2_ttf
TESTDIR = ./test

test.exe: ${TESTDIR}/a.cpp wSDL.hpp
	${CXX} -I. ${CXXFLAG} $< -o $@ ${LIBS}

clean:
	rm test.exe

install-sdl:
	sudo apt-get install mercurial
	hg clone "https://hg.libsdl.org/SDL" "SDL"
	cd SDL && mkdir build && cd build && ../configure && make && sudo make install
	sudo apt get install libsdl2{-image,-ttf}-dev

actions: install-sdl test.exe

.PHONY: clean actions
