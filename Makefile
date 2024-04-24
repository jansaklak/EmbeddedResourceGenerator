CC=g++
CFLAGS=-std=c++17 -g

all: main

main: main.o Graf.o Hardware.o Times.o COM.o Cost_List.o Edge.o Cost_List_scheduler.o Cost_List_normalizing.o
	$(CC) $(CFLAGS) main.o Graf.o Hardware.o Edge.o Times.o COM.o Cost_List.o Cost_List_scheduler.o Cost_List_normalizing.o -o main

main.o: main.cpp
	$(CC) $(CFLAGS) -c main.cpp

Graf.o: Graf.cpp
	$(CC) $(CFLAGS) -c Graf.cpp

Costs.o: Cost_List.cpp
	$(CC) $(CFLAGS) -c Cost_List.cpp

Hardware.o: Hardware.cpp
	$(CC) $(CFLAGS) -c Hardware.cpp

Times.o: Times.cpp
	$(CC) $(CFLAGS) -c Times.cpp

COM.o: COM.cpp
	$(CC) $(CFLAGS) -c COM.cpp

Edge.o: Edge.cpp
	$(CC) $(CFLAGS) -c Edge.cpp

Cost_List_normalizing.o: Cost_List_normalizing.cpp
	$(CC) $(CFLAGS) -c Cost_List_normalizing.cpp


Cost_List_scheduler.o: Cost_List_scheduler.cpp
	$(CC) $(CFLAGS) -c Cost_List_scheduler.cpp


run: main
	./main

clean:
	rm -rf *.o main *.Identifier data/*.temp

with_GDB:
	gdb main
