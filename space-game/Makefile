CC=gcc
STD=-std=c99
CFLAGS=-Wall -Wextra -Werror $(STD)
LDFLAGS=-lglfw3 -lcglm -lGL -lX11 -lpthread -ldl -lXrandr -lXi 
PFLAGS=-Iinclude/
LPFLAGS=-I../lib/cglm/include/ -I../lib/glfw/include/ -I../lib/glad/include/ -I"../lib/stb-image/include/" -I../lib/obsidian/include/
PFLAGS:=$(PFLAGS) $(LPFLAGS)
SRCDIRS=src
SOURCES=$(wildcard $(SRCDIRS)/*.c) $(wildcard $(SRCDIRS)/**/*.c)
OUTPUT_DIR=..

.PHONY: all clean

all: space-game.exe

space-game.exe: $(SOURCES)
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -L../lib/obsidian -lobsidian -lm $(PFLAGS) -o $(OUTPUT_DIR)/$@

clean:
	rm -f $(OUTPUT_DIR)/space-game.exe