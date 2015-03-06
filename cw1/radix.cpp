#include<cmath>
#include<cstdlib>
#include<cstring>
#include<cstdio>

#define BASE 10

//tab - array of n numbers we are sorting
//n - number of numbers in array
//d - number of digits in largest number
void radix_sort(int *tab, int n, int d)
{
    //RADIX SORT for numbers lower than k <= n^O(1)
    //O(cn) c - constant -> O(n)
    //O((n+base)*d) where d = log_base(k))
    //array for counting digit occurances
    //in counting sub-sorting
    int *counts = new int[BASE];
    //array for intermediate (temporary) sorting results
    int *temp = new int[n];
    
    //process d digits starting from
    //least significant to most significant digit
    for(int i=0; i<d; i++)
    {
        //for each ith digit execute COUNTING SORT algorithm
        
        //counts array zeroing
        for(int j=0; j<BASE; j++)
        {
            counts[j] = 0;
        }
        
        //calculating divisor by which to divide number to
        //get to ith digit from the right side
        int position_divisor = (int) pow((double) BASE, (double) i);
        
        //counting occurances of each digit [0,BASE)
        //for ith digit position in all numbers (n - array size)
        for(int j=0; j < n; j++)
        {
            //calculating index in counts array
            //to which current number match
            //based on number, position_divisor and BASE
            int idx = (int) (tab[j]/position_divisor) % BASE;
            //increment number of occurances of given digit for
            //current digit position
            counts[idx]++;
        }
        
        //summing up number of counts in previous positions
        for(int j=1; j < BASE; j++)
        {
            counts[j] += counts[j-1];
        }
        
        //soritng elements for current ith digit position
        //based on counts array
        //we loop in reverse order to keep the same values
        //in the same order as before sorting (stable sort)
        for(int j=n-1; j >=0; j--) {
            int idx = (int) (tab[j]/position_divisor) %BASE;
            temp[counts[idx] -1] = tab[j];
            counts[idx]--;
        }
        
        memcpy(tab, temp, n *sizeof(int));
    }
}

int main(int argc, char **argv)
{
    
    int tab[11]= {0,3,4,700,900,-1,2,5,6,8,30};
    
    printf("PRZED POSORTOWANIEM\n");
    for(int i=0;i<11;i++)
        printf("%d  ", tab[i]);
    printf("\n\n");
    
    printf("SORTOWANIE RADIX_SORT()\n\n");
    radix_sort(tab, 11, 3);
    
    printf("PO POSORTOWANIU\n");
    for(int i=0;i<11;i++)
        printf("%d  ", tab[i]);
    printf("\n\n");
    
    return 0;
}