.PHONY: obsidian
.PHONY: space-game
.PHONY: all
.PHONY: run
.PHONY: clean

all: obsidian space-game run
	clear

run:
	clear 
	./space-game.exe

obsidian:
	make -C obsidian/

space-game:
	make -C space-game/

clean:
	make -C obsidian/ clean
	make -C space-game/ clean