CC = gcc
CFLAGS = -std=c99 -I$(INC_DIR) -Wall -Wextra
CPPFLAGS = -s -O2
LDFLAGS = -L$(BUILD_DIR)
LDLIBS = -lmathy

BUILD_DIR = ../build/tests
INC_DIR = ../include

SOURCES = $(wildcard *.c)
TARGETS = $(patsubst %.c, $(BUILD_DIR)/%.exe, $(SOURCES))

all: $(TARGETS)

$(BUILD_DIR)/%.exe: %.c
	$(CC) $(CFLAGS) $(CPPFLAGS) -o $@ $< $(LDFLAGS) $(LDLIBS)

clean:
	$(RM) $(TARGETS)
