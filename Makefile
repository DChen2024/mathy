CC = gcc
CFLAGS = -std=c99 -I$(INC_DIR) -Wall -Wextra -DMATHY_EXPORTS

BUILD_DIR = build
INC_DIR = include
SRC_DIR = src

BUILD_TYPE ?= debug
ifeq ($(BUILD_TYPE), debug)
	CPPFLAGS = -g -O0
else ifeq ($(BUILD_TYPE), release)
	CPPFLAGS = -s -O2
else
	$(error Invalid BUILD_TYPE: must be either 'debug' or 'release')
endif
VERSION = v1.0.0

SOURCE = $(SRC_DIR)/mathy.c
TARGET = $(BUILD_DIR)/$(BUILD_TYPE)/$(VERSION)/mathy.dll

all: $(TARGET)

$(TARGET): $(SOURCE)
	$(CC) $(CFLAGS) $(CPPFLAGS) -shared -o $@ $<

clean:
	$(RM) $(TARGET)
