CPPFLAGS += -pedantic-errors -std=c++11

all: main

main: objectFiles/main.o objectFiles/game.o objectFiles/io.o objectFiles/menu.o
	g++ -o $@ $^ -pthread

objectFiles/main.o: main.cpp game/game.h
	g++ ${CPPFLAGS} -c $< -o $@

objectFiles/game.o: game/game.cpp
	g++ ${CPPFLAGS} -c $^ -o $@

objectFiles/io.o: io/io.cpp
	g++ ${CPPFLAGS} -c $^ -o $@

objectFiles/menu.o: menu/menu.cpp
	g++ ${CPPFLAGS} -c $^ -o $@

clean:
	rm objectFiles/*
	rm main
	
PHONY: clean