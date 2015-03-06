#include<cstdlib>
#include<cstdio>
#include<ctime>

//size of arrays
#define N 20
//maximal possible number in arrays
#define MAX_VALUE 100

//function prototypes:
void fillWithRandomNumbers(int *, int);
void bubbleSort(int *, int*);
void swap(int *, int *);
void printArray(int *, int);
bool searchSum(int *, int*, int, int);

//program entry point
//compilation: g++ -o szukanie_sumy szukanie_sumy.cpp
//{ ./szukanie_sumy , podanie liczby x }
int main(int argc, char *argv[])
{
    //declaration of two arrays, each should be sorted in ascending order
    int tabA[N];
    int tabB[N];
    //numberfor which we are searching whether exists sum a+b equal to it
    int x;
    
    printf("\n############## PROGRAM: SZUKANIE SUMY ###############\n\n");
   
    srand( time(NULL) );
    
    //filling tabA with random numbers
    fillWithRandomNumbers(tabA, N);
    //filling tabB with random numbers
    fillWithRandomNumbers(tabB, N);
    
    //printing generated random arrays
    printf("Program wygenerował następujące tablice liczb:\n");
    printf("Tablica A: \n");
    printArray(tabA, N);
    printf("Tablica B: \n");
    printArray(tabB, N);
    
    //sorting both arrays in ascending order!
    //using bubbleSort from "zestaw_1"
    bubbleSort(&tabA[0], &tabA[N-1]);
    bubbleSort(&tabB[0], &tabB[N-1]);
    
    //printing sorted arrays
    printf("Tablice zostały posortowane: \n");
    printf("Tablica A: \n");
    printArray(tabA, N);
    printf("Tablica B: \n");
    printArray(tabB, N);
    
    printf("Podaj liczbę 'x' dla której należy szykać sumy: \n");
    scanf("%d", &x);
    
    //searching whether exists sum of element 'a' belonging to tabA
    //and element 'b' belonging to tabB which is equal to 'x'  (x = a + b)
    bool flag = searchSum(tabA, tabB, N, x);
    
    if(flag == true) {
        printf("Znaleziono sumę 'a + b = x' dla a należącego do Tablicy A i b należącego do Tablicy B\n");
    } else {
        printf("Nie udało się odnaleść elementu a należącego do Tablicy A i b należącego do Tablicy B, które spełniałyby warunek 'a + b = x' \n");
    }
    
    return 0;
}

bool searchSum(int *tabA, int *tabB, int length, int x)
{
    //flag inicating whether algorithm found sum of a + b equal to x
    //initially set to FALSE!
    bool sumEqualToXFounded = false;
    
    //counter i starts at the left end of tabA array
    //counter j starts at the right end of tabB array
    int i = 0, j = length -1;
    
    while( i<length && j >=0) {
        if((tabA[i] + tabB[j]) == x) {
            //for current values of 'i' and 'j' indices a + b = x!
            sumEqualToXFounded = true;
            break;
        } else if((tabA[i] + tabB[j]) < x) {
            //a + b < x, b is greatest number in tabB array so we can only
            //find a + b = x if we increment i counter
            i++;
        } else {
            //a + b > x, a is smallest number in tabA array so we can only
            //find a + b = x if we decrement j counter
            j--;
        }
    }
    
    return sumEqualToXFounded;
}

/*********************************************************************
 * fillWithRandomNumbers() - fills all elements of array  passed to  *
 * this function as first argument with randomly genereted numbers   *
 * Params:                                                           *
 * @array - pointer to array of integer values                       *
 * @length - size of array passed as first function argument         *
 *********************************************************************/
void fillWithRandomNumbers(int *array, int length) {
    
    for(int i=0; i < length; i++) {
        array[i] = (rand() % MAX_VALUE);
    }
}

/*********************************************************************
 *  bubbleSort() -    method rearranging elements of tab array using *
 *  pointers arithmetic. Function takes two arguments:               *
 *  @firstElement -  this is pointer to first element of the tab     *
 *  		     array (starting point)                          *
 *  @lastElement - this is pointer to last element of the tab array  *
 * This function executes following steps:                           *
 * 1) looping through array n-1 times in 'while' loop, in each loop  *
 *    lastElement pointer is decremented (using pointer arithmetic)  *
 *    after n-1 loops lastElement will have pointer address equal to *
 *    firstElement.                                                  *
 * 2) in each execution of 'while' loop there is 'for' loop passing  *
 *    through each element in array starting at firstElement and     *
 *    finishing at current lastElement (lastElement is continuosly   *
 *    decrementing while at the end of array are placed maximum      *
 *    values of current subarray).                                   *
 * 3) in 'for' loop there is comparision of value pointed by current *
 *    pointer value and value pointed by next pointer, if thay are   *
 *    in wrong order i.e. p1 > p2 we are swapping this two values    *
 *    to achive correct orfer.                                       *
 *********************************************************************/
void bubbleSort(int *firstElement, int *lastElement) {
    
    while(firstElement < lastElement) {
		//printf("Rearanging %d elements.\n", (int) (lastElement - firstElement));
        for(int *p = firstElement; p <lastElement; p++) {
            //printf("Comparing %d and %d\n", *p, *(p+1));
			if(*p > *(p+1)) {
				//printf("Swapping.\n");
				swap(p, (p+1));
			}
        }
        
		lastElement--;
    }
    
}

/***********************************************************************
 * swap() - this method exchages values pointed by pointers a and b    *
 * Params:    							       *
 * @a - pointer to integer value in array 			       *
 * @b - pointer to intefer value in array 			       *
 ***********************************************************************/
void swap(int *a, int *b) {
    
	int temp;
    
	temp = *a;
	*a = *b;
	*b = temp;
	
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
