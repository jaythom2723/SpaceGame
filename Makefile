.PHONY: obsidian
.PHONY: obtexcvtr
.PHONY: obmdlcvtr
.PHONY: space-game
.PHONY: all
.PHONY: run
.PHONY: clean

all: obsidian obtexcvtr obmdlcvtr space-game run

COLOR_GREEN=\033[0;32m
COLOR_RED=\033[0;31m
COLOR_BLUE=\033[0;34m
COLOR_YELLOW=\033[0;33m
COLOR_END=\033[0m

run:
	cd build && ./space-game.exe

obsidian:
	@echo "$(COLOR_GREEN)"
	make -C obsidian/
	@echo "$(COLOR_END)"

obtexcvtr:
	@echo "$(COLOR_RED)"
	make -C obtexcvtr/
	@echo "$(COLOR_END)"

obmdlcvtr:
	@echo "$(COLOR_YELLOW)"
	make -C obmdlcvtr/
	@echo "$(COLOR_END)"

space-game:
	@echo "$(COLOR_BLUE)"
	make -C space-game/
	@echo "$(COLOR_END)"

clean:
	make -C obsidian/ clean
	make -C obtexcvtr/ clean
	make -C space-game/ clean