#include<cstdlib>
#include<cstdio>
#include<ctime>

using namespace std;

//constant defining number of readed positive numbers into array
#define N 100
#define MAX_VALUE 1000

int readRandomNumbers(int *array, int length);
int getMaxValueInNumbers(int *array, int length);

//program entry point
// compilation:
// gcc -o zadanie7 zadanie7.cpp
// { ./zadanie7 }
int main(int argc, char *argv[]) {
    
    int numbers[N]; //array which will be filled with N randomly selected numbers
    int maxValue; //will hold calculated maximal value in array of numbers
    
    printf("\n### PROGRAM Z ZADANIA 7 ###\n");
    //Read N random numbers into array of positive numbers
    int flag = readRandomNumbers(numbers, N);
    if(flag == -1) {
        printf("Program encounter error while reading random numbers into array.");
        exit(0);
    }
    
    //searching maximal value in array of positive numbers
    maxValue = getMaxValueInNumbers(numbers, N);
    if(maxValue == -1) {
        printf("The numbers array was empty!\n");
    } else {
        printf("The maximum of the numbers array is: %d.\n", maxValue);
    }
    
    return 0;
}


int readRandomNumbers(int *array, int length) {
    
    srand(time(NULL));
    printf("Wypełnianie tablicy 100 losowymi liczbami z przedzialu [0,1000).\n");
    for(int i=0; i<length; i++) {
        array[i] = (rand() % MAX_VALUE);
        //random positive number will be less than MAX_VALUE
        printf("%d ", array[i]);
    }
    printf("\n\n");
}

int getMaxValueInNumbers(int *array, int length) {
    //initializing maximal value by first element of the array
    int max = array[0];
    /*****************************************************************
     * LOOP INVARIANT:                                               *
     * max equals the maximum value in a[0...0] &&  0<=length        *
     *****************************************************************/
    printf("Invariant value at the beginning: %d.\n", max);
    //searching max value in array
    for(int i=1; i<length; i++) {
        /*****************************************************************
         * LOOP INVARIANT:                                               *
         * max equals the maximum value in a[0...i-1]                    *
         *****************************************************************/
        if(array[i] > max) max = array[i];
        /*****************************************************************
         * max equals the maximum value in a[0...i]                      *
         *****************************************************************/
        
         printf("Current invariant value: %d.\n", max);
         //after i++ incrementation
        /*****************************************************************
         * LOOP INVARIANT:                                               *
         * max equals the maximum value in a[0...i-1]                    *
         *****************************************************************/
    }
    /**********************************************************************
     * LOOP INVARIANT:                                                    *
     * max equals the maximum value in a[0...length-1] &&  i==length      *
     **********************************************************************/
    return max;
}