/****************************************************************
 *            @author Michal Ziobro, 24.10.2013                 *
 * bubbleSort, selectionSort, insertionSort, countingSort       *
 ****************************************************************/
#include<iostream>

using namespace std;


const int maxZ = 100; // maksymalna ilosc testow
const int maxSize = 10; // maksymalny rozmiar pojedynczego testu
const int maxValue = 1000; // maksymalna wartosc liczby z ciagu liczb

char *algorithmType; 

void scanInput(int *tab);
void bubbleSort(int *firstElement, int *lastElement);
void swap(int *i, int *j);
void selectionSort(int *firstElement, int *lastElement);
void insertionSort(int *firstElement, int *lastElement); 
void countingSort(int *firstElement, int *lastElement); 


//entry point to the program 
int main(int argc, char *argv[]) { 
	int tab[maxSize];

	if(argc != 2) {  printf("Uruchomienie: ./zestaw1.cpp bubbleSort|selectionSort|insertionSort|countingSort < input.dat > output.dat"); exit(0); }
	algorithmType = argv[1]; 

	if(strcmp(algorithmType, "bubbleSort") == 0) { 
		  printf("********************** BUBBLE SORT *************************\n"); 	
		 
	} else if(strcmp(algorithmType, "selectionSort") == 0) { 
		  printf("******************** SELECTION SORT ************************\n");
		 
	} else if(strcmp(algorithmType, "insertionSort") == 0) { 
		  printf("********************** INSERTION SORT *************************\n"); 
	 	  	
	} else if(strcmp(algorithmType, "countingSort") == 0) { 
		  printf("********************** COUNTING SORT  *************************\n");
		 
	} else { 
		  printf("********************** BUBBLE SORT *************************\n"); 	
		  
	}
		
	scanInput(tab);
}

/*********************************************************************
 * scanInput() - this method is scanning for input of data, executes *
 *               sorting algorithm and print sorted results          *
 *********************************************************************/
void scanInput(int *tab) {
	
	int z;
	scanf("%d", &z);
	for(int i=0; i<z; i++) {
		int size;
		scanf("%d", &size);
		for(int j=0; j<size; j++)
			scanf("%d", &tab[j]);

		if(strcmp(algorithmType, "bubbleSort") == 0) { 
		   	
		  bubbleSort(&tab[0], &tab[size]);
		} else if(strcmp(algorithmType, "selectionSort") == 0) { 
		  
		  selectionSort(&tab[0], &tab[size]);
		} else if(strcmp(algorithmType, "insertionSort") == 0) { 
		  
	 	  insertionSort(&tab[0], &tab[size]); 	
		} else if(strcmp(algorithmType, "countingSort") == 0) { 
		  
		  countingSort(&tab[0], &tab[size]);
		} else { 
		   	
		  bubbleSort(&tab[0], &tab[size]);
		}

		for(int j=0; j<size; j++)
			printf("%d ", tab[j]);
		printf("\n");
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

/*******************************************************************
 * selectionSort() - method rearranging array of elements in steps:*
 * 1) find the smallest item in all array and exchange it with     *
 *    first element  						   *
 * 2) find the smallest item in subarray 1 to n-1 and exchange it  *
 *    with second element 					   *
 * 3) continue along this way 					   *
 * there is N exchanges, and N - 1 - i comparisons in  ith loop    *
 * which gives approximatly N^2/2 compares			   *
 * Parameters: 							   *
 * @firstElement - pointer to the first element of sorted array    *
 * @lastElement - pointer to the last element of sorted array 	   *
 *******************************************************************/
void selectionSort(int *firstElement, int *lastElement) 
{

	while(firstElement < lastElement) { 

                //pointer to current minimal value in the subarray (p to lastElement)
		int *minimal = firstElement; 
		
		for(int *p = firstElement; p <= lastElement; p++) { 
			if( *p < *minimal) minimal = p; 
		}
		//exchanging value pointed by firstElement to smallest value pointed by minimal and selected in last inner loop pass in subarray  (i -> N)	
		swap(firstElement, minimal); 
		
		firstElement++; 
	}

}

/********************************************************************
 * insertionSort() - method rearranging array of element in steps:  *
 * 1) we start form firstElement and assume it is in right position *
 * 2) next we go to second element and comper it with  elements     *
 *    on the left side of it going form current element in left     *
 *    direction. Comparison stops when previous element is less or  *
 *    equal to current element. 				    *
 * 3) we go to 3, 4, ... i element and in each pass we take this ith*
 *    element and we are placing it in right position in the left   *
 *    part of array by comparing this ith element with previous     *
 *    element in reverse order (decrementing as long as we reach    *
 *    right position) 						    *
 * 4) sorting finishes when we reach the last element of the array  * 
 * This algorith is faster in partially sorted arrays		    *
 *  best case there is only N-1 compares and 0 exchanges  	    *
 *  on avarage there is N^2/4 compares and N^2/4 exchanges          *
 *  worst case scenario there is N^2/2 compares and N^2/2 exchanges *
 ********************************************************************/

 void insertionSort(int *firstElement, int* lastElement) 
 {
        int *currentElement = firstElement; 
	while(currentElement < lastElement) {

		//we assign currentElement to pointer p and we move it to the left comparing each time 
		//whether the element to the left is greater until this condition fails or we reach the leftmost position.
		for(int *p = currentElement; p > firstElement && (*p < *(p-1)); p--) 
		{
			swap(p, p-1); 
		}
		
		currentElement++; 
	}

 }


 /******************************************************************
  * countingSort() - implementation of Counting Sort algorithm     *
  * 1) we create array of size equal to range of possible values.  *
  * 2) we are zeroing this array of possible values  count[0],     *
  *   count[1]... count[k]     	     				   *
  * 3) we counts in array 'count' occurances of given element      *
  * 4) we pass through 'count' array and to ith elements counts of *
  *    previous elements. This way we get position for each value. *
  *    Count.Key -> value in sorted array, Count.Value -> position *
  * 5) we pass forword original array and place                    *
  *    each item on suitable position indicated by count[item]     *
  ******************************************************************/
void countingSort(int *firstElement, int *lastElement) 
{
	int count[maxValue];
	int sorted[maxSize]; 

	for(int i = 0; i < maxValue; i++) { 
		count[i] = 0; //zeroing count array
	}

	for(int *p = firstElement; p <= lastElement; p++) { 
		count[*p]++;  //increment number of occurances value indicated by pointer p
	}

        int total = 0, currentCount = 0; 
	for(int i=1; i < maxValue; i++) { 
	        currentCount = count[i];
		count[i] = total;
		total += currentCount; 
	}

	for(int *p = firstElement; p <= lastElement; p++) { 
		sorted[ count[*p]] = *p;
		count[*p]++;
	}

        for(int *p = firstElement, *s = sorted; p <= lastElement; p++, s++) { 
		*p = *s; //coping values form sorted array to original 'tab' array
        }

}
