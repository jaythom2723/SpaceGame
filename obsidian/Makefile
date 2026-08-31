CC=gcc
STD=-std=c99
CFLAGS=-Wall -Wextra -Werror $(STD)
LDFLAGS=-lglfw3 -lcglm -lGL -lm -lX11 -lpthread -ldl -lXrandr -lXi
PFLAGS=-Iinclude/
LPFLAGS=-I../lib/cglm/include/ -I../lib/glfw/include/ -I../lib/glad/include/ -I"../lib/stb-image/include/"
PFLAGS:=$(PFLAGS) $(LPFLAGS)
SRCDIRS=src/
SOURCES=$(wildcard $(SRCDIRS)*.c) $(wildcard $(SRCDIRS)**/*.c)
OUTPUT_DIR=../lib/obsidian
HEADERS?=$(wildcard include/*.h) $(wildcard include/**/*.h)

.PHONY: all clean
all: libobsidian.a headers

clean:
	rm -f $(OUTPUT_DIR)/libobsidian.a
	rm -f $(SOURCES:.c=.o)
	rm -f $(wildcard $(OUTPUT_DIR)/include/obsidian/*.h) $(wildcard $(OUTPUT_DIR)/include/obsidian/**/*.h)

libobsidian.a: $(SOURCES:.c=.o)
	ar rcs $(OUTPUT_DIR)/$@ $^

headers: $(HEADERS)
	cp -r include $(OUTPUT_DIR)

.c.o:
	$(CC) $(CFLAGS) -c $< $(PFLAGS) -o $@