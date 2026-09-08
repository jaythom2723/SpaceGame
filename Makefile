include make/config.mk
include make/common.mk

.PHONY: obsidian
.PHONY: obtexcvtr
.PHONY: space-game
.PHONY: all
.PHONY: run
.PHONY: clean

all: obsidian obtexcvtr space-game run

COLOR_GREEN=\033[0;32m
COLOR_RED=\033[0;31m
COLOR_BLUE=\033[0;34m
COLOR_END=\033[0m

run:
	cd build && valgrind --leak-check=full --log-file="../valgrind.log" ./space-game.exe

obsidian:
	@echo "$(COLOR_GREEN)"
	make -C $(OBSIDIAN_DIR) 2>&1 | tee build.log
	@echo "$(COLOR_END)"

obtexcvtr:
	@echo "$(COLOR_RED)"
	make -C $(OBTEXCVTR_DIR) 2>&1 | tee -a build.log
	@echo "$(COLOR_END)"

space-game:
	@echo "$(COLOR_BLUE)"
	make -C $(CELESTIAL_DIR) 2>&1 | tee -a build.log
	@echo "$(COLOR_END)"

clean:
	rm -f ./*.log
	rm -f ./**/*.log
	make -C $(OBSIDIAN_DIR) clean
	make -C $(OBTEXCVTR_DIR) clean
	make -C $(CELESTIAL_DIR) clean