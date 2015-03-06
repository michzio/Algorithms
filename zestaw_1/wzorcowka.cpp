using namespace std;

#include<cstdlib>
#include<ctime>
#include<cstdio>
#include<algorithm>

const int maxZ = 100; // maksymalna ilosc testow
const int maxSize = 10; // maksymalny rozmiar pojedynczego testu
const int maxValue = 1000; // maksymalna wartosc liczby z ciagu liczb

int main() {
	int tab[maxSize];
	int z;
	scanf("%d", &z);
	for(int i=0; i<z; i++) {
		int size;
		scanf("%d", &size);
		for(int j=0; j<size; j++)
			scanf("%d", &tab[j]);
		sort(&tab[0], &tab[size]);	
		for(int j=0; j<size; j++)
			printf("%d ", tab[j]);
		printf("\n");
	}
	return 0;
}
