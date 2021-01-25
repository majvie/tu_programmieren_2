

all : example1 example2 example3

libduma.a : duma.o print.o sem_inc.o
	ar cr libduma.a duma.o print.o sem_inc.o


example1 : example1.cpp example1.cpp duma.c print.c sem_inc.c
	gcc -g -o example1 example1.cpp duma.c print.c sem_inc.c -lpthread

example2 : example2.cpp libduma.a
	gcc -g -o example2 -I. example2.cpp libduma.a -lpthread
example3 : example3.cpp libduma.a
	gcc -g -o example3 -I. example3.cpp libduma.a -lpthread
#example4 : example4.cpp libduma.a
#	gcc -g -o example4 -I. example4.cpp libduma.a -lpthread -std=c++03


