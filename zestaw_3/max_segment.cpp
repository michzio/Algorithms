#include<cstdlib>
#include<cstdio>
#include<ctime>


//defining size of array of numbers used in algorithm "searching maximal segment"
#define N 10
//maximal possible value in numbers array
#define MAX_VALUE 1000

//function prototypes:
void fillWithRandomNumbers(int *, int);
void printArray(int *, int);
int searchMaxSegment(int *, int);
int max(int, int);

//program entry point
int main(int argc, char *argv[]) {
    
    int table[N];
    int max_segment = 0;
    
    printf("\n############## PROGRAM: MAKSYMALNY SEGMENT ###############\n\n");
    
    //filling table with random numbers
    fillWithRandomNumbers(table, N);
    
    //printing generated random array
    printf("Program wygenerował następującą tablice liczb:\n");
    printArray(table, N);
    
    printf("Szukanie maksymalnego segmentu...\n");
    
    //searching maximal segment value
    max_segment = searchMaxSegment(table, N);
    
    printf("Maksymalny znaleziony segment ma wartość: %d\n", max_segment);
    
    return 0;
}

/********************************************************************
 * searchMaxSegment() - this function is searching maximal segment  *
 * for table A[1..n] we count maximal value from zero and from each *
 * numbers SUM(k=i, j, A[k]) where 1<= i <= j <= n                  *
 * Params:                                                          *
 * @table - pointer to array of integers                            *
 * @length - integer value, size of table array                     *
 ********************************************************************/
int searchMaxSegment(int *table, int length) {
   
    int maxSegmentValue = 0, suffix = 0;
    
    for(int i=0; i<length; i++) {
        suffix = max(table[i]+suffix, 0);
        maxSegmentValue = max(maxSegmentValue, suffix);
    }
    
    return maxSegmentValue;
}

/*********************************************************************
 * max() function returns maximal integer value from both arguments  *
 * Params: @a - first integer, @b - second integer                   *
 * Result value: @max - integer indicating maximal value max(a, b)   *
 *********************************************************************/
int max(int a, int b) {
    if(a > b)
        return a;
    return b;
}


/*********************************************************************
 * fillWithRandomNumbers() - fills all elements of array  passed to  *
 * this function as first argument with randomly genereted numbers   *
 * Params:                                                           *
 * NUMBERS - POSITIVE OR NEGATIVE! rand() % 2*MAX_VALUE - MAX_VALUE  *
 * @array - pointer to array of integer values                       *
 * @length - size of array passed as first function argument         *
 *********************************************************************/
void fillWithRandomNumbers(int *array, int length) {
    
    srand( time(NULL) );
    
    for(int i=0; i < length; i++) {
        array[i] = (rand() % (2*MAX_VALUE)) - MAX_VALUE;
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


