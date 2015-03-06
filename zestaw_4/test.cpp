#include<cstdio>
#include "linked_list.h"
#include "array_list.h"

void testArrayList(void);
void testLinkedList(void);

//entry point
int main(int argc, char *argv[])
{
    printf("\n\n######## PROGRAM TESTUJĄCY DZIAŁANIE LIST ########\n\n");
    
    if(argc != 2) {
        printf("\n Użycie programu: {./test ArrayList} lub {./test LinkedList}.\n");
        return 1;
    }
    
    if(strcmp(argv[1], "ArrayList")) {
        testArrayList();
    } else if(strcmp(argv[1], "LinkedList")) {
        testLinkedList();
    } else {
        printf("\n Błędny parametr wywołania programu.");
    }
    
    return 0;
}

//metoda testujaca strukture listy oparta o tablice
void testArrayList(void)
{
    int tc;
    scanf("%d", &tc);
    
    //tworzymy liste z tablica o pojemnosci odpowiadajacej liczbie komend
    ADT::ArrayList<int> L(tc);
    
    
    for(int t=0; t<tc; t++) {
        char c = ' ';
        while(c!='A' && c!='D' && c!='S')
            c = getchar(); // wczytuj znak po znaku dopoki nie napotkasz znaku dot. polecenia
        if(c=='A') {
            int num;
            scanf("%d", &num);
            //L.add_back(num);
        }
        else if(c=='D') {
            if(L.count() == 0) // lista pusta
                printf("EMPTY\n");
            else {
                int num = L.first();
                L.remove_first();
                printf("%d\n", num);
            }
        }
        else {
            printf("%d\n", L.count());
        }
    }

    
    
}
//metoda testujaca strukture listy oparta o wskaźniki
void testLinkedList(void)
{
    //tworzymy liste wskaźnikowa
    ADT::LinkedList<int> L;
   
    int tc;
    scanf("%d", &tc);
    
    for(int t=0; t<tc; t++) {
        char c = ' ';
        while(c!='A' && c!='D' && c!='S')
            c = getchar(); // wczytuj znak po znaku dopoki nie napotkasz znaku dot. polecenia
        if(c=='A') {
            int num;
            scanf("%d", &num);
            L.push_front(num);
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

}