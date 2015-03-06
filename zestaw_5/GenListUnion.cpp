#include<cstdio>
#include<cstdlib>
#include<ctime>
#include<list>

using namespace std;

int modZ = 100;
int modS = 100;
int modE = 1000;

int main() {
    srand(time(NULL));
    int z = rand()%modZ + 1;
    printf("%d\n", z);
    while(z--) {
        int s1 = rand()%modS + 1;
        list<int> L1;
        for(int i=0; i<s1; i++)
            L1.push_back(rand()%modE);
        L1.sort();
        printf("%d\n", s1);
        for(list<int>::iterator it = L1.begin(); it != L1.end(); it++)    
            printf("%d ", *it);
        printf("\n");
        s1 = rand()%modS + 1;
        L1.clear();
        for(int i=0; i<s1; i++)
            L1.push_back(rand()%modE);
        L1.sort();
        printf("%d\n", s1);
        for(list<int>::iterator it = L1.begin(); it != L1.end(); it++)    
            printf("%d ", *it);
        printf("\n");
        
    }
}
