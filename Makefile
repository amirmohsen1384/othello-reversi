CC=x86_64-w64-mingw32-g++
CFLAGS= -Wall -Wextra -std=c++17 -I"$(shell cygpath -m "$(CURDIR)")"
APP=reversi.exe

all=main

main: game.o board.o player.o match.o scoreboard.o
	$(CC) $(CFLAGS) -o $(APP) main.cpp *.o 

match.o: src/game/match.cpp menu.o graphics.o
	$(CC) $(CFLAGS) -c src/game/match.cpp -o match.o

system.o: src/core/system.cpp
	$(CC) $(CFLAGS) -c src/core/system.cpp -o system.o

graphics.o: system.o src/core/graphics.cpp
	$(CC) $(CFLAGS) -c src/core/graphics.cpp -o graphics.o

point.o: src/core/point.cpp view_core.o
	$(CC) $(CFLAGS) -c src/core/point.cpp -o point.o

menu.o: navigator.o src/menu/menu.cpp graphics.o
	$(CC) $(CFLAGS) -c src/menu/menu.cpp -o menu.o

navigator.o: src/menu/navigator.cpp
	$(CC) $(CFLAGS) -c src/menu/navigator.cpp -o navigator.o

board.o: src/game/board.cpp size.o point.o graphics.o
	$(CC) $(CFLAGS) -c src/game/board.cpp -o board.o

player.o: src/game/player.cpp general.o
	$(CC) $(CFLAGS) -c src/game/player.cpp -o player.o

general.o: src/core/general.cpp
	$(CC) $(CFLAGS) -c src/core/general.cpp -o general.o

size.o: src/core/size.cpp
	$(CC) $(CFLAGS) -c src/core/size.cpp -o size.o

view_game.o: src/game/view.cpp menu.o graphics.o
	$(CC) $(CFLAGS) -c src/game/view.cpp -o view_game.o

view_core.o: src/core/view.cpp graphics.o
	$(CC) $(CFLAGS) -c src/core/view.cpp -o view_core.o
	
game.o: src/game/game.cpp view_game.o exceptions.o menu.o graphics.o
	$(CC) $(CFLAGS) -c src/game/game.cpp -o game.o
	
exceptions.o: src/core/exceptions.cpp
	$(CC) $(CFLAGS) -c src/core/exceptions.cpp -o exceptions.o

scoreboard.o: src/scoreboard/scoreboard.cpp ranked.o
	$(CC) $(CFLAGS) -c src/scoreboard/scoreboard.cpp -o scoreboard.o

ranked.o: src/scoreboard/player.cpp
	$(CC) $(CFLAGS) -c src/scoreboard/player.cpp -o ranked.o

clean:
	@rm *.o