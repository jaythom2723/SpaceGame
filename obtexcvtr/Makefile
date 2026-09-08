CC=gcc
STD=-std=c99
CFLAGS=-Wall -Wextra -Werror $(STD)
LDFLAGS=-lm -lX11 -lpthread -ldl -lXrandr -lXi
PFLAGS=-Iinclude/
SRCDIRS=src/
SOURCES=$(wildcard $(SRCDIRS)*.c) $(wildcard $(SRCDIRS)**/*.c)
OUTPUT_DIR=../build

.PHONY: all clean
all: obtexcvtr.exe

clean:
	rm -f $(OUTPUT_DIR)/obtexcvtr.exe

obtexcvtr.exe: $(SOURCES)
	$(CC) $(CFLAGS) $(PFLAGS) $< $(LDFLAGS) -o $(OUTPUT_DIR)/$@