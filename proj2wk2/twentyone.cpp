//
//                          twentyone.cpp
//                      Author: Noah Mendelsohn
//
//     Tufts University, Fall 2021
//     CS 11: Project 2
//
//     Implementation of a Twentyone game for CS 11 to be used as a
//     testing main for Project 2 Week 1 classes.
//
//     Note: There's no need to change anything in this file.
//

#include <iostream>
#include <fstream>
#include "Card.h"
#include "CardLinkedList.h"
#include "Deck.h"

using namespace std;

const string WELCOME_MESSAGE      = "Let's play Twenty One!!";
const string LOSER_MESSAGE        = "Sorry, YOU LOSE!!";
const string OUT_OF_CARDS_MESSAGE = "The game deck ran out of \
                                     cards. Game ending.";

const int MAX_LEGAL_HAND = 21;    // Hands worth more than this lose

Deck *initialize_game_deck(string filename);
bool should_we_draw_card();

//***************************************************************
//                          main
//***************************************************************

int main(int argc, char *argv[])
{
    if (argc != 2) {
        cerr << "ERROR: Expected 2 command line arguments, "
             << "but you provided " << argc << " instead" << endl
             << "Usage: ./twentyone [filename]" << endl;
        exit(1);
    }

    Deck *game_deck;
    string filename = argv[1];
    game_deck = initialize_game_deck(filename);

    bool game_lost = false;
    bool player_quit = false;
    int sum = 0;
    // Discard pile starts with one card drawn from top of game deck

    //-------------------------------------------------------
    // Play the game
    //-------------------------------------------------------

    cout << WELCOME_MESSAGE << endl;

    // Each time around one player gets to play. We rotate through
    // players until someone wins or game deck is empty
    while ((game_deck -> number_of_cards() > 0) and
           (not player_quit) and (not game_lost)) {
        if (game_deck -> number_of_cards() > 0) {
            player_quit = not should_we_draw_card();
            if (not player_quit) {
                Card new_card(game_deck->draw_top_card());
                sum += new_card.value();
                cout << "You drew " << new_card.value() 
                     << " and your new sum is " << sum << endl;
            }
            if (sum > MAX_LEGAL_HAND) {
                game_lost = true;
                cout << LOSER_MESSAGE << endl;
            }
        } else {
            cout << OUT_OF_CARDS_MESSAGE << endl;
        }
    }

    // Player chooses to quit. Report if the next card would've
    // been good or bad for your hand.
    if (player_quit and (game_deck -> number_of_cards() > 0)) {
        Card new_card(game_deck->draw_top_card());
        if (sum + new_card.value() < MAX_LEGAL_HAND) {
            cout << "You quit too early, the next card was a " 
                 << new_card.value() << endl;
        } else {
            cout << "You quit just in time, the next card was a " 
                 << new_card.value() << endl;
        }
    }

    delete game_deck;

    return 0;
}

//
// Get input from the user on whether or not they 
// would like to draw a card.
//
bool should_we_draw_card()
{
    char choice;
    do {
        cout << endl 
             << "Would you like to draw a card from the "
             << "game deck (c) quit (q)?: ";
        cin >> choice;
    } while ((choice != 'c' ) and (choice != 'q'));
    //  choice is either c or q
    return (choice == 'c');
}

//
// Initialize the game Deck
//
Deck *initialize_game_deck(string filename)
{
    Deck *game_deck = new Deck(filename);

    // Clean up and return
    return game_deck;
}
