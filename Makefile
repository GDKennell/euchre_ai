all: euchre.cpp data_structures.o
	g++ euchre.cpp data_structures.o -g --std=c++11 -o Euchre

data_structures.o: data_structures.cpp data_structures.h
	g++ -g -c data_structures.cpp

clean:
	rm -f *.o
