#include<cstdio>
#include<ctime>
#include<cstdlib>

const int MAX_WORDS = 100;
const int MAX_LETTERS = 15;
const int LENGTH_OF_ALFABET = 26;
const int MAX_FREQUENCY = 20;

char alfabet[LENGTH_OF_ALFABET] { 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j','k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z' };

int main(int argc, char **argv)
{
    //losuj liczbe całkowita
    srand( time(NULL) );
    int tc = rand()%MAX_WORDS;
    printf("%d\n", tc);
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
        
        printf("%s ", word);
    }
    
    printf("\n");
    for(int i=0; i<tc; i++)
    {
        int frequency = rand()%MAX_FREQUENCY + 1;
        printf("%d ", frequency);
    }
    
    printf("\n");


	return 0; 
} 
