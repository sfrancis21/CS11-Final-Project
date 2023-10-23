//****************************************************************
//
//                          racko.cpp
//
//     Implementation of a Racko game for CS 11
//
//     Author: Noah Mendelsohn
//
//     Tufts University, Fall 2021
//     CS 11: Project 2
//
//     Modified by: Sara Francis
//     Date: 12/5
//
//****************************************************************

#include <iostream>
#include <fstream>

#include "Card.h"
#include "Deck.h"
#include "RackoPlayer.h"

using namespace std;

const unsigned int NUMBER_OF_PLAYERS    = 2;
const unsigned int CARDS_IN_PLAYER_HAND = 5;
const string       WELCOME_MESSAGE      = "Let's play Racko!!\n";

const string       GRAPHICS_SWITCH      = "--graphics";   // for command line

//***************************************************************
//                   State of the Game
//***************************************************************

struct RackoGameState {
    bool graphics_on;
    string filename;
    Deck *game_deck_p;     // Input cards
    Deck *discard_pile_p;
    // Note the following is an array of pointers to RackoPlayer objects
    // ...so, each entry in the array is a pointer
    RackoPlayer *players[NUMBER_OF_PLAYERS];

    // ADD YOUR OWN DECLARATIONS BELOW FOR VARIABLES YOU USE
    // TO TRACK THE STATE OF THE GAME...IF YOU NEED ANY
};

//***************************************************************
//                     Function Prototypes
//***************************************************************

void initialize_game(RackoGameState *gs_p,
                     int argc, char *argv[]);
void parse_cmdline(RackoGameState *gs_p,
                     int argc, char *argv[]);
void initialize_card_decks(RackoGameState *gs_p);
void initialize_players(RackoGameState *gs_p);
void clean_up_game(RackoGameState *gs_p);
void play_the_game(RackoGameState *gs_p);
void delete_players(RackoGameState *gs_p);
void welcome_message();

//***************************************************************
//                          main
//
//      THIS FUNCTION IS COMPLETE. YOU SHOULD NOT HAVE
//      TO MODIFY IT.
//
//***************************************************************

int
main(int argc, char *argv[])
{
    // All data needed to play the game
    RackoGameState gs;

    //********************************************************
    // Initialize game deck, create and initialize discard pile &
    // player objects Note whether --graphics specfied on 
    //  command line
    //********************************************************

    // (note, we pass &gs rather than gs so that the functions
    // like initialize_game can update the game state)
    initialize_game(&gs, argc, argv);

    //********************************************************
    // Play the game
    //********************************************************

    welcome_message();
    play_the_game(&gs);

    //********************************************************
    // Clean up and exit
    //********************************************************

    clean_up_game(&gs);
    return 0;
}


//***************************************************************
//          Functions to Play the Game
//***************************************************************

//************************************************************
//
//                    welcome_message
//
//      THIS FUNCTION IS COMPLETE. YOU SHOULD NOT HAVE
//      TO MODIFY IT.
//
//************************************************************

void welcome_message()
{
    cout << WELCOME_MESSAGE << endl;
}

//************************************************************
//
//                    play_the_game
//
//      FOLLOW THE INSTRUCTIONS BELOW AND IN THE COMMENTS
//      TO COMPLETE THIS FUNCTION.
//
//      Play the whole game and report results
//
//
//    Here you need to fill in the logic that will cycle through all
//    the players, giving each turn at the game by calling the
//    RackoPlayer::play() method.
//
//    You will continue to loop, giving each player a turn in order
//    until one of three things happens:
//
//      1) Some player wins. In that case you must print a message
//         saying:
//
//              Congratulations <playername>, you've won!!
//
//         Followed by a newline.
//
//      2) While RackoPlayer::play() is interacting with the user, the
//         user may ask to quit the game. Indeed, play() usually
//         returns true, but it returns false if the player asks to
//         quit.  In this case you terminate the loop, and write the
//         following message:
//
//           User asked to stop the game. Leaving now.
//
//         followed by a newline.
//
//      3) Before calling RackoPlayer::play() each time you must
//         ensure that there are still cards in the game deck. If
//         there are not, you do not call play(), and instead you
//         write the message:
//
//            The game was stopped because we ran out of cards.
//
//         followed by a newline.
//
//    Once the loop has stopped for any of these three reasons and the
//    appropriate message printed, the play_the_game function returns.
//
//
//  Hints:
//
//      * Typically there are two players, but we give you a constant
//        named NUMBER_OF_PLAYERS, and if we were to recompile with some
//        bigger number then  your game should support that many.
//
//      * You may want to use a for loop with an integer loop variable.
//        that goes from 0 (index to the first entry in the players array)
//        to 1 (second entry), and so on until you've reached
//        NUMBER_OF_PLAYERS - 1.
//
//      * What's a little trickier is that you typically DON'T stop
//        when that limit has been reached. The loop stops >only<
//        for the three reasons described above. Once the last
//        player has played you cycle back the first.
//
//      * Challenge: how do you write a for loop that increments a player
//        number, but then cycles back to 0 just when it would have
//        reached NUMBER_OF_PLAYERS? Hint: the % (modulus) operator
//        is worth thinking about here!
//
//************************************************************

