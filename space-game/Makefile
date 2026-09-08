include ../make/config.mk
include ../make/common.mk
include ../make/dependencies.mk

PFLAGS=-I"$(CELESTIAL_INC_DIR)" -I"$(LIB_CGLM_INC_DIR)" -I"$(LIB_GLFW_INC_DIR)" -I"$(LIB_GLAD_INC_DIR)" -I"$(LIB_STBIMAGE_INC_DIR)" -I"$(LIB_OBSIDIAN_INC_DIR)"
SOURCES=$(wildcard $(CELESTIAL_SRC_DIR)/*.c) $(wildcard $(CELESTIAL_SRC_DIR)/**/*.c)

.PHONY: all clean

all: space-game.exe

space-game.exe: $(SOURCES)
	$(CC) $(CFLAGS) $(PFLAGS) $(DEPEN_X11) $^ $(DEPEN_OBSIDIAN) $(DEPEN_OPENGL) $(DEPEN_MATH) -o $(BUILD_DIR)/$@

clean:
	rm -f $(BUILD_DIR)/space-game.exe