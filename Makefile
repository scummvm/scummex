CXX     := c++
AR      := ar cru
RANLIB  := ranlib
MKDIR   := mkdir -p
ECHO    := echo -n
CAT     := cat
RM      := rm -f
RM_REC  := $(RM) -r
ZIP     := zip -q
CP      := cp

RESSW    := --define __WIN32__ --define __WIN95__ --define __GNUWIN32__
REZ_CMD  := `wx-config --rezflags`
OBJS     := file.o scummex.o resource.o image.o wxwindows.o descumm.o descumm6.o codec37.o codec47.o bomp.o scaler.o util.o
CXXFLAGS := -DOSUNIX -g -O -Wall -Wuninitialized -Wshadow -Wstrict-prototypes -Wno-unused-variable -Wno-long-long -Wno-multichar -Wno-unknown-pragmas
CXXFLAGS += `wx-config --cxxflags` `sdl-config --cflags`
LIBS     := `wx-config --libs` `sdl-config --libs`
INCLUDES := -I.
CXXFLAGS += $(INCLUDES)

MODULE_DIRS :=
MODULES := sound

# Default build rule
all: scummex

-include $(addsuffix /module.mk,$(MODULES))

# The name for the directory used for depenency tracking
DEPDIR  := .deps
DEPDIRS := $(addsuffix /$(DEPDIR),$(MODULE_DIRS))

# Main executable build rule
scummex: ${OBJS}
	$(CXX) $+ ${LIBS} -o $@
ifdef REZ_CMD
	$(REZ_CMD) scummex
endif

clean:
	rm -f scummex $(OBJS)

distclean: clean
	$(RM_REC) $(DEPDIRS)

.PHONY: all clean distclean mac
.SUFFIXES: .o .cpp


# C++ build rule with dependency tracking (requires GCC)
.cpp.o:
	$(MKDIR) $(*D)/$(DEPDIR)
	$(CXX) -Wp,-MMD,"$(*D)/$(DEPDIR)/$(*F).d2" $(CXXFLAGS) $(CPPFLAGS) -c $(<) -o $*.o
	$(ECHO) "$(*D)/" > $(*D)/$(DEPDIR)/$(*F).d
	$(CAT) "$(*D)/$(DEPDIR)/$(*F).d2" >> "$(*D)/$(DEPDIR)/$(*F).d"
	$(RM) "$(*D)/$(DEPDIR)/$(*F).d2"

# Include the dependency tracking files. We add /dev/null at the end
# of the list to avoid a warning/error if no .d file exist
-include $(wildcard $(addsuffix /*.d,$(DEPDIRS))) /dev/null
