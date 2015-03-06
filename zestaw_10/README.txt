Program: 
huffman_tree.cpp - plik z kodem zrodlowy + funkcja main wczytujaca dane inputowe
huffman_tree.h - plik naglowkowy 

Kompilacja: 
clang++ -stdlib=libc++ -std=c++11 -o huffman huffman_tree.cpp
lub
g++ -std=c++11 -o huffman huffman_tree.cpp

Generator: 
generator.cpp - generuje liczbe int N = slow/czestosci wystapien, nastepnie N slow oddzielonych spacjami i N czestosci wystapien oddzielonych spacjami

Kompilacja:
clang++ -stdlib=libc++ -std=c++11 -o generator generator.cpp
lub 
g++ -std=c++11 -o generator generator.cpp

Uruchomienie: 

./generator > input.txt
./huffman < input.txt  (opcjonalnie > output.txt) 

dodatkowo 
./huffman < input2.txt + zdjecie drzewa huffmana odpowiadajacego 
