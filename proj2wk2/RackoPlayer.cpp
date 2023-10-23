//****************************************************************
//
//                         RackoPlayer.cpp
//
//     Methods to support the RackoPlayer class, which represents
//     the state of the game for and handles interaction with a 
//     Racko player.
//
//     Author: Noah Mendelsohn
//
//     Tufts University, Fall 2021
//     CS 11: Project 2
//
//     Modified by: Sara Francis
//     Date: 12/1
//
//****************************************************************

#include <iostream>
#include <cstdlib>   // for system call to clear the screen

#include "Deck.h"
#include "Hand.h"
#include "RackoPlayer.h"

#include "CardImage.h"

using namespace std;

//************************************************************************
//
//                            Constructor
//
//   * Records the game deck and discard pile pointers, and player's name
//   * Draws cards from the game deck to fill initial hand for the player
//   * Checks for the odd case that the hand wins immediately, in which
//     case we exit the whole came and declare success NEEDSWORK: is
//     that what we want?
//
//  Remember: the member variables we set here are available to all
//  the member functions of this class. This becomes the shared state of
//  what we know about this player, and also allows us to easily find
//  things like the game deck that are needed to play the game.
//
//  NOTE TO STUDENTS: WE WROTE MOST OF THIS FOR YOU, BUT THERE IS
//  ONE IMPORTANT PIECE OF THE CODE YOU HAVE TO ADD...SEE COMMENTS
//  IN THE CODE BELOW.
//
//************************************************************************

RackoPlayer::RackoPlayer(std::string player_name, unsigned int num_cards,
                           Deck *game_deck_arg, Deck *discard_pile_arg,
                           bool graphics_on)
{
    // Make local copies of pointers to the game deck, the discard pile, etc.
    // Note the player's name, and whether graphics is on
    m_game_deck_p    = game_deck_arg;
    m_discard_pile_p = discard_pile_arg;

    m_player_name = player_name;
    m_graphics_on = graphics_on;

    // Initialize the player's hand by adding the appropriate
    // number of cards to m_hand by drawing from the game deck.
    // So that everyone plays the same way and we can test the results,
    // the cards are always added in order, with the first card drawn
    // winding up first in the hand.

    // DELETE THE FOLLOWING PRINT STATEMENT AND VOID CASTS AND REPLACE
    // THEM WITH YOUR CODE TO PUT CARDS FROM THE GAME DECK INTO
    // m_hand. MAKE SURE YOU PUT IN THE RIGHT NUMBER OF CARDS AND
    // CHECK VERY CAREFULLY TO MAKE SURE THEY ARE IN THE RIGHT ORDER!
    // (You will lose a lot of credit if either the count or the order
    // is wrong, as the game results will surely be different from the
    // correct ones!)
    
    for (unsigned int i = 0; i < num_cards; i++) {
        Card temp = m_game_deck_p->draw_top_card();
        m_hand.add_card_at(i, temp);
    }

    // Check for super-unlikely case that initial hand is a winner.
    // Just give up)
    if (has_winning_hand()) {
        cout << "AMAZING!! Player " << player_name
             << " drew a winning hand at the start of the game!" << endl;
        cout << "The game is over before it starts." << endl;
        exit(0);
    }
}

//************************************************************************
//                            Destructor
//************************************************************************

RackoPlayer::~RackoPlayer()
{
    // No explicit destruction needed for this class.
    // The automatic member destruction will clean up the deck object
}

//************************************************************************
//                             player_name
//************************************************************************

std::string RackoPlayer::player_name()
{
    return m_player_name;
}

