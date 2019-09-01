CC = clang++
mkfile_path :=$(shell pwd)
INCLUDES=-I/usr/include -I/usr/local/include -I./json11 -I/Users/qingyun/Downloads/git/rapidjson/include/ -I$(mkfile_path)
CFLAGS = -c -std=c++11 -Wall -fno-rtti -fno-exceptions
DFLAGS = -std=c++11 -Wall -g -pthread
UNIT_TEST1 = -L/usr/local/lib -lUnitTest++ test/main.cpp

LIBS = 

SRCS = *.cpp

objects = $(sources:.cpp=.o)

default: all

main:
	$(CC) $(DFLAGS) $(INCLUDES) main.cpp 

json11.o: 
	$(CC) $(CFLAGS) json11/json11.cpp

driver.o: 
	$(CC) $(CFLAGS) driver.hpp

task_timer.o:
	$(CC) $(CFLAGS) task_timer.hpp

task_type.o:
	$(CC) $(CFLAGS) task_type.cpp

task.o:
	$(CC) $(CFLAGS) task.cpp

buffer.o:
	$(CC) $(CFLAGS) buffer.cpp

cache_buffer.o: 
	$(CC) $(CFLAGS) cache_buffer.cpp

process.o: json11.o task_type.o task.o buffer.o cache_buffer.o
	$(CC) $(CFLAGS) process.cpp 

dispatcher.o:
	$(CC) $(CFLAGE) dispatcher.hpp

client: json11.o
	$(CC) $(DFLAGS) $(INCLUDES) client.cpp json11.o -o client

client2.o:
	$(CC) $(CFLAGS) client2.cpp

client2_app: client2.o
	$(CC) $(DFLAGS) $(INCLUDES) client2_app.cpp client2.o -o client2

server: 
	$(CC) $(DFLAGS) $(INCLUDES) server.cpp json11.o process.o -o server

server2.o: process.o
	$(CC) $(CFLAGS) $(INCLUDES) server2.cpp

server2_app: server2.o
	$(CC) $(DFLAGS) $(INCLUDES) server2_app.cpp server2.o json11.o process.o task.o task_type.o buffer.o cache_buffer.o -o server2

test1: server2.o client2.o
	$(CC) $(DFLAGS) $(UNIT_TEST1) $(INCLUDES) test/test1.cpp server2.o client2.o json11.o process.o task.o task_type.o buffer.o cache_buffer.o -o test1; ./test1

geom:
	$(CC) $(DFLAGS) $(INCLUDES) test/test_geometry.cpp

all: server2_app client2_app

test: all test1 

clean_test:
	rm test1;

retest: clean_test test1
	./test1

clean:
	rm *.o server2 client2 test1

kill:
	pkill server2 client2 test1

