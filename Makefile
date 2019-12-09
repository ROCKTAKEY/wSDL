CXX = g++
CXXSTD := c++17
CXXFLAG = -Wall -Wextra -O0 -g -std=${CXXSTD}
LIBS = -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf
TESTDIR = ./test

test.exe: ${TESTDIR}/a.cpp wSDL.hpp
	${CXX} -I. ${CXXFLAG} $< -o $@ ${LIBS}

clean:
	rm test.exe

install-sdl:
	sudo apt-get install mercurial
	hg clone "https://hg.libsdl.org/SDL" "SDL"
	cd sdl
	mkdir build
	../.configure
	make
	sudo make install

actions: install-sdl test.exe

.PHONY: clean actions
