#include<iostream>
#include<list>

//test LinkedList<T>
using namespace std;

// a predicate implemented as a function:
bool single_digit (const int& value) { return (value<10); }

void logDoublyLinkedList(std::list<int> list)
{
    std::fprintf(stdout, "\n{ ");
    for(auto it = list.begin(); it != list.end(); ++it) {
        std::fprintf(stdout, "%d, ", *it);
    }
    std::fprintf(stdout,"}\n\n");
}

int main(int argc, char *argv[])
{
    cout << endl;
    cout << endl;
    cout << "TESTOWY PROGRAM list<T>" << endl;
    cout << endl;
    cout << "Tworzenie listy..." << endl;
    list<int> list1;
    
    cout << "push_front 5, 26, 33" << endl;
    list1.push_front(5);
    list1.push_front(26);
    list1.push_front(33);
    cout << "--------------------------------------" << endl;
    
    cout << "push_back 5, 26, 33" << endl;
    list1.push_back(5);
    list1.push_back(26);
    list1.push_back(33);
    cout << "--------------------------------------" << endl;
    
    list<int>::iterator itr = list1.begin();
    list<int>::reverse_iterator reverse_itr = list1.rbegin();
    cout << endl;
    cout << "front()" << endl;
    cout << "pierwszy element listy: " << list1.front() << endl;
    cout << "--------------------------------------" << endl;
    
    cout << "back()" << endl;
    cout << "ostatni element listy: " << list1.back() << endl;
    cout << "--------------------------------------" << endl;
    
    cout << endl;
    cout << "*(++itr)" << endl;
    cout << "drugi element (indirection operator): " << *(++itr) << endl;
    cout << "--------------------------------------" << endl;
    
    cout << endl;
    cout << "*(++reverse_itr)" << endl;
    cout << "drugi od konca element (indirection operator): " << *(++reverse_itr) << endl;
    cout << "--------------------------------------" << endl;

    
    cout << endl;
    cout << "list.element_at(++itr)" << endl;
    cout << "trzeci element listy to: " << *(++itr) << endl;
    cout << "--------------------------------------" << endl;

    
    cout << endl;
    cout << "insert(begin(), 10)" << endl;
    list<int>::const_iterator const_itr = list1.begin();
    list1.insert(const_itr, 10);
    logDoublyLinkedList(list1);
    cout << "--------------------------------------" << endl;
    
    cout << "inser(begin(), 20)" << endl;
    const_itr = list1.begin();
    list1.insert(const_itr, 20);
    logDoublyLinkedList(list1);
    cout << "--------------------------------------" << endl;
    
    cout << "3 x pop_front()" << endl;
    list1.pop_front();
    list1.pop_front();
    list1.pop_front();
    logDoublyLinkedList(list1);
    cout << "--------------------------------------" << endl;
    
    cout << "3 x pop_back()" << endl;
    list1.pop_back();
    list1.pop_back();
    list1.pop_back();
    logDoublyLinkedList(list1);
    cout << "--------------------------------------" << endl;
    
    cout << "front()" << endl;
    cout << "pierwszy element listy: " << list1.front() << endl;
    logDoublyLinkedList(list1);
    cout << "--------------------------------------" << endl;
    
    cout << "back()" << endl;
    cout << "ostatni element listy: " << list1.back() << endl;
    logDoublyLinkedList(list1);
    cout << "--------------------------------------" << endl;
    
    cout << "empty()" << endl;
    if(list1.empty())
        cout << "Lista jest pusta" <<endl;
    else
        cout << "Lista nie jest pusta"<<endl;
    cout << "--------------------------------------" << endl;
    
    cout << "Tworzenie list2...." << endl;
    list<int> list2;
    
    cout << "list2.push_front 11, 21, 31" << endl;
    list2.push_front(11);
    list2.push_front(21);
    list2.push_front(31);
    logDoublyLinkedList(list2);
    cout << "--------------------------------------" << endl;
    
    cout << "list2.push_back 11, 21, 31" << endl;
    list2.push_back(11);
    list2.push_back(21);
    list2.push_back(31);
    logDoublyLinkedList(list2);
    cout << "--------------------------------------" << endl;

    
    cout << "list.push_front 1, 2, 3" << endl;
    list1.push_front(1);
    list1.push_front(2);
    list1.push_front(3);
    logDoublyLinkedList(list1);
    cout << "--------------------------------------" << endl;
    
    cout << "list.push_back 1, 2, 3" << endl;
    list1.push_back(1);
    list1.push_back(2);
    list1.push_back(3);
    logDoublyLinkedList(list1);
    cout << "--------------------------------------" << endl;
    
    cout << "list2.splice_after(begin(), list, list.begin(), itr)" << endl;
    auto it = list1.begin();
    it++; it++;
    list2.splice(list2.begin(), list1, list1.begin(), it);
    logDoublyLinkedList(list1);
    logDoublyLinkedList(list2);
    cout << "--------------------------------------" << endl;
    
    cout << "list2.remove(21)" << endl;
    list2.remove(21);
    logDoublyLinkedList(list2);
    cout << "--------------------------------------" << endl;
    
    cout << "list2.remove_if(single_digit)" << endl;
    list2.remove_if(single_digit);
    logDoublyLinkedList(list2);
    cout << "--------------------------------------" << endl;
    
    cout << "list2.push_front 3,3,3,1" << endl;
    list2.push_front(3);
    list2.push_front(3);
    list2.push_front(3);
    list2.push_front(1);
    logDoublyLinkedList(list2);
    cout << "--------------------------------------" << endl;
    
    cout << "list2.unique()" << endl;
    list2.unique();
    logDoublyLinkedList(list2);
    cout << "--------------------------------------" << endl;
    
    cout << "list2.reverse()" << endl;
    list2.reverse();
    logDoublyLinkedList(list2);
    cout << "--------------------------------------" << endl;
    
    list<int> list3;
    list3.push_front(2);
    list3.push_front(4);
    list<int> list4;
    list4.push_front(3);
    list4.push_front(5);
    
    cout << "list4.swap(list3)" << endl;
    cout << "przed swap()" << endl;
    logDoublyLinkedList(list3);
    logDoublyLinkedList(list4);
    list4.swap(list3);
    cout << "po swap()" << endl;
    logDoublyLinkedList(list3);
    logDoublyLinkedList(list4);
    cout << "--------------------------------------" << endl;
  
    cout << "list2.sort()" << endl;
    list2.sort();
    logDoublyLinkedList(list2);
    cout << "--------------------------------------" << endl;
   
    cout << "list.sort(std::greater<double>()) & list2.sort(std::greater<double>())" << endl;
    list1.sort(std::greater<double>());
    list2.sort(std::greater<double>());
    logDoublyLinkedList(list1);
    logDoublyLinkedList(list2);
    cout << "--------------------------------------" << endl;
    
    cout << "list.merge(list2, greater<double>())" << endl;
    list1.merge(list2, std::greater<double>());
    logDoublyLinkedList(list1);
    cout << "--------------------------------------" << endl;
    
    return 0;
}
