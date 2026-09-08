include ../make/config.mk
include ../make/common.mk
include ../make/dependencies.mk

PFLAGS=-I"$(OBSIDIAN_INC_DIR)" -I"$(LIB_CGLM_INC_DIR)" -I"$(LIB_GLFW_INC_DIR)" -I"$(LIB_GLAD_INC_DIR)" -I"$(LIB_STBIMAGE_INC_DIR)"
SOURCES=$(wildcard $(OBSIDIAN_SRC_DIR)/*.c) $(wildcard $(OBSIDIAN_SRC_DIR)/**/*.c)
OUTPUT_DIR=$(LIB_OBSIDIAN_DIR)
HEADERS?=$(wildcard $(LIB_OBSIDIAN_INC_DIR)/*.h) $(wildcard $(LIB_OBSIDIAN_INC_DIR)/**/*.h)

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
	$(CC) $(CFLAGS) $(PFLAGS) $(DEPEN_X11) -c $< $(DEPEN_OPENGL) $(DEPEN_MATH)  -o $@