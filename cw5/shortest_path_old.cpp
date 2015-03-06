#include <iostream>
#include <fstream>
#include <string>
#include <cstdio>
#include <cstring>
using namespace std;


int main(int argc, char **)
{
    string line;
    ifstream myfile;
    
    int size = 0;
    char *ch;
    int number;
    char *line_c;
    
    
    ch = new char[10];
    myfile.open("graph.txt", ifstream::in);

    if(myfile.is_open()) {
        
        getline(myfile, line);
        size = atoi(line.c_str());
        
        cout << size <<endl;
        int **matrix = new int*[size];
        for(int i = 0; i< size; ++i)
            matrix[i] = new int[size];
        
        int i = 0;
        
        while(getline(myfile, line) )
        {
            line_c = new char[line.length()];
            line_c = (char*) line.c_str();
            ch = strtok( line_c, " ");
            number = atoi(ch)-1;
            matrix[i][number] = 1;
            
            while(ch != NULL) {
                ch = strtok(NULL, " ");
                number = atoi(ch) -1;
                matrix[i][number] = 1;
            }
            
            i++;
        }
       
        cout << size << endl;
        
    }

    myfile.close();
   
    return 0;
}



//zadanie domowe
//algorytm najkrotszej sciezki
//rekurencyjny
//dowolna reprezentacja grafu
//lista sasiedztwa
//lub tablica
//wagi beda podawane w takiej samej strukturze jak lista krawedzi
//druga lista ktora jest lista wag
//.... nie wiecej niz 20 wierzchołków