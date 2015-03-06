Folder zawiera 2 programy: 
1) array_list 
  PLIKI: array_list.cpp array_list.h
  Kompilacja: 
  > g++ -o array_list array_list.cpp 
  URUCHOMIENIE:
  > ./array_list < input.dat

2) linked_list
   PLIKI: linked_list.cpp linked_list.h
   Kompilacja: 
   > g++ -o linked_list linked_list.cpp
   URUCHOMIENIE:
   > ./linked_list <input.dat
3) generowanie input.dat
   > g++ -o generator gen.cpp
   > ./generator > input.dat
4) generowanie output.dat
   > g++ -o solution solution.cpp
   > ./solution < input.dat > output.dat
5) w folderze jest rowniez program do testowania test.cpp
   UWAGA! aby go uruchomic nalezy zakomentowac w array_list.cpp i linked_list.cpp funkcje main()!!! 
   > make all  
   > make clean 
   //instrukcje kompilacji zdefiniowane w pliku makefile 
   Niestety u mnie nie udalo sie poprawnie zlinkowac plikow array_list.o linked_list.o z programem test 
