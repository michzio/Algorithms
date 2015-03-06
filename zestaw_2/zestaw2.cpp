/****************************************************************
 *            @author Michal Ziobro, 24.10.2013                 *
 * mergeSort                                                    *
 ****************************************************************/
#include<iostream>
#include<cstdlib>
#include<cstdio>
#include<ctime>

using namespace std;


const int maxZ = 100; // maksymalna ilosc testow
const int maxSize = 128000; // maksymalny rozmiar pojedynczego testu
const int maxValue = 1000; // maksymalna wartosc liczby z ciagu liczb

//variable containing information about type of sorting algorithm selected by client while running program, ex. quickSort, mergeSort
char *algorithmType; 

//auxulliary array used by merge sort algorithm to place elements while merging subarrays
int auxulliaryArray[maxSize];
//array containing elements which will be sorted by selected algorithm
int tab[maxSize];

float timespans[40];

//function scans for input, reads data possibly from input.dat place them in 'tab' array, execute suitable sorting algorithm, and prints the result to the stdout
void scanInput(int *tab);
//function executing Merge Sort algorithm, divide-and-conquer, calls recursively mergeSort() on first half of array and second half of array, than executes mergeSortedSubarays() function
void mergeSort(int *firstElement, int *lastElement);
//function merging two sorted sunarrays into one array 
void mergeSortedSubarrays(int *firstElement, int *middleElement, int *lastElement);
//implementation of quickSort algorithm
void quickSort(int *array, int lowestIdx, int highestIdx);
void printTimeSpans(void);

//entry point to the program 
int main(int argc, char *argv[]) { 

        if(argc == 2) algorithmType = argv[1];
        else algorithmType = "mergeSort";

        if(strcmp(algorithmType, "mergeSort") == 0) { 
		printf("********************** MERGE SORT *************************\n");
 	} else if(strcmp(algorithmType, "quickSort") == 0) { 
		printf("********************** QUICK SORT *************************\n");
 	}
		
	scanInput(tab);

	printTimeSpans(); 
}

