CC=gcc
CPPFLAGS=-I../ressources/
CFLAGS=-Wall -Werror -Wshadow -std=c99

LD=gcc
LDFLAGS=
LIBS_NAME=pe elf

DEBUG ?= 0

ifeq ($(DEBUG), 0)
	CFLAGS += -ggdb -O0 -DDEBUG=$(DEBUG)
	RELEASE_TYPE=debug
	CARGO_FLAGS=
else
	CFLAGS += -O2
	RELEASE_TYPE=release
	CARGO_FLAGS=--release
endif
LIBS=$(foreach libname,$(LIBS_NAME),../../$(libname)/target/$(RELEASE_TYPE)/lib$(libname).a)

SRC=c_api.c
OBJ=$(SRC:%.c=%.o)
BIN=exe_parse


.PHONY : all clean

.SECONDARY : $(LIBS)


$(BIN) : $(OBJ) $(LIBS)
	$(LD) $(LDFLAGS) -o $@ $^ -lpthread -ldl

%.o : %.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -o $@ -c $<

%.a :
	@echo Please build libs first
	false

