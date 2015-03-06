#include<cstdlib>
#include<cstdio>
#include<ctime>


//defining size of series of numbers used to do "searching leader of sequence"
#define N 50
//maximal possible value in series array
#define MAX_VALUE 1000

//function prototypes:
int randomSeriesOfNumbersWithLeader(int *, int);
void shuffle(int *, int);
void printArray(int *, int);
int searchLeaderOfSeries(int *, int);

//program entry point
//compilation: gcc -o przywodca przywodca.cpp
//{ ./przywodca }
int main(int argc, char *argv[]) {

    //N-element series of numbers with leader of series
    int series[N];
    //leader of seried of numbers
    int leaderNumber;
    
    int flag = randomSeriesOfNumbersWithLeader(series, N);
    if(flag == -1) {
        printf("An error has occured while generating random series of numbers.\n");
        exit(0);
    }
    
    //searching leader of series of numbers
    leaderNumber = searchLeaderOfSeries(series, N);
    printf("Przywodcą ciągu jest liczba: %d\n", leaderNumber);
    
    return 0;
}

/********************************************************************
 * leaderOfSeries() - is implementation of algorithm for searching  *
 * leader of series of numbers                                      *
 ********************************************************************/
int searchLeaderOfSeries(int *series, int length)
{
    int counter = 0;
    int totalCounter = 0;
    int leaderIdx;
    
    for(int i=0; i<length; i++) {
        
        if(counter == 0) {
            counter++;
            leaderIdx = i;
        } else if(series[i] == series[leaderIdx]) {
            counter++;
        } else {
            counter--;
        }
    }
    
    return series[leaderIdx];
}

/********************************************************************
 * randomSeriesOfNumbersWithLeader() - is function which randomly   *
 * generate series of numbers and place them into array series      *
 * this series will have size equal to 'length' argument and also   *
 * contain at least 'length/2 + 1' amount of some number (leader)   *
 * Params:                                                          *
 * @series - array of numbers with leader                           *
 * @length - size of array of numbers                               *
 * Return value:                                                    *
 * @error_code - integer flag indicating whether series has been    *
 *                successfully generated                            *
 ********************************************************************/
int randomSeriesOfNumbersWithLeader(int *series, int length) {
    
    srand(time(NULL));
    //generating random number which will be leader in sereis
    int leaderNumber = (rand() % MAX_VALUE);
    
    //generating leader count as integer number between 'length/2 + 1' to 'length'
    int minimalLeaderCount = length/2 + 1;
    int leaderCount = rand() %(length - minimalLeaderCount) + minimalLeaderCount;
    printf("Leader number count: %d", leaderCount);
    
    //filling first 'leaderCount' elements with previously generated leader number
    for(int i=0; i < leaderCount; i++) {
        series[i] = leaderNumber;
    }
    
    //filling remaining elements of series array with random numbers
    for(int i = leaderCount; i< length; i++) {
        series[i] = (rand() % MAX_VALUE);
    }
    
    //shuffle elements in series array
    shuffle(series, length);
    
    //printing series array for testing purposes
    printArray(series, length);
    
}

/**********************************************************************
 * shuffle() is function which shuffles elements in array using       *
 * very simple algorithm: passing through each element and choosing   *
 * random index and swapping values under current and random index.   *
 * Params:                                                            *
 * @array - array which elements will be shuffled                     *
 * @length - size of array                                            *
 **********************************************************************/
void shuffle(int *array, int length) {
    
    for (int i = 0; i < length - 1; i++)
    {
        int j = rand() % length;
        int temp = array[j];
        array[j] = array[i];
        array[i] = temp;
    }
    
}

/*********************************************************************
 * printArray() function just prints each element of the array       *
 * Params:                                                           *
 * @array - array which elements will be printed to the screen       *
 * @length - size of printed array                                   *
 *********************************************************************/
void printArray(int *array, int length) {
    printf("\n");
    for(int i = 0; i < length; i++) {
        printf("%d ", array[i]);
    }
    printf("\n\n");
}