void printTimeSpans(void) {
	 if(strcmp(algorithmType, "mergeSort") == 0) { 
		printf("********************** MERGE SORT *************************\n");
 	} else if(strcmp(algorithmType, "quickSort") == 0) { 
		printf("********************** QUICK SORT *************************\n");
 	}
 	
	printf("Czas sortowania zbioru posortowanego (1000 elementow): %g\n", timespans[0]); 
	printf("Czas sortowania zbioru posortowanego odwrotnie (1000 elementow): %g\n", timespans[1]); 
	printf("Czas sortowania zbioru posortowanego z losowym elementem na poczatku (1000 elementow): %g\n", timespans[2]);
	printf("Czas sortowania zbioru posortowanego z losowym elementem na koncu (1000 elementow): %g\n", timespans[3]);  
	printf("Czas sortowania zbioru z losowym rozkladem elementow (1000 elementow): %g\n", timespans[4]); 

	printf("Czas sortowania zbioru posortowanego (2000 elementow): %g\n", timespans[5]); 
	printf("Czas sortowania zbioru posortowanego odwrotnie (2000 elementow): %g\n", timespans[6]); 
	printf("Czas sortowania zbioru posortowanego z losowym elementem na poczatku (2000 elementow): %g\n", timespans[7]);
	printf("Czas sortowania zbioru posortowanego z losowym elementem na koncu (2000 elementow): %g\n", timespans[8]);  
	printf("Czas sortowania zbioru z losowym rozkladem elementow (2000 elementow): %g\n", timespans[9]);

	printf("Czas sortowania zbioru posortowanego (4000 elementow): %g\n", timespans[10]); 
	printf("Czas sortowania zbioru posortowanego odwrotnie (4000 elementow): %g\n", timespans[11]); 
	printf("Czas sortowania zbioru posortowanego z losowym elementem na poczatku (4000 elementow): %g\n", timespans[12]);
	printf("Czas sortowania zbioru posortowanego z losowym elementem na koncu (4000 elementow): %g\n", timespans[13]);  
	printf("Czas sortowania zbioru z losowym rozkladem elementow (4000 elementow): %g\n", timespans[14]);  

	printf("Czas sortowania zbioru posortowanego (8000 elementow): %g\n", timespans[15]); 
	printf("Czas sortowania zbioru posortowanego odwrotnie (8000 elementow): %g\n", timespans[16]); 
	printf("Czas sortowania zbioru posortowanego z losowym elementem na poczatku (8000 elementow): %g\n", timespans[17]);
	printf("Czas sortowania zbioru posortowanego z losowym elementem na koncu (8000 elementow): %g\n", timespans[18]);  
	printf("Czas sortowania zbioru z losowym rozkladem elementow (8000 elementow): %g\n", timespans[19]); 

	printf("Czas sortowania zbioru posortowanego (16000 elementow): %g\n", timespans[20]); 
	printf("Czas sortowania zbioru posortowanego odwrotnie (16000 elementow): %g\n", timespans[21]); 
	printf("Czas sortowania zbioru posortowanego z losowym elementem na poczatku (16000 elementow): %g\n", timespans[22]);
	printf("Czas sortowania zbioru posortowanego z losowym elementem na koncu (16000 elementow): %g\n", timespans[23]);  
	printf("Czas sortowania zbioru z losowym rozkladem elementow (16000 elementow): %g\n", timespans[24]); 

	printf("Czas sortowania zbioru posortowanego (32000 elementow): %g\n", timespans[25]); 
	printf("Czas sortowania zbioru posortowanego odwrotnie (32000 elementow): %g\n", timespans[26]); 
	printf("Czas sortowania zbioru posortowanego z losowym elementem na poczatku (32000 elementow): %g\n", timespans[27]);
	printf("Czas sortowania zbioru posortowanego z losowym elementem na koncu (32000 elementow): %g\n", timespans[28]);  
	printf("Czas sortowania zbioru z losowym rozkladem elementow (32000 elementow): %g\n", timespans[29]); 

	printf("Czas sortowania zbioru posortowanego (64000 elementow): %g\n", timespans[30]); 
	printf("Czas sortowania zbioru posortowanego odwrotnie (64000 elementow): %g\n", timespans[31]); 
	printf("Czas sortowania zbioru posortowanego z losowym elementem na poczatku (64000 elementow): %g\n", timespans[32]);
	printf("Czas sortowania zbioru posortowanego z losowym elementem na koncu (64000 elementow): %g\n", timespans[33]);  
	printf("Czas sortowania zbioru z losowym rozkladem elementow (64000 elementow): %g\n", timespans[34]); 

	printf("Czas sortowania zbioru posortowanego (128000 elementow): %g\n", timespans[35]); 
	printf("Czas sortowania zbioru posortowanego odwrotnie (128000 elementow): %g\n", timespans[36]); 
	printf("Czas sortowania zbioru posortowanego z losowym elementem na poczatku (128000 elementow): %g\n", timespans[37]);
	printf("Czas sortowania zbioru posortowanego z losowym elementem na koncu (128000 elementow): %g\n", timespans[38]);  
	printf("Czas sortowania zbioru z losowym rozkladem elementow (128000 elementow): %g\n", timespans[39]); 
}

/*********************************************************************
 * scanInput() - this method is scanning for input of data, executes *
 *               sorting algorithm and print sorted results          *
 *********************************************************************/
void scanInput(int *tab) {
	
	int z, idx_in_timespans = 0;
	float timespan; 
	clock_t t0, t1;
	
	scanf("%d", &z);
	for(int i=0; i<z; i++) {
		int size;
		scanf("%d", &size);
		for(int j=0; j<size; j++)
			scanf("%d", &tab[j]);

                 t0 = clock();
                  

		if(strcmp(algorithmType, "mergeSort") == 0) { 
		        mergeSort(&tab[0], &tab[size]);
		 } else if(strcmp(algorithmType, "quickSort") == 0) { 
			quickSort(tab, 0, size); 
		 }

		 t1 = clock();
		 
		 timespan = (((float)t1 - (float)t0) / CLOCKS_PER_SEC  ) * 1000; 
		 
		 timespans[idx_in_timespans++] = timespan;

		for(int j=0; j<size; j++)
			printf("%d ", tab[j]);
		printf("\n");
	}
}

/**********************************************************************
 * mergeSort() - function implements merge sort algorithm             *
 * 1) it checks whether firstElement < lastElement else return from   *
 *    function							      *
 * 2) calculating pointer to middleElement of array                   *
 * 3) calling recursively mergeSort on left half of array (from first *
 *    to middle element) and right half of array (from middle to last)*
 * 4) calling mergeSortedSubarrays(), which merges two sorted halfs of*
 *    array.							      *
 * Params: 							      *
 * @firstElement - pointer to first element of the array to sort      *
 * @lastElement - pointer to last element of the array to sort        *
 **********************************************************************/
