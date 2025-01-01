CC = gcc
CFLAGS = -std=c99 -Wall -Wextra -O2 -s
CPPFLAGS = -I$(INC_DIR)
LDFLAGS = -L$(OUT_DIR)
LDLIBS = -lmathy

INC_DIR = ../mathy
OUT_DIR = ../build

SOURCES = $(wildcard *.c)
TARGETS = $(SOURCES:%.c=$(OUT_DIR)/%.exe)

all: $(TARGETS)

$(OUT_DIR)/%.exe: %.c
	$(CC) $(CFLAGS) $(CPPFLAGS) -o $@ $< $(LDFLAGS) $(LDLIBS)

clean:
	$(RM) $(TARGETS)
