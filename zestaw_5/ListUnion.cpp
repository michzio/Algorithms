#include<cstdio>
#include<list>

using namespace std;

int main() {
    int z;
    scanf("%d", &z);
    while(z--) {
        int size1;
        scanf("%d", &size1);
        list<int> L1;
        for(int i=0; i<size1; i++) {
            int element;
            scanf("%d", &element);
            L1.push_back(element);
        }
        int size2;
        scanf("%d", &size2);
        list<int> L2;
        for(int i=0; i<size2; i++) {
            int element;
            scanf("%d", &element);
            L2.push_back(element);
        }
        L1.merge(L2);
        for(list<int>::iterator it = L1.begin(); it != L1.end(); it++)
            printf("%d ", *it);
        printf("\n");    
        L1.unique();
        for(list<int>::iterator it = L1.begin(); it != L1.end(); it++)
            printf("%d ", *it);
        printf("\n");    
    
    }

}
