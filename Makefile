CC=g++
CFLAGS=-std=c++11 -g

all: main

main: main.o Graf.o Hardware.o Times.o COM.o Cost_List.o TimeAndCost.o
	$(CC) $(CFLAGS) main.o Graf.o Hardware.o Times.o COM.o Cost_List.o TimeAndCost.o -o main -lncurses

main.o: main.cpp
	$(CC) $(CFLAGS) -c main.cpp

Graf.o: Graf.cpp
	$(CC) $(CFLAGS) -c Graf.cpp

Costs.o: Cost_List.cpp
	$(CC) $(CFLAGS) -c Cost_List.cpp -lncurses

Hardware.o: Hardware.cpp
	$(CC) $(CFLAGS) -c Hardware.cpp

Times.o: Times.cpp
	$(CC) $(CFLAGS) -c Times.cpp

COM.o: COM.cpp
	$(CC) $(CFLAGS) -c COM.cpp

TimeAndCost.o: TimeAndCost.cpp
	$(CC) $(CFLAGS) -c TimeAndCost.cpp

run: main
	./main

clean:
	rm -rf *.o main *.Identifier *.dat

with_GDB:
	gdb main
