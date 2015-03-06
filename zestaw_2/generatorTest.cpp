using namespace std;

#include<cstdlib>
#include<ctime>
#include<cstdio>

const int maxZ = 100; // maksymalna ilosc testow
const int size1000 = 1000; //zbiór 1000 elementow
const int maxValue = 1000; // maksymalna wartosc liczby z ciagu liczb
const int numberOfSets = 8*5; // 8 różnych wielkości zbioru i 5 typów zbioru dla każdej wielkości

//funkcja generująca następujące typy zbiorów:
// - zbiór posortowany
// - zbiór posortowany odwrotnie
// - zbiór posortowany z losowym elementem na początku
// - zbiór posortowany z losowym elementem na końcu
// - zbiór z losowym rozkładem elementów
void generateSets(int);

int main() {
	srand( time(NULL) );
	
    printf("%d\n", numberOfSets);
    
	//generowanie zbiorów 1000 elementowych
    generateSets(size1000);
    //generowanie zbiorów 2000 elementowych
    generateSets(2*size1000);
    //generowanie zbiorów 4000 elementowych
    generateSets(4*size1000);
    //generowanie zbiorów 8000 elementowych
    generateSets(8*size1000);
    //generowanie zbiorów 16000 elementowych
    generateSets(16*size1000);
    //generowanie zbiorów 32000 elementowych
    generateSets(32*size1000);
    //generowanie zbiorów 64000 elementowych
    generateSets(64*size1000);
    //generowanie zbiorów 128000 elementowych
    generateSets(128*size1000);
		
	return 0;
}

void generateSets(int size)
{
    //1. generowanie zbioru posortowanego:
    printf("%d\n", size);
    
    for(int j=0; j<size; j++)
        printf("%d ", (j+1));
    printf("\n");
    
    //2. generowanie zbioru posortowanego odwrotnie:
    printf("%d\n", size);
    for(int j=size; j> 0; j--)
        printf("%d ", j);
    printf("\n");
    
    //3. generowanie zbioru posortowanego z losowym elementem na początku:
    printf("%d\n", size);
    //losowy pierwszy element
    printf("%d ", rand() % maxValue);
    //uporządkowane pozostałe n-1 elementów
    for(int j=1; j < size; j++)
        printf("%d ", j);
    printf("\n");
    
    //4. generowanie zbioru posortowanego z losowym elementem na końcu
    printf("%d\n", size);
    //uporządkowane początkowe n-1 elementów
    for(int j=0; j < size-1; j++)
        printf("%d ", j);
    //losowy ostatni element
    printf("%d ", rand() % maxValue);
    printf("\n");
    
    //5. zbiór z losowym rozkładem elementów
    printf("%d\n", size);
    //wszystkie elementy generowanego zbioru są losowe
    for(int j=0; j<size; j++)
        printf("%d ", rand() % maxValue);
    printf("\n");

}