//************************************************************************
//                               play
//
//     This handles a turn for the player, including both user interaction
//     and updating the state of the game. It does NOT check whether
//     the result is a winning hand.
//
//     What you must do and how to get it right
//     ----------------------------------------
//
//     This function and the private helper functions it calls embody
//     all the logic for one player's turn...FOR FULL CREDIT YOU WILL
//     NEED TO GET THIS FUNCTION TO EXACTLY MATCH, IN ALL CASES, THE
//     SPECIFICATIONS WE GIVE YOU IN THE INSTRUCTIONS.
//
//     TO MAKE DOING THIS EASIER, WE GIVE YOU A RUNNABLE REFERENCE
//     IMPLEMENTATION OF THE racko EXECUTABLE THAT WE BELIEVE TO BE
//     CORRECT. Anytime you are not sure what this code should do, use
//     the reference executable we give you to play with the same
//     input card deck, and of course the same choices by each
//     users. IF YOUR OUTPUT MATCHES WHAT THE CORRECT EXECUTABLE DOES
//     IN ALL CASES, YOU WILL RECEIVE FULL CREDIT.
//
//     Overview of Function Logic
//     --------------------------
//
//     As noted above, you will want to read the instructions, and
//     match the reference exectuable exactly. Still, it's handy to
//     have a summary of roughly what this function must do, so here's
//     a quick synopsis:
//
//      Of course, in all cases the exact specifications for prompting
//     text, error handling etc., but roughly what you must do for
//     each call to play() is:
//
//
//     * Tell the player it's their turn. We urge you to use the
//       its_your_turn function that we've written for you, as it
//       gets the formatting exactly right (and clears the screen
//       first in graphics mode.)
//
//     * Show them what's in their hand. Run the sample executable
//       to see exactly what this should look like.
//
//     * Ask them whether they want to draw from the game deck, or the
//       discard pile, or whether they prefer to quit the whole game.
//        Again, the sample executable shows how this should look.
//
//     * If the user says to quit, just return false immediately. That's the
//       signal to the caller of play() that quit was requested.
//
//     * ONLY WHEN YOU GET TO HANDLING GRAPHICS: add a step here to
//       print the user's hand again (Why? In graphics mode things tend
//       to scroll off the screen and the user will want to see their
//       hand before makign a choice. In normal mode only a few lines
//       are written and the hand is usually still visible.) DON'T WORRY
//       ABOUT THIS UNLESS YOU GET TO DOING GRAPHICS.
//
//     * Depending on whether the user selected game deck (g) or discard
//       pile (d), draw a new card from the appropriate deck.
//
//     * Ask the user which card they would like to discard, and do
//       it. They can either choose the value of a card already in
//       their hand, in which case you must take that card out of
//       their hand and put it on the discard pile, and put the new
//       card in their hand in place of the old.  Or, the user may
//       choose to discard the new card (even if it just came off the
//       discard pile...put the new_card on the discard pile. In that
//       case, no changes are needed to the user's hand.
//
//     * Return true from play(), because the user did not ask to quit,
//       and play() has successfully played the users' turn.
//
//     Helper Functions
//     ----------------
//
//     You will want to use a lot of helper functions: if you don't
//     your code will be very disorganized, and way over 30 lines.
//
//     Throughout the remainder of this source file we give you a
//     variety of private helper functions we wrote that we think may
//     be useful. Some of them are fully written. Some you would have
//     to complete.
//
//     IT'S NOT REQUIRED THAT YOU USE THESE HELPER FUNCTIONS IF YOU
//     PREFER TO ORGANIZE YOUR IMPLEMENTATION DIFFERENTLY. OBVIOUSLY,
//     IF YOU DO USE THEM, YOU WILL HAVE TO FILL IN ANY MISSING
//     CODE. Our solution does use these and it does work, but many
//     other approaches are possible. Have fun! In all cases, make
//     sure your output exactly matches the sample executable.
//
//     HOWEVER: What's most important is that your play() function work
//     properly. That means it must do exactly what the instructions say,
//     and it must match what the reference executable we give you does.
//     Making a mess of your helper functions is not an excuse for having
//     play() do the wrong thing!
//
//************************************************************************


bool RackoPlayer::play()
{
    // REPLACE THE FOLLOWING TEMPORARY PRINT STATEMENT
    // WITH YOUR LOGIC TO IMPLEMENT play();
    its_your_turn();
    print_hand();
    show_top_discard_card();
    
    char choice;
    choice = discard_gamedeck_or_quit();
    if (choice == 'q') {
        return false;
    } else {
        Card temp;
        temp = get_new_card(choice);
        replace_card_in_hand(temp);
        return true;
    }
}


