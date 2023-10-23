//****************************************************************
//
//                         Hand.cpp
//
//     Represents a single hand with 0 or more cards.
//     This is a thin wrapper around Card_Linked_List, but Hands
//     can draw themselves either as a string on cout using "print"
//     or on a canvas using "draw".
//
//     Note that most functions are delegated directly to the 
//     underlying Card_Linked_List, and that code is mostly in 
//     Hand.h since it's so short.
//
//     Author: Noah Mendelsohn
//
//     Tufts University, Fall 2021
//     CS 11: Project 2
//
//     Modified by:
//     Date:
//
//****************************************************************

#include <iostream>

#include "Hand.h"

using namespace std;

//****************************************************************
//                 Constructor and Destructor
//****************************************************************

Hand::Hand()
{
    // Add code here if you think it's needed
}

// Destructor
Hand::~Hand()
{
    // Add code here if you think it's needed
}


//****************************************************************
//                     num_cards
//
//         Return number of cards in the hand
//****************************************************************

unsigned int Hand::num_cards()
{
    // Add code here to get num_cards from ll
    // Replace the place-holder return 0 and print statement below.
    return m_card_list.num_cards();                        // Replace me
}
  
//****************************************************************
//                       add_card
//
//    Insert a card at the specified position. For an empty list, the
//    only sensible index is 0. If the index is out of range on any
//    insertion, make no changes and quietly return.
//
//****************************************************************

void Hand::add_card_at(unsigned int index, Card c)
{
    // Add code here to add the card using m_card_list function(s)
    // Replace the void casts and print statement below.
    m_card_list.add_card_at(index, c);
}

//****************************************************************
//                      remove_card_at
//
//    Remove a card at the specified position. If the list is empty or
//    index is out of range, quietly do nothing.
//
//****************************************************************

void Hand::remove_card_at(unsigned int index)
{
    // Add code here to remove the card using m_card_list function(s)
    // Replace the void casts and print statement below.
    m_card_list.remove_card_at(index);
}

//****************************************************************
//                        get_card_at
//
//    Return a copy of a card at the specified position.  If the list
//    is empty or index is out of range, return a card with value
//    NO_CARD_HERE.
//
//****************************************************************

Card Hand::get_card_at(unsigned int index)
{
    // Add code here to get the card using m_card_list function(s)
    // Replace the place-holder return Card(1) below
    // and also the void cast.
    return m_card_list.get_card_at(index);
}

//****************************************************************
//                      debug_print 
//
//    Calls debug_print on the underlying CardLinkedList
//
//****************************************************************

void Hand::debug_print()
{
    m_card_list.debug_print();  // prints all cards in the list
}

//****************************************************************
//                          print
//
//   Print the card numbers to cout in the form:
//  
//                32, 13, 15, ... 12
//
//   There is a comma and a space between each pair
//   of numbers. The line is ended with a newline.
//
//   If the hand is empty then just the newline prints.
//
//****************************************************************

void Hand::print()
{
    int num = num_cards();
    for(int i = 0; i < num - 1; i++) {
        Card temp = get_card_at(i);
        cout << temp.value() << ", ";
    }
    Card temp = get_card_at(num - 1);
    cout << temp.value() << endl;
    // Add code here to print a single line with the values of
    // all cards, in order, separated by commas, and followed
    // by a single newline.
    //
    // So for a list containing 3 cards with values 13, 2, 5
    // print the 8 characters:
    //           13, 2, 5
    // followed by a newline (endl). There must be no extra commas
    // and if there is only one item in the list, no commas
    // at all (test this!).
    //
    // If the list is empty then print only a single newline (endl).
}

//=========================================================
//
//          THE FOLLOWING CODE IS USED ONLY
//          IN GRAPHICS MODE. DON'T WORRY ABOUT
//          IT UNTIL YOU GET TO THAT.
//
//=========================================================

//
//   Divide and round up
//
unsigned int Hand::divide_round_up(unsigned int dividend, unsigned int divisor)
{
    return ((dividend + divisor - 1) / divisor);
}

void Hand::draw_card_row(GraphicsCanvas *gc_p,
                         unsigned int first_card_index,
                         int row, int col, int spacing)
{
    int num_cards_in_deck = num_cards();
    int cards_to_draw = std::min(CARDS_PER_DISPLAY_ROW,
                                 num_cards_in_deck - first_card_index);
    int next_col = col;

    for (unsigned int card_num = first_card_index;
         card_num < first_card_index + cards_to_draw;
         card_num++) {
        CardImage img(get_card_at(card_num));
        img.draw_image_to_canvas(gc_p, row, next_col);
        next_col += spacing;
    }
}

void Hand::draw_hand_to_canvas(GraphicsCanvas *gc_p)
{
    int number_of_cards = num_cards();
    // short no card message name for convenience
    const string nocardmsg = NO_CARDS_IN_HAND_MESSAGE;

    int next_row = FIRST_CARD_ROW;

    if (number_of_cards > 0) {
        for (int card_num = 0;
             card_num < number_of_cards;
             card_num += CARDS_PER_DISPLAY_ROW) {
            draw_card_row(gc_p, card_num, next_row, FIRST_CARD_COL,
                          CARD_HORIZ_OFFSET);
            next_row += CARD_VERT_OFFSET;
        }
    } else {
        // No cards in the deck
        // Write message centered in the canvas
        gc_p->render_string(gc_p->height()/2,       // Middle row
                            (gc_p->width() - nocardmsg.length()) / 2, // center
                            nocardmsg);
    }
}

//
//  Print the deck using graphics
//
void Hand::graphic_print(std::string label)
{
    // Most of the parameters governing the shape and size of the canvas
    // were computed as constants in Hand.h, but the height depends on
    // the number of cards.

    unsigned int num_rows = divide_round_up(num_cards(),
                                            CARDS_PER_DISPLAY_ROW);

    unsigned int canvas_height =  (VERT_BORDER_OVERHEAD * 2) +
                                  (CARD_HEIGHT * num_rows) +
                                  CARD_VERT_SPACING * (num_rows - 1);

    GraphicsCanvas gc(canvas_height, CANVAS_WIDTH);

    gc.border(BORDER_CHAR);

    // Put the label over the border, centered with a space
    // on either side
    gc.render_string(0, (gc.width() - (label.length()) / 2) - 1,
                     string(" ") + label + string(" "));

    // Put the images of all cards in the canvas, arranged
    // into num_rows, each with CARDS_PER_DISPLAY_ROW cards.
    draw_hand_to_canvas(&gc);

    // Print the resulting canvas using cout
    gc.print_to_cout();
}
