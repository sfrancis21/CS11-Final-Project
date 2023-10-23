//
//                         CardLinkedList.cpp
//
//           A class that manages a linked list of cards
//
//  The data are stored as a singly-linked sequence of nodes accessed
//  from a single pointer (in the data member 'm_front') to the first
//  node in the list (or the nullptr if the list is empty). Each node
//  has a Card instance named card.
//
//  Author: Noah Mendelsohn 
//
//  Tufts University, Fall 2021
//  CS 11: Project 2
//
//  Modified by:
//  Date:
// 
//  Anything else you would like to include:
//

/**********************************************************/
/***      Students: Fill in the TODO items below        ***/
/***  You may add additional private member functions   ***/
/**********************************************************/

#include <iostream>
#include <iomanip>     // gets setw for debug printing
#include <string>

#include "CardLinkedList.h"

using namespace std;

/*------------------------------------------------------------------
 *                    Constructor(s) and Destructor
 *----------------------------------------------------------------*/

//
// Purpose:  Constructor initializes an empty list
//
CardLinkedList::CardLinkedList()
{
    m_front = nullptr;
    m_list_length = 0;
}

//
// Purpose:  Destructor recycles the space associated with all nodes
//           in the list
//
CardLinkedList::~CardLinkedList()
{
    while(m_front != nullptr) {
        Node *to_delete_p = m_front;
        if (to_delete_p != nullptr) {
            m_front = to_delete_p -> next;
            delete to_delete_p;
        }
    }
}

/*------------------------------------------------------------------
 *          Public functions to manipulate and query the list
 *----------------------------------------------------------------*/

//
// Purpose:  Return the current size of the list
//           i. e., the number of words/nodes in the list
//
unsigned int CardLinkedList::num_cards()
{
    return m_list_length;
}


//
// Purpose:  Add Card node to the list at the specified position. If index is
//           out of range then return quietly
//
//           Recycles the node containing the word (and its count)
//
void CardLinkedList::add_card_at(unsigned int index, Card c)
{    
    if(index == 0) {
        m_front = new Node{c, m_front};
    } else {
        Node *prev;
        unsigned int count = 0;
        for (prev = m_front; ((prev != nullptr) and (count < (index-1)));
            prev = prev->next) {
                count++;
        }
        if (prev == nullptr) {
            return;
        } else {
            Node *newNode_p = new Node{c, prev->next};
            prev->next = newNode_p;
        }
        
    }
    m_list_length = m_list_length + 1;
    return;
}

//
// Purpose:  Remove word from the list.  That is, it removes and
//           recycles the node containing the word (and its count)
//
void CardLinkedList::remove_card_at(unsigned int index)
{
    if (index == 0) {
        Node *to_delete_p = m_front;
        if (to_delete_p != nullptr) {
            m_front = to_delete_p->next;
            delete to_delete_p;
        }
    } else {
        Node *prev;
        Node *current;
        unsigned int count = 0;
        for (prev = m_front; ((prev != nullptr) and (count < (index-1)));
            prev = prev->next) {
                count++;
        }
        count = 0;
        for (current = m_front; ((current != nullptr) and (count < (index)));
            current = current->next) {
                count++;
        }
        if (current == nullptr) {
            return;
        } else {
            prev->next = current->next;
        }
        delete current;
    }
    m_list_length = m_list_length - 1;
    return;
}


//
// Purpose:  Return the index-th element of the list
// Params:   index must be in range [0 .. size) where
//           size is the number of words stored in the list
// Effects:  if index is out of range, prints error message and returns
//           NO_CARD_HERE
Card CardLinkedList::get_card_at(unsigned int index)
{
    Node *np;
    unsigned int count = 0;
    for (np = m_front; ((np != nullptr) and (count < index)); np = np->next) {
        count++;
    }
    if (np == nullptr) {
        return NO_CARD_HERE;
    } else {
        return np->card;
    }
}


//
// Purpose:  Print out the underlying list to cout
// Effects:  Prints to cout
// Notes:    This function is here for debugging purposes.
//           It will not be called in the final version of the program.
//           Still LEAVE IT AS-IS:  WE WILL USE IT FOR TESTING.
//
void CardLinkedList::debug_print()
{
    int index_num = 0;

    cout << "DEBUG: CardLinked List (num_cards = " << num_cards()
         << ") ["  << endl;
    cout <<  "  Index_num   Node Address  Face Value" << endl;
    for (Node *curr_p = m_front; curr_p != nullptr; curr_p = curr_p->next) {
        cout <<  setw(9) << index_num++;
        cout <<  setw(16) << curr_p;
        cout <<  "       ";
        curr_p->card.debug_print();
        cout << endl;
    }
    cout << "]" << endl;
}

