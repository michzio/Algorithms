Zawartosc folderu: 

Lista podwojnie wiazana cykliczna DoublyLinkedList 
>doubly_linked_list.h
>doubly_linked_list.cpp

Program testowy: do porwnania z wzrocowym autputem std::list<int>
>test.cpp

Pliki tekstowe z outputem:
>output_test.txt
>output_doubly_linked.txt

KOMPILACJA:
> clang++ -stdlib=libc++ -std=c++11 -o test test.cpp
 lub
> g++ -stdlib=libc++ -std=c++11 -o test test.cpp

> clang++ -stdlib=libc++ -std=c++11 -o doubly_linked_list doubly_linked_list.cpp
 lub
> g++ -stdlib=libc++ -std=c++11 -o doubly_linked_list doubly_linked_list.cpp

URUCHOMIENIE
> ./test > output_test.txt
> ./doubly_linked_list > output_doubly_linked.txt


