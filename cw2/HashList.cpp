#include <iostream>

using namespace std;

const int HASHTAB_SIZE = 100;
const int EMPTY = -1;

struct Item
{
    int Nr;
    Item *Next;
};

class HashTab
{
    Item _Table[HASHTAB_SIZE];

    unsigned int Hash(int pIndexNr) { return pIndexNr % 100; }

public:
    HashTab() 
    { 
        for (unsigned int i = 0; i < HASHTAB_SIZE; i++) {
            _Table[i].Nr = EMPTY;
            _Table[i].Next = NULL;
        }
    }
    void Insert(unsigned int pIndexNr);
    bool Find(unsigned int pIndexNr);
    bool Remove(unsigned int pIndexNr);
};

void HashTab::Insert(unsigned int pIndexNr)
{
    unsigned int Ind = Hash(pIndexNr);
    if (_Table[Ind].Nr == EMPTY) {
        _Table[Ind].Nr = pIndexNr;
        return;
    } 
    Item *It = new Item;
    It->Nr = pIndexNr;
    It->Next = NULL;

    if (_Table[Ind].Next == NULL) {
        _Table[Ind].Next = It;
        return;
    }
    Item *i;
    for (i = _Table[Ind].Next; i->Next != NULL; i = i->Next) 
        ;

    i->Next = It;
}

bool HashTab::Find(unsigned int pIndexNr)
{
    unsigned int Ind = Hash(pIndexNr);
    if (_Table[Ind].Nr == EMPTY) 
        return false;
    
    for (Item *It = &_Table[Ind]; It != NULL; It = It->Next)
        if (It->Nr == pIndexNr)
            return true;

    return false;
}

bool HashTab::Remove(unsigned int pIndexNr)
{
    unsigned int Ind = Hash(pIndexNr);
    if (_Table[Ind].Nr == EMPTY) 
        return false;

    if ((_Table[Ind].Nr != pIndexNr) && (_Table[Ind].Next == NULL))
        return false;

    if (_Table[Ind].Nr == pIndexNr) {
        if (_Table[Ind].Next == NULL)
            _Table[Ind].Nr = EMPTY;

        return true;
    }
    Item *i;
    for (i = &_Table[Ind]; (i->Next != NULL) && (i->Next->Nr != pIndexNr); i = i->Next) 
        ;

    if (i->Next == NULL)
        return false;

    Item *It = i->Next;
    i->Next = It->Next;
    delete It;
    return true;
}

int main()
{
    HashTab Tab;

    Tab.Insert(124234);
    Tab.Insert(123334);
    Tab.Insert(234234);
    Tab.Insert(24234234);
    Tab.Insert(546575);
    Tab.Insert(235355);
    Tab.Insert(567567);

    cout << Tab.Find(124234) << endl;
    cout << Tab.Find(234234) << endl;
    cout << Tab.Find(123334) << endl;
    cout << Tab.Find(24234234) << endl;
    cout << Tab.Find(567567) << endl;

    Tab.Remove(123334);
    cout << Tab.Find(124234) << endl;
    cout << Tab.Find(234234) << endl;
    cout << Tab.Find(123334) << endl;
    cout << Tab.Find(24234234) << endl;

    return 0;
}

