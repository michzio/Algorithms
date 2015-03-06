all: test

test: linked_list.o array_list.o test.o
	g++ -o test linked_list.o array_list.o test.o  -Wall -g

test.o: test.cpp
	g++ -c test.cpp -o test.o -Wall -g

linked_list.o: linked_list.cpp linked_list.h
	g++ -c linked_list.cpp -o linked_list.o -Wall -g

array_list.o: array_list.cpp array_list.h
	g++ -c array_list.cpp -o array_list.o -Wall -g

clean: FRC
	rm -f linked_list.o array_list.o test test.o
FRC: