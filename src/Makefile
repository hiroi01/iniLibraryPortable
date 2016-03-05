
ifeq ($(LITE), 1)
TARGET_LIB = libiniplite.a
else
TARGET_LIB = libinip.a
endif

OBJS = conf.o utils.o


INCDIR = ../include
CFLAGS = -O2 -G0 -Wall
CXXFLAGS = $(CFLAGS) -fno-exceptions -fno-rtti
ASFLAGS = $(CFLAGS)

ifeq ($(LITE), 1)
CFLAGS += -DILP_NOUSE_DEFAULT_BUTTON_INFO
endif

LIBDIR =
LDFLAGS =

PSPSDK=$(shell psp-config --pspsdk-path)
include $(PSPSDK)/lib/build.mak
