include ../make/config.mk
include ../make/common.mk
include ../make/dependencies.mk

PFLAGS=-I"$(OBTEXCVTR_INC_DIR)" -I"$(LIB_STBIMAGE_INC_DIR)"
SOURCES=$(wildcard $(OBTEXCVTR_SRC_DIR)/*.c) $(wildcard $(OBTEXCVTR_SRC_DIR)/**/*.c)

.PHONY: all clean
all: obtexcvtr.exe

clean:
	rm -f $(BUILD_DIR)/obtexcvtr.exe

obtexcvtr.exe: $(SOURCES)
	$(CC) $(CFLAGS) $(PFLAGS) $(DEPEN_X11) $< $(DEPEN_MATH) -o $(BUILD_DIR)/$@