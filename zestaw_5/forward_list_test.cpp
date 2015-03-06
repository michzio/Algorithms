#include<iostream>
#include<forward_list>
#include<iterator>

using namespace std; 

void logForwardList(std::forward_list<int> list)
{
    std::fprintf(stdout, "\n{ ");
    for(auto it = list.begin(); it != list.end(); ++it) {
        std::fprintf(stdout, "%d, ", *it);
    }
    std::fprintf(stdout,"}\n\n");
}

// a predicate implemented as a function:
bool single_digit (const int& value) { return (value<10); }

int main(int argc, char *argv[])
{
    cout << endl;
    cout << endl;
    cout << "TESTOWY PROGRAM LinkedList<T>" << endl;
    cout << endl;
    cout << "Tworzenie listy..." << endl;
    std::forward_list<int> list;
    
    cout << "push_front 5, 26, 33" << endl;
    list.push_front(5);
    list.push_front(26);
    list.push_front(33);
    cout << "--------------------------------------" << endl;
    
    std::forward_list<int>::iterator itr = list.begin();
    cout << endl;
    cout << "front()" << endl;
    cout << "pierwszy element listy: " << list.front() << endl;
    cout << "--------------------------------------" << endl;
    
    cout << endl;
    cout << "*(++itr)" << endl;
    cout << "drugi element (indirection operator): " << *(++itr) << endl;
    cout << "--------------------------------------" << endl;
    
    cout << endl;
    cout << "list.element_at(++itr)" << endl;
    cout << "trzeci element listy to: " << "no member element_at()" << endl;
    cout << "--------------------------------------" << endl;
    
    cout << endl;
    cout << "insert_after(before_begin(), 10)" << endl;
    std::forward_list<int>::const_iterator const_itr = list.before_begin();
    list.insert_after(const_itr, 10);
    logForwardList(list);
    cout << "--------------------------------------" << endl;
    
    cout << "inser_after(cbegin(), 20)" << endl;
    const_itr = list.cbegin();
    list.insert_after(const_itr, 20);
    logForwardList(list);
    cout << "--------------------------------------" << endl;
    
    cout << "3 x pop_front()" << endl;
    list.pop_front();
    list.pop_front();
    list.pop_front();
    logForwardList(list);
    cout << "--------------------------------------" << endl;
    
    cout << "front()" << endl;
    cout << "pierwszy element listy: " << list.front() << endl;
    logForwardList(list);
    cout << "--------------------------------------" << endl;
    
    cout << "empty()" << endl;
    if(list.empty())
    cout << "Lista jest pusta" <<endl;
    else
    cout << "Lista nie jest pusta"<<endl;
    cout << "--------------------------------------" << endl;
    
    cout << "Tworzenie list2...." << endl;
    forward_list<int> list2;
    
    cout << "list2.push_front 11, 21, 31" << endl;
    list2.push_front(11);
    list2.push_front(21);
    list2.push_front(31);
    logForwardList(list2);
    cout << "--------------------------------------" << endl;
    
    
    cout << "list.push_front 1, 2, 3" << endl;
    list.push_front(1);
    list.push_front(2);
    list.push_front(3);
    logForwardList(list);
    cout << "--------------------------------------" << endl;
    
    cout << "list2.splice_after(before_begin(), list, list.begin(), itr)" << endl;
    auto it = list.begin();
    it++; it++;
    list2.splice_after(list2.before_begin(), list, list.begin(), it);
    logForwardList(list);
    logForwardList(list2);
    cout << "--------------------------------------" << endl;
    
    cout << "list2.remove(21)" << endl;
    list2.remove(21);
    logForwardList(list2);
    cout << "--------------------------------------" << endl;
    
    cout << "list2.remove_if(single_digit)" << endl;
    list2.remove_if(single_digit);
    logForwardList(list2);
    cout << "--------------------------------------" << endl;
    
    cout << "list2.push_front 3,3,3,1" << endl;
    list2.push_front(3);
    list2.push_front(3);
    list2.push_front(3);
    list2.push_front(1);
    logForwardList(list2);
    cout << "--------------------------------------" << endl;
    
    cout << "list2.unique()" << endl;
    list2.unique();
    logForwardList(list2);
    cout << "--------------------------------------" << endl;
    
    cout << "list2.reverse()" << endl;
    list2.reverse();
    logForwardList(list2);
    cout << "--------------------------------------" << endl;
    
    cout << "list2.sort()" << endl;
    list2.sort();
    logForwardList(list2);
    cout << "--------------------------------------" << endl;
    
    cout << "list.sort(std::greater<double>()) & list2.sort(std::greater<double>())" << endl;
    list.sort(std::greater<double>());
    list2.sort(std::greater<double>());
    logForwardList(list);
    logForwardList(list2);
    cout << "--------------------------------------" << endl;
    
    cout << "list.merge(list2, greater<double>())" << endl;
    list.merge(list2, std::greater<double>());
    logForwardList(list);
    cout << "--------------------------------------" << endl;
    
    return 0;
}