//************************************************************************
//                            its_your_turn
//
//   Called to announce start of a player's turn (or end game in
//   unlikely case that game deck is empty).
//
//************************************************************************

void RackoPlayer::its_your_turn()
{
    // Tell new player it's their turn
    // Start each player's display at the top of the terminal
    //
    // (The clear_screen() call is useful in graphics mode so
    // that each player's turn starts at the top of their display.
    // Just leave it in, and don't worry about it unless you get
    // to trying graphics mode).
    clear_screen();
    cout << "========================================" << endl;
    cout << "         Player " << m_player_name << " it's your turn!" << endl;
    cout << "========================================" << endl << endl;


    // Bail out if there are no cards in the deck
    if (m_game_deck_p->number_of_cards() <= 0) {
        cerr << "Player " << m_player_name
             << " cannot take turn because game deck is empty" << endl;
        exit(1);
    }
}

//************************************************************************
//                         has_winning_hand
//
//    Return true if player is holding a winning hand, otherwise
//    false. Remember, a hand wins only if all the cards are in order
//    from low to high.
//
//************************************************************************

bool RackoPlayer::has_winning_hand()
{
    // REPLACE THE FOLLOWING TEMPORARY PRINT STATEMENT WITH CODE THAT
    // RETURNS TRUE IF THE CARDS IN M_HAND REPRESENT A WINNING RACKO HAND.
    int length = m_hand.num_cards();
    Card temp;
    Card check;
    int count = 0;
    for (int i = 0; i < length - 1; i++) {
        temp = m_hand.get_card_at(i);
        check = m_hand.get_card_at(i + 1);
        if (temp.value() < check.value()) {
            count = count + 1;
        }
    }
    if (count == length - 1) {
        return true;
    } else {
        return false;
    }
}


//************************************************************************
//                    show_top_discard_card
//
//   If not in graphics mode, then write a message that
//   says:
//
//     The top card on the discard pile is N
//
//   followed by a newline (endl), where N is the face
//   value of the top card on the discard pile.
//   
//   If in graphics mode, show the image of the card
//   with the label DISCARD_GRAPHIC_LABEL.
//
//************************************************************************

void RackoPlayer::show_top_discard_card()
{
    if (m_graphics_on) {
        CardImage img(m_discard_pile_p->inspect_top_card());
        img.graphic_print(DISCARD_GRAPHIC_LABEL);
    } else {
        cout << "The top card on the discard pile is: "
             << m_discard_pile_p->inspect_top_card().value()
             << endl;
    }
}


//************************************************************************
//                      discard_gamedeck_or_quit
//
//   Repeatedly ask the user to enter a character representing a choice
//   of:
//           g - draw from game deck
//           d - draw from game discard pile
//           q - quit the game
//
//   Once the user enters a char consisting of one of the three single
//   characters above (lowercase only), return that character. Otherwise,
//   prompt again until the user enters one of the three.
//
//   The prompt must be:
//      "Would you like to draw from the game
//       deck (g) discard pile (d) or quit (q)?: "
//
//   (all on one line, with no quotes. The constant CHOICE_PROMPT
//   below has the right string, so:
//
//                 cout << CHOICE_PROMPT;   // No endl
//
//   will print exactly the right thing.
//
//************************************************************************

const string CHOICE_PROMPT = "Would you like to draw from the game "
                             "deck (g) discard pile (d) or quit (q)?: ";

char RackoPlayer::discard_gamedeck_or_quit()
{
    char choice;
    cout << CHOICE_PROMPT;
    cin >> choice;

    while ((choice != 'g') and (choice != 'd') and (choice != 'q')) {
        cout << CHOICE_PROMPT;
        cin >> choice;
    }

    return choice;
}

//************************************************************************
//                          get_new_card
//
//   Get the new card from either the game deck or the discard pile depending
//   on whether "choice" parameter is 'g' or 'd'.
//
//   Display the chosen card.
//
//   Input:
//          choice - A character which is known to be either 'd' or 'g'
//                   (you do not have to check and you must not
//                   do or print anything if it's not)
//
//************************************************************************

