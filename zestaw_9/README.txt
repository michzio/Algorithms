Program: 
dictionary.cpp - plik ze zrodlem + main 
dictionary.h - plik naglowkowy

Kompilacja: 
clang++ -stdlib=libc++ -std=c++11 -o dict dictionary.cpp
lub
g++  -std=c++11 -o dict dictionary.cpp

Generator: 
generator.cpp 

Kompilacja: 
clang++ -stdlib=libc++ -std=c++11 -o generator generator.cpp
lub 
g++ -std=c++11 -o generator generator.cpp

Uruchomienie 
./dict < input.txt (przyklad z zestawu 9) 
./generator > input2.txt
./dict < input2.text (opcjonalnie > output.txt) 

