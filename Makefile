all: euchre.cpp data_structures.o ai.o util.o
	g++ euchre.cpp data_structures.o ai.o util.o -g --std=c++11 -o Euchre

data_structures.o: data_structures.cpp data_structures.h
	g++ -g -c data_structures.cpp

ai.o: ai.cpp ai.h util.h
	g++ -g -c ai.cpp

util.o: util.cpp util.h
	g++ -g -c util.cpp

clean:
	rm -f *.o
