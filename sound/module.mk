MODULE := sound

MODULE_OBJS := \
	sound/sound.o \
	sound/audiostream.o \
	sound/mixer.o \
	sound/rate.o \
	sound/voc.o

MODULE_DIRS += \
	sound

# Include common rules 
include common.rules
