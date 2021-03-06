#  Makefile.inc
#  DarumaBasic
#  2016.2.7. Toshi Nagata
#  Copyright 2016 Toshi Nagata. All rights reserved.
#
#  This file is to be included from SOME_DIR/Makefile
#  It assumes that the current directory is SOME_DIR

C_SRCS = builtin.c gencode.c graph.c token.c edit.c execcode.c memory.c screen.c transmessage.c
C_SRCS += start.c string.c darumalogo.c y.tab.c

SUBDIR := $(shell basename $(PWD))
OBJS := $(patsubst %c,%o,$(patsubst %.cpp,%.o,$(SRCS)))
C_OBJS := $(patsubst %c,%o,$(patsubst %.cpp,%.o,$(C_SRCS)))
A_OBJS := $(patsubst %.o,build/$(SUBDIR)/%.o,$(OBJS)) $(patsubst %.o,build/common/%.o,$(C_OBJS))
# A_SRCS := $(patsubst %.c,../$(SUBDIR)/%.c,$(SRCS)) $(patsubst %.c,../common/%.c,$(C_SRCS))
# A_SRCS := $(patsubst $(SUBDIR)/../%.c,%.c,$(A_SRCS))
A_DEPS := $(patsubst %.o,%.d,$(A_OBJS))

PRODUCT_DIR = daruma_$(PLATFORM)

CC ?= gcc
DARUMA ?= $(PRODUCT_DIR)/daruma

# A_OBJS := $(patsubst ../%.c,build/%.o,$(A_SRCS)) 

INCLUDES += -I$(PWD) -I$(PWD)/../common
CFLAGS ?= -O2 -g

CFLAGS += $(ARCHFLAGS) $(BASEFLAGS) $(INCLUDES)
LDFLAGS += $(ARCHFLAGS) -lm

#ifeq ($(NO_MATHLIB),)
#LIBS += -lm
#endif

all: build/$(SUBDIR) ../common/y.tab.c build/common $(OTHER_DIRS) $(DARUMA)

-include $(DEPS)

build/$(SUBDIR):
	mkdir -p build/$(SUBDIR)

build/common:
	mkdir -p build/common

$(DARUMA): $(A_OBJS) $(SYS_OBJS)
	mkdir -p $(PRODUCT_DIR)
	$(CC) -o $@ $(A_OBJS) $(SYS_OBJS) $(LIBS) $(LDFLAGS)
	cp -rf ../basic ../common/fontdata.bin $(PRODUCT_DIR)/
	tar cvfj $(PRODUCT_DIR).tar.bz2 $(PRODUCT_DIR)

clean:
	rm -rf bin
	rm -rf build

../common/y.tab.c ../common/y.tab.h: ../common/grammar.y
	(cd ../common; bison -d -y grammar.y)

build/%.o: ../%.c Makefile
	$(CC) $(CFLAGS) -MMD -c $< -o $@

build/%.o: ../%.cpp Makefile
	$(CPP) $(CPPFLAGS) -MMD -c $< -o $@
