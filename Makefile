CC=g++
RESSW=--define __WIN32__ --define __WIN95__ --define __GNUWIN32__
OBJECTS=file.o scummex.o resource.o mixer.o image.o sound.o wxwindows.o descumm.o
CFLAGS=-DOSUNIX -g -Wall -Wstrict-prototypes -Wuninitialized -Wno-unused-variable -Wno-long-long -Wno-multichar -Wno-unknown-pragmas `wx-config --cxxflags` `sdl-config --cflags`
LIBS=`wx-config --libs` `sdl-config --libs` -lSDL_mixer

.SUFFIXES: .o .cpp

all:    ${OBJECTS}
	$(CC) -o scummex ${OBJECTS} ${LIBS}

.cpp.o: 
	$(CC) ${CFLAGS} -c -o $@ $<
	
clean:
	rm -f scummex file.o scummex.o resource.o mixer.o image.o sound.o wxwindows.o descumm.o
