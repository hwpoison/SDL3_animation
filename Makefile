CC = gcc
CFLAGS = -Wall -Iinclude -ISDL3 -g
LDFLAGS = -Lbin -lSDL3 -lSDL3_image

TARGET = bin/main.exe
BUILD_DIR = build
SRC_DIR = src

SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SRCS))

$(TARGET): $(OBJS)
	$(CC) $(OBJS) $(LDFLAGS) -o $(TARGET)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	del /Q $(subst /,\,$(OBJS)) $(subst /,\,$(TARGET))
