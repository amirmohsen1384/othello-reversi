CC=x86_64-w64-mingw32-g++
CFLAGS= -Wall -Wextra -std=c++17 -I"$(shell cygpath -m "$(CURDIR)/include")"
APP=reversi.exe

all=main

main: graphics.o menu.o point.o board.o player.o interfaces.o size.o
	$(CC) $(CFLAGS) -o $(APP) main.cpp *.o 

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

interfaces.o: src/core/interfaces.cpp
	$(CC) $(CFLAGS) -c src/core/interfaces.cpp -o interfaces.o

size.o: src/core/size.cpp
	$(CC) $(CFLAGS) -c src/core/size.cpp -o size.o

clean:
	@rm *.o
	@rm $(APP)