all: euchre.cpp data_structures.o ai.o util.o deck.o
	g++ euchre.cpp data_structures.o ai.o util.o deck.o -g --std=c++11 -o Euchre

trump_test: trump_call_test.cpp data_structures.o ai.o util.o deck.o
	g++ trump_call_test.cpp data_structures.o ai.o util.o deck.o -g --std=c++11 -o TrumpTest

data_structures.o: data_structures.cpp data_structures.h
	g++ -g -c data_structures.cpp

ai.o: ai.cpp ai.h util.h
	g++ -g -c ai.cpp

util.o: util.cpp util.h
	g++ -g -c util.cpp

deck.o: Deck.cpp Deck.h data_structures.h
	g++ -g -c Deck.cpp

clean:
	rm -f *.o