Card RackoPlayer::get_new_card(char choice)
{
    Card temp;
    if (choice == 'd') {
        temp = m_discard_pile_p->draw_top_card();
        int c = temp.value();
        cout << "You drew card " << c << " from the discard pile." << endl;
    } else if (choice == 'g') {
        temp = m_game_deck_p->draw_top_card();
        int c = temp.value();
        cout << "You drew card " << c << " from the game deck." << endl;
    }
    
    return temp;
}

//************************************************************************
//                          replace_card_in_hand
//
//   When this function is called, a new card has already been chosen.
//   The user now gets to decide which card to put on the discard pile.
//
//   Input:
//          new_card - A Card which may be placed in the m_hand
//
//   WRITE CODE TO LOOP REPEATING THE FOLLOWING STEPS UNTIL A VALID
//   CHOICE IS MADE:
//
//   * WE GIVE YOU THIS STEP: Ask the user which card to put on
//     the discard pile. Again, we give you a string constant with
//     the right prompt, so you can ask the question with the statement
//     (do not print a newline):
//
//                    cout << REPLACE_CARD_PROMPT;
//
//   * WE GIVE YOU THIS STEP: use the supplied "read_int_from_cin"
//     function to ask the user which number to drop. The reason we
//     give this to you is that although the user is suppoed to enter
//     a number, users often mistakenly enter something else (like q
//     for quit), and C++ makes recovery from that quite tricky. So we
//     do it for you. Just call the function using the code we provide
//     below that says:
//
//                  value_to_drop = read_int_from_cin();
//
//      You can assume this does the same as:
//
//                  cin >> value_to_drop;
//
//      except that it asks the user to retry if something other than
//      a number is entered
//
//   * YOU MUST CODE THIS LOGIC: Now you must deal with two cases:
//
//       1) If the selected number matches the value of new_card,
//          then the user is asking to discard the newly
//          picked-up card. Just put it on the discard pile
//          (and yes, it may have just come from the discard pile.)
//
//       2) This case is a little harder. The user claims
//          the number is in their hand, but we have to check.
//          If the "value_to_drop" does indeed match a card in the
//          hand, replace the card in the hand.
//
//   * WE GIVE YOU THE LOOP LOGIC: If the user has made an invalid
//     choice, that is if the user has chosen a number that is not the
//     new card and is not already in the hand, repeat the entire
//     loop, asking for another value_to_drop, etc.
//
//
//************************************************************************

const string REPLACE_CARD_PROMPT = "What is the face value of the card"
                                   " you would like to discard? ";

void RackoPlayer::replace_card_in_hand(Card new_card)
{
    int value_to_drop;

    // YOUR CODE MUST SET discarded_a_card TO TRUE ONCE THE
    // USER SELECTS A VALID CARD NUMBER AND YOU HAVE ACTUALLY
    // DISCARDED A CARD...THIS WILL CAUSE THE LOOP TO TERMINATE.
    bool discarded_a_card = false;

    // LOOP until the card chosen is one that's in the hand and discard it
    do {
        // Ask the user which card they would like to drop
        cout << REPLACE_CARD_PROMPT;

        //
        // Read a number. If user enters something other than an integer
        // then cin will go to the fail state. In that case, we clear the
        // error, read the bad input the user gave for the integer

        value_to_drop = read_int_from_cin();
        //Checks if want to discard the one just pulled
        int card_num = new_card.value();
        if (card_num == value_to_drop) {
            m_discard_pile_p->put_card_on_top(new_card);
            discarded_a_card = true;
        } else {
            //Want to discard from hand
            unsigned int length = m_hand.num_cards();
            Card temp;
            int card_value;
            for (unsigned int i = 0; i < length; i++) {
                temp = m_hand.get_card_at(i);
                card_value = temp.value();
                if (card_value == value_to_drop) {
                    Card c = m_hand.get_card_at(i);
                    m_discard_pile_p->put_card_on_top(c);
                    m_hand.remove_card_at(i);
                    m_hand.add_card_at(i, new_card);
                    discarded_a_card = true;
                }
            }
        }
        // WRITE CODE HERE TO DISCARD THE APPROPRIATE CARD,
        // REPLACING THE SELECTED CARD IN THE USER'S HAND
        // IF NECESSARY.
        //
        // If the user makes a valid choice (a card in the
        // hand or new_card), then set discarded_a_card to
        // true so the loop will terminate.
        //
        // If an invalid choice is made, then leave
        // discarded_a_card with it's current value,
        // which is false.
        //
        // If there was bad input, then we tell the user
        // before asking again.
        //
        if (not discarded_a_card) {
            cout << value_to_drop
                 << " was not the value of the drawn card and was not in "
                 << "your hand. Try again..." << endl;
        }
    } while (not discarded_a_card);
}

