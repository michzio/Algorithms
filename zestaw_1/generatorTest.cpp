using namespace std;

#include<cstdlib>
#include<ctime>
#include<cstdio>

const int maxZ = 100; // maksymalna ilosc testow
const int maxSize = 10; // maksymalny rozmiar pojedynczego testu
const int maxValue = 1000; // maksymalna wartosc liczby z ciagu liczb

int main() {
	srand( time(NULL) );
	int z = rand() % maxZ;
	z++; // zeby bylo >= 1
	printf("%d\n", z);
	for(int i=0; i<z; i++) {
		int size = rand() % maxSize;
		size++; // zeby bylo >= 1
		printf("%d\n", size);
		for(int j=0; j<size; j++)
			printf("%d ", rand() % maxValue);
		printf("\n");
	}
	return 0;
}
