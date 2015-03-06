#include<cstdio>
#include<ctime>
#include<cstdlib>

const int MAX_WORDS = 120;
const int MAX_LETTERS = 20;
const int LENGTH_OF_ALFABET = 26;
const int MAX_SETS = 20;
const int MAX_OPERATIONS = 100;

char alfabet[LENGTH_OF_ALFABET] { 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j','k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z' };

int main(int argc, char **argv)
{
    //losuj liczbe całkowita
    srand( time(NULL) );
    int tc = rand()%MAX_WORDS + 1;
    printf("%d\n", tc);
    
    char **dictionary = new char *[tc];
    
    for(int i=0; i< tc; i++)
    {
        int noOfLetters = rand()%MAX_LETTERS + 1;
        
        char *word = new char[noOfLetters+1];
        
        for(int j=0; j< noOfLetters; j++)
        {
            int letterNo = rand()%LENGTH_OF_ALFABET;
            word[j] = alfabet[j];
        }
        word[noOfLetters] = '\0';
        
        dictionary[i] = word;
        
        printf("%s ", word);
    }
    printf("\n");
    
    //losowanie liczby zbiorów
    int setCount = rand()%MAX_SETS + 1;
    printf("%d\n", setCount);
    
    for(int i=0; i< setCount; i++)
    {
        int noOfElementsInSet = rand()%tc +1;
        printf("%d ", noOfElementsInSet);
        
        for(int j=0; j< noOfElementsInSet; j++) {
            int randomWordIdx = rand()%tc;
            
            printf("%s ", dictionary[randomWordIdx]);
        }
        printf("\n");
    }
    
    //liczba operacji
    int noOfOperations = rand()%MAX_OPERATIONS + 1;
    printf("%d\n", noOfOperations);
    
    for(int i=0; i<noOfOperations; i++)
    {
        int operType = rand()%2;
        if(operType == 0) {
            printf("E ");
            
            int operType2 = rand()%3;
            int randA = rand()%setCount;
            int key = rand()%tc;
            if(operType2 == 0)
            {
                //dodaj
                printf("D ");
                printf("%d %d", randA, key);
                
            } else if(operType2 == 1)
            {
                //usun
                printf("U ");
                printf("%d %d", randA, key);
                
            } else if(operType2 == 2)
            {
                //sprawdz
                printf("S ");
                printf("%d %d", randA, key);
                
            }
            
        } else if(operType == 1) {
            printf("Z ");
            
            int operType2 = rand()%5;
            
            int randA = rand()%setCount;
            int randB = rand()%setCount;
            int randC = rand()%setCount;
            
            if(operType2 == 0)
            {
                //suma
                printf("S ");
               
                 printf("%d %d %d", randA, randB, randC);
                
            } else if(operType2 == 1)
            {
                //roznica
                printf("R ");
                printf("%d %d %d", randA, randB, randC);
                
            } else if(operType2 == 2)
            {
                //przeciecie
                printf("P ");
                printf("%d %d %d", randA, randB, randC);
                
            } else if(operType2 == 3)
            {
                //dopelnienie
                printf("D ");
                printf("%d %d", randA, randB);
            } else if(operType2 == 4)
            {
                //wypisz
                printf("W ");
                printf("%d", randA);
            }
        }
        
        printf("\n");
    }
    
    printf("\n");


	return 0; 
} 
