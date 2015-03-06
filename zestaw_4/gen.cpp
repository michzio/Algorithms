#include<cstdio>
#include<ctime>
#include<cstdlib>

using namespace std;

const int modulo = 1000000;

int main() {
    srand( time(NULL) );
    int tc = rand()%modulo;
    printf("%d\n", tc);
    for(int t=0; t<tc; t++) {
        int c = rand()%3;
        if(c==0) 
            printf("A %d\n", rand()%modulo);
        else if(c==1)
            printf("D\n");
        else
            printf("S\n");
    }
    return 0;
}
