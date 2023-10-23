//
//                       test_linked_list.cpp
//
//     A testing program that creates a CardLinkedList object and
//     tries the basic operations on it, using debug_print to report
//     on the internal state of all objects as it goes.
//
//                   Tufts University, Fall 2021 
//                        CS 11: Project 2
//
//    Modified by: Sara Francis
//

#include <iostream>
#include "Card.h"
#include "CardLinkedList.h"
using namespace std;

int main()
{
    cout << "Making card: value 33" << endl;
    Card c33(33);
    Card c20(20);
    Card c13(13);
    Card c19(19);
    Card c10(10);
    c33.debug_print();
    cout << endl;  // newline for viewing convenience

    cout << "Making linked list" << endl;
    CardLinkedList ll;
    ll.debug_print();

    cout << "Adding card 33 to list" << endl;
    ll.add_card_at(0, 33);
    ll.debug_print();

    // Add your own tests here
    cout << "Adding card 20 to list" << endl;
    ll.add_card_at(1, 20);
    ll.debug_print();
    
    cout << "Adding card 13 to list" << endl;
    ll.add_card_at(2, 13);
    ll.debug_print();
    
    cout << "Adding card 19 to list" << endl;
    ll.add_card_at(3, 19);
    ll.debug_print();
    
    cout << "Adding card 33 to list" << endl;
    ll.add_card_at(2, 33);
    ll.debug_print();
    
    cout << "Remove card 20 to list" << endl;
    ll.remove_card_at(1);
    ll.debug_print();

    cout << "Get card 33 to list" << endl;
    Card n20 = ll.get_card_at(2);
    ll.debug_print();
    n20.debug_print();
    
    cout << "Adding card 33 to list" << endl;
    ll.add_card_at(0, 10);
    ll.debug_print();
    
    return 0;
}

