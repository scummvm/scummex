CXX      := g++
RESSW    := --define __WIN32__ --define __WIN95__ --define __GNUWIN32__
REZ_CMD  := `wx-config --rezflags`
OBJS     := file.o scummex.o resource.o mixer.o image.o sound.o wxwindows.o descumm.o descumm6.o
CXXFLAGS := -DOSUNIX -g -O -Wall -Wuninitialized -Wshadow -Wstrict-prototypes -Wno-unused-variable -Wno-long-long -Wno-multichar -Wno-unknown-pragmas
CXXFLAGS += `wx-config --cxxflags` `sdl-config --cflags`
LIBS     := `wx-config --libs` `sdl-config --libs` -lSDL_mixer

.SUFFIXES: .o .cpp

all: scummex

.cpp.o: 
	$(CXX) ${CXXFLAGS} -c $< -o $@

scummex: ${OBJS}
	$(CXX) $+ ${LIBS} -o $@

# Special build rule for MacOS: need to add the resource fork to the binary. 
# Not sure if we can just use REZ_CMD unconditionally on all platforms?
mac: scummex
	$(REZ_CMD) scummex

clean:
	rm -f scummex file.o scummex.o resource.o mixer.o image.o sound.o wxwindows.o descumm.o descumm6.o

.PHONY: all clean mac
