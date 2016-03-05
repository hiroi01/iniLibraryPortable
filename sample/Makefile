TARGET = sample
OBJS = main.o

BUILD_PRX = 1
INCDIR = ../include
CFLAGS = -G0 -Wall
CXXFLAGS = $(CFLAGS) -fno-exceptions -fno-rtti
ASFLAGS = $(CFLAGS)
 
LIBDIR = ../lib
LDFLAGS =
LIBS = -linip

EXTRA_TARGETS = EBOOT.PBP
PSP_EBOOT_TITLE = INI Library Portable Sample

PSPSDK=$(shell psp-config --pspsdk-path)
include $(PSPSDK)/lib/build.mak
