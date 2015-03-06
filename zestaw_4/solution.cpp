#include<cstdio>
#include<list>

using namespace std;

int main() {
    list<int> L;
    int tc;
    scanf("%d", &tc);
    for(int t=0; t<tc; t++) {
        char c = ' ';
        while(c!='A' && c!='D' && c!='S')
            c = getchar(); // wczytuj znak po znaku dopoki nie napotkasz znaku dot. polecenia
        if(c=='A') {
            int num;
            scanf("%d", &num);
            L.push_back(num);
        }
        else if(c=='D') {
            if(L.empty()) // lista pusta
                printf("EMPTY\n");
            else {
                int num = L.front();
                L.pop_front();
                printf("%d\n", num);
            }
        }
        else {
            printf("%d\n", L.size());
        }
    }
    return 0;
}
