Program: 
binary_search_tree.cpp - kod zrodlowy + main generujacy output testowy
binary_search_tree.h - plik naglowkowy

Kompilacja 
clang++ -stdlib=libc++ -std=c++11 -o bst binary_search_tree.cpp
lub 
g++ -std=c++11 -o bst binary_search_tree.cpp

Generator: 
liczba wartosci (string)  N,  N stringow , N kluczy

generator.cpp

Kompilacja
clang++ -stdlib=libc++ -std=c++11 -o generator generator.cpp
lub
g++  -std=c++11 -o generator generator.cpp

Uruchomienie: 
./generator > input.txt
./bst < input.txt (opcjonalnie  > output.txt)
