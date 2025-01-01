CC = gcc
CFLAGS = -std=c99 -Wall -Wextra -O2 -s
CPPFLAGS = -DMATHY_EXPORTS
LDFLAGS = -shared

SRC_DIR = mathy
OBJ_DIR = build

SRC = $(wildcard $(SRC_DIR)/*.c)
OBJ = $(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.obj)

TARGET = build/mathy.dll

all: $(TARGET)

$(OBJ_DIR)/%.obj: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) $(CPPFLAGS) -o $@ -c $<

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	$(RM) $(OBJ) $(TARGET)
