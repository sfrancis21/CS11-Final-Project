#include <iostream>
#include "Card.h"
#include "CardLinkedList.h"
#include "Deck.h"
using namespace std;

int main()
{
    Deck *dd = new Deck("shuffled_deck_large.txt");
    Card temp2 = dd->draw_top_card();
    Card temp = dd->inspect_top_card();
    Card c20(20);
    temp.debug_print();
    dd->put_card_on_top(20);
    dd->debug_print();
    return 0;
}