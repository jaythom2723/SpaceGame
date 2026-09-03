CC=gcc
STD=-std=c99
CFLAGS=-Wall -Wextra -Werror $(STD)
LDFLAGS=-lX11 -lpthread -ldl -lXrandr -lXi -L../lib/obsidian -lobsidian -lm -lglfw3 -lcglm -lGL
PFLAGS=-Iinclude/
LPFLAGS=-I../lib/cglm/include/ -I../lib/glfw/include/ -I../lib/glad/include/ -I"../lib/stb-image/include/" -I../lib/obsidian/include/
PFLAGS:=$(PFLAGS) $(LPFLAGS)
SRCDIRS=src
SOURCES=$(wildcard $(SRCDIRS)/*.c) $(wildcard $(SRCDIRS)/**/*.c)
OUTPUT_DIR=../build

.PHONY: all clean

all: space-game.exe

space-game.exe: $(SOURCES)
	$(CC) $(CFLAGS) $^ $(PFLAGS) $(LDFLAGS) -o $(OUTPUT_DIR)/$@

clean:
	rm -f $(OUTPUT_DIR)/space-game.exe