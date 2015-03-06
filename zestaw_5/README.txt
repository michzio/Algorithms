ZESTAW 5, Michal Ziobro
================================================================
Folder zawiera nastepujace pliki: 
./forward_list_test.cpp  
(zawiera kod generujacy testowy output użycia poszczególnych funkcji forward_list) 
./outputForwardList.txt
(wygenerowany wynik dzilania programu forward_list_test)
./linked_list.h 
./linked_list.cpp
(zawierają implementacje LinkedList<T> oraz generuja output dla LinkedList<T>
 output powinien być zgodny (poza funkcja element_at() z ./outputForwardList.txt)
./outputLinkedList.txt
(wygenerowany wynik dzialania programu linked_list.cpp)
./GenListUnion.cpp
(generuje input do sprawdzenia dzialania operacji union -zadanie 2)
./input.txt 
(plik wygenerowany programem GenListUnion.cpp)
./ListUnion.cpp
(przykladowy wzorcowy wynik operacji union na danych z input.txt)
./output.txt 
(wynik dzialania programu ListUnion.cpp)
./linked_list_union.h
./linked_list_union.cpp
(wlasna implementacja LinkedList<T> wraz z programem wykonujacym operacje union
 zastpaiona standardowa lista std::list<T> przez ADT::LinkedList<T>)
./output_linked_list.txt
(wynik dzialanaia program linked_list_union.cpp powinien być zgodny z output.txt
 generowanym przez program wzrocowy)
======================================================================
KOMPILACJA I URUCHAMIANIE

> g++ -stdlib=libc++ -std=c++11 -o forward_list_test forward_list_test.cpp
> g++ -stdlib=libc++ -std=c++11 -o linked_list linked_list.cpp

TESTOWANIE
> ./forward_list_test > outputForwardList.txt
> ./linked_list > outputLinkedList.txt
WYNIK POIWNIEN BYC ZGODNY POZA FUNKCJA element_at() której nie ma w forward_list

> g++ -o generator GenListUnion.cpp
> ./generator > input.txt 

> g++ -o list_union ListUnion.cpp
> ./list_union < input.txt > output.txt 

> g++ -stdlib=libc++ -std=c++11 -o linked_list_union linked_list_union.cpp
> ./linked_list_union < input.txt > output_linked_list.txt

WYNIK output.txt ORAZ output_linked_list.txt POWINNY BYC ZGODNE
======================================================================= 
UWAGI!
programy kompilowane byly kompilatorem clang gdyż g++ nie obsługiwał c++11
ex. 
clang++ -stdlib=libc++ -std=c++11 -o forward_list_test forward_list_test.cpp
być może w g++ -stdlib=libc++ nie jest wymagane
