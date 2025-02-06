CC=x86_64-w64-mingw32-g++
CFLAGS=-std=c++17
APP=reversi.exe

all=main

main: graphics.o menu.o point.o board.o
	$(CC) $(CFLAGS) -o $(APP) main.cpp *.o 

system.o: src/system.cpp
	$(CC) $(CFLAGS) -c src/system.cpp -o system.o

graphics.o: system.o src/graphics.cpp
	$(CC) $(CFLAGS) -c src/graphics.cpp -o graphics.o

point.o: src/point.cpp
	$(CC) $(CFLAGS) -c src/point.cpp -o point.o

menu.o: navigator.o src/menu.cpp
	$(CC) $(CFLAGS) -c src/menu.cpp -o menu.o

navigator.o: src/navigator.cpp
	$(CC) $(CFLAGS) -c src/navigator.cpp -o navigator.o

board.o: src/board.cpp
	$(CC) $(CFLAGS) -c src/board.cpp -o board.o

clean:
	@rm *.o
	@rm $(APP)