void play_the_game(RackoGameState *gs_p)
{
    // WRITE YOUR CODE HERE
    bool turn = true;
    while(turn) {
        //Check Deck Length
        if (gs_p->game_deck_p->number_of_cards() <= 0) {
            cout << "The game was stopped because we ran out of cards." << endl;
            return;
        }
        
        //Player Turns
        for (unsigned int i = 0; i < NUMBER_OF_PLAYERS; i++) {
            bool check;
            check = gs_p->players[i]->play();
            if (check == false) {
                cout << "User asked to stop the game. Leaving now." << endl;
                return;
            }
            //
            //Check for Win
            bool win;
            win = gs_p->players[i]->has_winning_hand();
            if (win == true) {
                string name;
                name = gs_p->players[i]->player_name();
                cout << "Congratulations " << name << ", you've won!!"
                << endl;
                return;
            }
        }    
    }
}


//***************************************************************
//             Initialization and cleanup functions
//***************************************************************

//************************************************************
//                parse_cmdline
//
//      THIS FUNCTION IS COMPLETE. YOU SHOULD NOT HAVE
//      TO MODIFY IT.
//
//************************************************************

void parse_cmdline(RackoGameState *gs_p,
                     int argc, char *argv[])
{
    // Graphics mode switch from command line
    if (argc == 3) {
        if (argv[1] == GRAPHICS_SWITCH) {
            gs_p->graphics_on = true;
        } else {
            cerr << "racko: invalid --graphics switch on command line: "
                 << argv[1] << endl;
            exit(1);
        }
        // If there was a --graphics switch, filename is 2nd argument
        gs_p->filename = argv[2];
    } else if (argc != 2) {
        cerr << "Usage: racko <filename> " << endl
             << "       racko --graphics <filename>" << endl;
            exit(1);
    } else {
        // argc == 2
        // There was no --graphics switch, filename is 1st argument
        gs_p->graphics_on = false;
        gs_p->filename = argv[1];
    }
}

//************************************************************
//                initialize_game
//
//      THIS FUNCTION IS COMPLETE. YOU SHOULD NOT HAVE
//      TO MODIFY IT UNLESS YOU WANT TO HAVE IT
//      USE DIFFERENT HELPER FUNCTIONS.
//
//************************************************************

void initialize_game(RackoGameState *gs_p,
                     int argc, char *argv[])
{
    parse_cmdline(gs_p, argc, argv);
    initialize_card_decks(gs_p);
    initialize_players(gs_p);
}

//************************************************************
//                  initialize_card_decks
//
//      FOLLOW THE INSTRUCTIONS BELOW AND IN THE COMMENTS
//      TO COMPLETE THIS FUNCTION.
//
//     Create the game deck and the discard pile, Both are of class
//     Deck.
//
//        * game_deck is initialized by reading the file in
//          gs_p->filename (parsed from the command line);
//
//        * dicard_pile is initialized by drawing the first
//          card from the game deck.
//
//  Results are put into gs_p-> game_deck_p and gs_p->dicard_pile_p.
//
//************************************************************

void initialize_card_decks(RackoGameState *gs_p)
{

    //
    //  Add code here to create a game deck and
    //  put the pointer to it into gs_p->game_deck_p
    //
    Deck *game_deck = new Deck(gs_p->filename);
    gs_p->game_deck_p = game_deck;

    //
    // Create a discard pile deck. Contents should be one
    // card drawn from the top of the game deck.
    //
    Card temp = game_deck->draw_top_card();
    Deck *discard_pile = new Deck(temp);
    gs_p->discard_pile_p = discard_pile;
}

//************************************************************
//                        initialize_players
//
//      Populate players array with RackoPlayer objects
//
//      FOLLOW THE INSTRUCTIONS BELOW AND IN THE COMMENTS
//      TO COMPLETE THIS FUNCTION.
//
//     Specific instructions:
//     ----------------------
//
//     * NUMBER_OF_PLAYERS is a constant giving the number of
//       players to be created.
//
//     * For each player:
//
//        - write a prompt in the following format (no quotes, and
//          no newline at the end):
//
//              "Enter the name for player 1: "
//
//          etc. The first player is #1, not 0.
//
//        - Use cin to read the name of the player into
//          a single string variable
//
//        - Starting with subscript 0 in the players array, create a
//          new RackoPlayer object for that named person, and put a
//          pointer to that object into the players array in the game
//          state. Thus the pointer to the RackoPlayer object for the
//          player listed as #1 in the prompt above goes into
//          players[0], etc.
//
//************************************************************

void initialize_players(RackoGameState *gs_p)
{
    // WRITE YOUR CODE HERE
    for(unsigned int i = 0; i < NUMBER_OF_PLAYERS; i++) {
        string name;
        int n = i + 1;
        cout << "Enter the name for player " << n << ": ";
        cin >> name;
        RackoPlayer *player = new RackoPlayer(
            name, CARDS_IN_PLAYER_HAND, gs_p->game_deck_p, gs_p->discard_pile_p,
            gs_p->graphics_on);
        gs_p->players[i] = player;
    }
}

//************************************************************
//                       clean_up_game
//
//      FOLLOW THE INSTRUCTIONS BELOW AND IN THE COMMENTS
//      TO COMPLETE THIS FUNCTION.
//
//      Return to the heap any objects that were allocated by
//      initialize_game (or by functions called by initialize_game).
//
//      (Hint: you can use valgrind to see whether you have correctly
//      deleted everything that should be deleted.)
//
//************************************************************

void clean_up_game(RackoGameState *gs_p)
{
    // WRITE YOUR CODE HERE
    delete gs_p->game_deck_p;
    delete gs_p->discard_pile_p;
    for (unsigned int i = 0; i < NUMBER_OF_PLAYERS; i++) {
        delete gs_p->players[i];
    }
}
