#include<cstdlib>
#include<cstdio>
#include<ctime>


//defining size of array of numbers used in algorithm "searching maximal segment"
#define N 50
//maximal possible value in numbers array
#define MAX_VALUE 1000

//function prototypes:
void fillWithRandomNumbers(int *, int);
void printArray(int *, int);
int lengthOfLongestDecreasingSubsequence(int *, int);
int max(int, int);
int minIndex(int *sequence, int j, int x);
int minIndexBinarySearch(int *sequence, int j, int x);

//program entry point
int main(int argc, char *argv[]) {
    
    int sequence[N];
    int maxLengthOfDecreasingSubsequence = 0;
    
    printf("\n############## PROGRAM: NAJDLUZSZY MALEJACY PODCIAG ###############\n\n");
    
    //filling table with random numbers
    fillWithRandomNumbers(sequence, N);
    
    //printing generated random array
    printf("Program wygenerował następujący ciag liczb dodatnich:\n");
    printArray(sequence, N);
    
    printf("Szukanie najdluzszego malejacego podciagu...\n");
    
    //searching longest decreasing subsequence length
    maxLengthOfDecreasingSubsequence = lengthOfLongestDecreasingSubsequence(sequence, N);
    
    printf("Dlugosc najdluzszego malejacego podciagu to: %d\n", maxLengthOfDecreasingSubsequence);
    
    return 0;
}

/********************************************************************
 * lengthOfLongestDecreasingSubsequence() - function is searching   * 
 * for longest decreasing subsequence length.                       *
 * Params:                                                          *
 * @sequence - pointer to array of integers                         *
 * @length - integer value, size of table array                     *
 ********************************************************************/
int lengthOfLongestDecreasingSubsequence(int *sequence, int length) {
   
    int maxSubsequenceLength = 1;
    
    for(int i=0,x,k; i<length; i++) {
        printf("i = %d\n", i);
        x = sequence[i];
        sequence[i] = 0;
        
        //searching minimal j <= i so that x >= sequence[j]
        k = minIndex(sequence, i, x);
        printf("k = %d (linear search)\n", k);
        
        k = minIndexBinarySearch(sequence, i,x);
        printf("k = %d (binary search)\n", k);
        
        sequence[k] = x;
        maxSubsequenceLength = max(k, maxSubsequenceLength);
        
        printArray(sequence, length);
    }
    
    return maxSubsequenceLength;
}

//searching minimal j index <= i so that x > sequence[j] in linear time
int minIndex(int *sequence, int j, int x) {
    
    //'j' is firslty set to current value of counter 'i' in loop in function
    //lengthOfLongestDecreasingSubsequence()
    //we are decrementing 'j' as long as value x = sequence[i] is greater than or
    //equal to sequance[j] i.e. sequence[j] for initial j i greater than sequance[j]
    //for decrementing j counter (elements to the left from first sequence[j]
    for(;(j >= 0) && (x>= sequence[j]); j--);
    j++;
    return j;
    
}

//searching x in sorted array sequence[0..i] so that
int minIndexBinarySearch(int *sequence, int j, int x) {
    
    int leftEnd = 0; //left end
    int rightEnd = j; //right end
    int middle, val;
    
    int result = rightEnd; //searched minimal Index ther j <=i and x >= sequence[j]
    
    while(leftEnd < rightEnd) {
        
        middle = (leftEnd + rightEnd)/2;
        
        val = sequence[middle];
        
        if(x >= val) {
            result = middle;
            rightEnd = middle;
        } else {
            leftEnd = middle+1;
        }
        
    }
    
    return result;
    
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
 * @array - pointer to array of integer values                       *
 * @length - size of array passed as first function argument         *
 *********************************************************************/
void fillWithRandomNumbers(int *array, int length) {
    
    srand( time(NULL) );
    
    for(int i=0; i < length; i++) {
        array[i] = (rand() % MAX_VALUE);
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


