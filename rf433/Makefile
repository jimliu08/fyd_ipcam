########################################################
# 
#   Makefile for smart ipcam application
#   Copyright (c) by liujm
#
########################################################

TARGET			:= rf433
SRC				:= $(wildcard *.c)
OBJECTS			:= $(SRC:.c=.o)
RM := rm -f
CROSS_COMPILE	:= arm-linux-
AS				:= $(CROSS_COMPILE)as
AR				:= $(CROSS_COMPILE)ar
CC				:= $(CROSS_COMPILE)gcc
CPP				:= $(CC) -E
LD				:= $(CROSS_COMPILE)ld
NM				:= $(CROSS_COMPILE)nm

CFLAGS			:= -Wall -O3 -DNO_DP -DNDEBUG -DLEAF_TTY_TEST
CINCS			:= -I. -I ../include
LINCS			:=
LDFLAGS			:=


LDFLAGS			+= -lpthread



all: $(TARGET)

$(TARGET): $(OBJECTS)
	@ echo "Generating $(TARGET)"
	@ $(CC) -o $@ $^ $(LINCS) $(LDFLAGS) $(CINCS) $(CFLAGS)
		
.PHONY: all clean

%.o:%.c
	@ echo "Compiling $*.c"
	@ $(CC) $(CFLAGS) $(CINCS) -c -o $@ $<

clean:
	$(RM) $(TARGET) $(OBJECTS) *.o

