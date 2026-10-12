TARGET = PSPPlatformDemo
OBJS = src/main.o
CFLAGS = -O2 -G0 -Wall -Wextra
CXXFLAGS = $(CFLAGS)
ASFLAGS = $(CFLAGS)
LIBS = -lpspdebug -lpspdisplay -lpspge -lpspctrl -lpspkernel
EXTRA_TARGETS = EBOOT.PBP
PSP_EBOOT_TITLE = PSP Platform - Signal Collector
PSPSDK ?= $(shell psp-config --pspsdk-path)
include $(PSPSDK)/lib/build.mak