void mergeSort(int *firstElement, int *lastElement) 
  {
    
    if(lastElement <= firstElement) return; 
    //calculate pointer to middle element
    int *middleElement = firstElement + (lastElement - firstElement)/2;
    //sort left half of the array
    mergeSort(firstElement, middleElement);
    //sort right half of the array
    mergeSort((middleElement + 1), lastElement);
    //merge sorted subarrays
    mergeSortedSubarrays(firstElement, middleElement, lastElement);
  }

/***********************************************************************
 * mergeSortedSubarrays() - function executing merging of two sorted   *
 * halfs of the array. 						       *
 * Params: 							       *
 * @firstElement - pointer to first element of first subarray          *
 * @middleElement - pointer to last element of first subarray          *
 *   	            middleElement +1 is first element of second subarray*
 * @lastElement - pointer to last element of second subarray	       *
 ***********************************************************************/
void mergeSortedSubarrays(int *firstElement, int *middleElement, int *lastElement)
{
  //copying elements from subarrays  to auxilliary array using pointer arithmetic
  for(int *p = firstElement; p <= lastElement; p++) 
    {
      //calculating current index in array based on pointer p and pointer to start of original array
      int position = p - tab; 
      //copying value pointed by p to auxulliary array at corresponding position (index position)
      auxulliaryArray[position] = *p;
    }
  
  //start indexe of first subarray
  int firstArrayIndex = (firstElement - tab); 
  //index of middle element which is also the last element of first subarray
  int middleElementIndex = (middleElement - tab);
  //start index of seconf subarray
  int secondArrayIndex = middleElementIndex + 1;
  //last index of merged array (or second subarray)
  int lastElementIndex = (lastElement - tab);

  //loop through all elements of array which we get after merging subarrays
  for(int *p = firstElement; p <= lastElement; p++)
    {
      //if we used all elements in first subarray while merging we add to merged array only remaining elements from second subarray
      if(firstArrayIndex > middleElementIndex) *p = auxulliaryArray[secondArrayIndex++];
      //if we used all elements in second subarray while merging we add to merged array only remaining elements from first subarray
      else if (secondArrayIndex > lastElementIndex) *p = auxulliaryArray[firstArrayIndex++];
      //comparing next elements in first and second array and adding to merged array smaller value.
      else if(auxulliaryArray[firstArrayIndex] > auxulliaryArray[secondArrayIndex])
	*p = auxulliaryArray[secondArrayIndex++];
      else 
	*p = auxulliaryArray[firstArrayIndex++];

    }

}


/**********************************************************************
 * quickSort() - alternative fast (NLogN) sorting algorithm           *
 * 1) if there remain only 1 or less elements array is already sorted *
 * 2) selecting pivot alement in the middle of the array 	      *
 * 3) making array to contain on the left side of pivot elements with *
 *    value lower then pivot and on the right side elements with value*
 *    greater then pivot. if elements are already on right position   *
 *    we increment/decrement indices i,j, where we find elements on   *
 *    wrong positon in each halfs we exchange them together.          *
 * 4) recursively sorting remaining subarrays 			      *
 **********************************************************************/
void quickSort(int *array, int lowestIdx, int highestIdx) 
{
	//if array has one or less elements it is already sorted
  	if((highestIdx - lowestIdx) <1) 
		return;
   
  	 //selecting pivot element as middle element of array
	 int pivot = array[(lowestIdx + highestIdx)/2];
         
	int i = lowestIdx;
	int j = highestIdx;
        //loop executing while index i is less than or equal to index j
	do { 
		//increment index i while ith element value is less than pivot value
		while(array[i] < pivot) i++;
		//decrement index j while jth element value is greater than pivot value
		while(array[j] > pivot) j--;
		//while after incrementing index i and decrementing index j they meet 			//condition i<=j we swap this two elements array[i] and array[j] 
		//in this case array[i] must by greater than pivot and array[j] is 
		//less than pivot  
		if(i<=j) {
			int temporary = array[i];
			array[i] = array[j];
			array[j] = temporary;
			//after swaping this two elements thay now are in good parts 				//of the array so we can increment i index and decrement j 				//index
			i++; j--;
		}
	} while(i<=j); 
	if(lowestIdx<j) quickSort(array, lowestIdx, j);
	if(i<highestIdx) quickSort(array, i, highestIdx); 
}
