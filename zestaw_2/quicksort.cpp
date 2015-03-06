//program entry point
#include<iostream>
void quickSort(int array[], int lowestIdx, int highestIdx);

int main(void) { 

	int testArray[] = {6,2,6,7,25,78,34,21,15,14,46}; 
	
	quickSort(testArray, 0, 10); 

	for (int i = 10; i >= 0; i--) 
    		std::cout << testArray[i];
}

void quickSort(int array[], int lowestIdx, int highestIdx) 
{
	//if array has one or less elements it is already sorted
  	if((highestIdx - lowestIdx) <1) 
		return array;
   
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
	while(i<=j); 
	if(lowestIdx<j) quickSort(array, lowestIdx, j);
	if(i<highestIdx) quickSort(array, i, highestIdx); 
}
