#include<cstdlib>
#include<cstdio>

using namespace std;

int getMaxValueFromReadedNumbers(int*, int*);

//program entry point
// compilation: gcc -o zadanie6 zadanie6.cpp
// { ./zadanie6 < input6.dat }
int main(int argc, char *argv[]) {
    
    int maxValue; //will hold calculated maximal value in readed array of numbers
    int *array; //array which will be filled with readed positive numbers
    int lengthOfArray = 0; //initial length of array
    
    //initial allocation of array
    array = (int *) malloc(lengthOfArray * sizeof(int));
    
    //if function returns -1 it indicates that there wasn't any positive number
    //input, other value > -1 is the maximum readed integer value
    maxValue = getMaxValueFromReadedNumbers(array, &lengthOfArray);
    
    if(maxValue == -1) {
        printf("There wasn't any positive number in the input.");
    } else {
        printf("The maximum of the readed numbers is: %d.\n", maxValue);
    }
    
    return 0;
}

/***********************************************************************
 * getMaxValueFormReadedNumbers() - this function is scanning for input*
 * and reads new positive numbers into array (reallocating/resizing    *
 * array of positive numbers in each loop execution). Function is also *
 * comparing newly readed value with current maximal value stored in   *
 * "max" variable. The loop invariant is "max holds the maximal value  *
 * in current array[0..length-1].                                      *
 * Params:                                                             *
 * @array - pointer to array which will store positive numbers elements*
 * @length - pointer to int which holds current array size             *
 * Return value:                                                       *
 * - through parameters function is filling "array" and passing length *
 * @max - integer value indicating maximal value in readed array of    *
 *        numbers, can be -1 if array is empty                         *
 ***********************************************************************
int getMaxValueFromReadedNumbers(int *array, int *length)
{
    //initial max value is -1 because we scan only for positive integers
    int max = -1;
    /********************************************************************
     * max initialy holds -1 which is error flag when array of readed   *
     * elements is empty                                                *
     ********************************************************************/
    printf("Loop invariant at the beginning (empty array): %d.\n", max);
    //auxilliary variable to hold currently readed number
    int currentNumber = -1;
    
    //scanning for first integer input
    scanf("%d", &currentNumber);
    
    //loop executes while currently readed number is greater then or equal 0,
    //sequance of inputted numbers should end with -1 and then we have stop
    //condition of loop
    while(currentNumber >= 0) {
        /***************************************************************
         * LOOP INVARIANT:                                             *
         * max holds the maximal value in array[0.....length-1]        *
         ***************************************************************/
        
        //we have new positive number in sequence!
        (*length)++; //increment size of array of readed positive integers
        //reallocating array with incremented size
        array = (int *) realloc(array, (*length)*sizeof(int));
        //inserting newly readed positive number to next array postion
        array[(*length) - 1] = currentNumber;
        
        //checking condition whether currently readed number is greater then
        //existing maximal value
        if(currentNumber > max) max = currentNumber;
        
        /***************************************************************
         * LOOP INVARIANT:                                             *
         * max holds the maximal value in array[0.....length-1]        *
         ***************************************************************/
        //printing current loop invariant
        printf("Current loop invariant: %d.\n", max);
        
        //scanning for next integer input
        scanf("%d", &currentNumber);
    }
    
    //function returns maximal value of readed array of positive numbers
    return max;
}

