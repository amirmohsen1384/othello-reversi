CC=x86_64-w64-mingw32-g++
CFLAGS= -Wall -Wextra -std=c++17 -I"$(shell cygpath -m "$(CURDIR)/include")"
APP=reversi.exe

all=main

main: graphics.o menu.o point.o board.o player.o general.o size.o match.o view_game.o view_core.o game.o
	$(CC) $(CFLAGS) -o $(APP) main.cpp *.o 

match.o: src/game/match.cpp
	$(CC) $(CFLAGS) -c src/game/match.cpp -o match.o

system.o: src/core/system.cpp
	$(CC) $(CFLAGS) -c src/core/system.cpp -o system.o

graphics.o: system.o src/core/graphics.cpp
	$(CC) $(CFLAGS) -c src/core/graphics.cpp -o graphics.o

point.o: src/core/point.cpp
	$(CC) $(CFLAGS) -c src/core/point.cpp -o point.o

menu.o: navigator.o src/menu/menu.cpp
	$(CC) $(CFLAGS) -c src/menu/menu.cpp -o menu.o

navigator.o: src/menu/navigator.cpp
	$(CC) $(CFLAGS) -c src/menu/navigator.cpp -o navigator.o

board.o: src/game/board.cpp
	$(CC) $(CFLAGS) -c src/game/board.cpp -o board.o

player.o: src/game/player.cpp
	$(CC) $(CFLAGS) -c src/game/player.cpp -o player.o

general.o: src/core/general.cpp
	$(CC) $(CFLAGS) -c src/core/general.cpp -o general.o

size.o: src/core/size.cpp
	$(CC) $(CFLAGS) -c src/core/size.cpp -o size.o

view_game.o: src/game/view.cpp
	$(CC) $(CFLAGS) -c src/game/view.cpp -o view_game.o

view_core.o: src/core/view.cpp
	$(CC) $(CFLAGS) -c src/core/view.cpp -o view_core.o
	
game.o: src/game/game.cpp
	$(CC) $(CFLAGS) -c src/game/game.cpp -o game.o
	
clean:
	@rm *.o