//************************************************************************
//
//                        read_int_from_cin
//
//  Mostly this is easy to do, but things get tricky if a user
//  enters something other than a number (like q for quit, which is
//  an easy mistake to make.) Recovering this in C++ is a little tricky,
//  as you have to reset an error marker on cin (using clear) and reread
//  to pull out the unread text (e.g. q).
//
//  SINCE THIS IS TRICKY AND NOT VERY INTERESTING, WE GIVE YOU THIS CODE
//  YOU CAN TRUST THAT IT WORKS.
//
//************************************************************************

int RackoPlayer::read_int_from_cin()
{
    int num;
    bool done = false;
    while (not done) {
       cin >> num;
       if (cin.fail()) {
           string scrap;
           cin.clear();     // clear the error state
           cin >> scrap;    // read the bad input
           cout << "Sorry, you should have entered a number but you entered: "
                << scrap << "." << endl;
           cout << "Please try again...: ";
       } else {
          done = true;
       }
    }

    return num;
}


//************************************************************************
//
//                         print_hand
//
//   At various points in the game you will want to show the player
//   the contents of their hand. Exactly how that's done depends on
//   whether we're in graphics mode or not.
//
//   Here's what this function does:
//
//     * If NOT in graphics mode it prints a message like:
//
//               Mark's hand: 3, 5 12, 13, 2
//
//     * If in graphics mode it uses Hand.graphi_print to print
//       a lovely looking visual display of the hand, with a nice
//       border etc.
//
//   SO YOU WON'T HAVE TO WORRY ABOUT GRAPHICS MODE, WE GIVE YOU ALL
//   THE CODE YOU NEED HERE FOR print_hand, BUT...
//
//   WHEN IN THE USUAL NON-GRAPHIC MODE, THIS CALLS Hand::print() AND
//   YOU >DO< HAVE SOME CODE TO COMPLETE FOR THAT. SO, IF YOU WANT
//   THIS FUNCTION TO WORK PROPERLY, MAKE SURE THAT Hand::print() is
//   properly coded.
//
//************************************************************************

void RackoPlayer::print_hand()
{
    if (m_graphics_on) {
        // On top of graphic display put label like Mary's Hand
        m_hand.graphic_print(player_name() + "'s" + HAND_GRAPHIC_SUFFIX);
    } else {
        cout << player_name() << "'s hand: ";
        m_hand.print();
    }
    cout << endl;
}


//************************************************************************
//
//                           debug_print
//
//   The assumption is that most printing will be handled by users of
//   this class, but being able to print in a standard way when
//   debugging is handy.
//
//************************************************************************

void RackoPlayer::debug_print()
{
    cout << endl;
    cout << "RackoPlayer::debug_print for player " << m_player_name << endl;
    cout << "Graphics mode is: ";
    if (m_graphics_on) {
        cout << "ON" << endl;
    } else {
        cout << "OFF" << endl;
    };
    cout << "Player's hand is: " << endl;
    m_hand.debug_print();
}


//************************************************************************
//                        clear_screen (GRAPHICS MODE ONLY)
//
//     Clears the user's terminal so next cout prints on top line
//     of display.
//
//     (Usable, for example, to make sure each user's turn is played on a clear
//     screen).
//
//     DO NOT WORRY ABOUT THIS UNLESS YOU GET TO TRYING GRAPHICS MODE,
//     EVEN THEN, YOU CAN ASSUME THIS WORKS RIGHT, AND WE CALL IT
//     FOR YOU IN THE RIGHT PLACE IN THE its_your_turn FUNCTION.
//
//************************************************************************

void
RackoPlayer::clear_screen()
{
    if (m_graphics_on) {
        system("clear");
    }
}
