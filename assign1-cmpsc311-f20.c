////////////////////////////////////////////////////////////////////////////////
//
//  File           : cmpsc311-f16-assign1.c
//  Description    : This is the main source code for for the first assignment
//                   of CMPSC311 at Penn State University.  See the related
//                   assignment page for details.
//
//   Author        : Benjamin Gutierrez
//   Last Modified : 9/12/20
//

// Include Files
#include <stdio.h>
#include <cmpsc311_util.h>
#include <time.h>

// Defines
#define NUM_CARDS 52
#define MAX_CARDS 11
#define SPADE   "\xE2\x99\xA0"
#define CLUB    "\xE2\x99\xA3"
#define HEART   "\xE2\x99\xA5"
#define DIAMOND "\xE2\x99\xA6"

// Functions

////////////////////////////////////////////////////////////////////////////////
//
// Function     : print_card
// Description  : print the card from the integer value
//
// Inputs       : card - the card to print
// Outputs      : 0 always

int print_card( int card ) {

    // CODE HERE
    char card_faces[] = "234567891JQKA";
    char *card_suits[] = { SPADE, CLUB, HEART, DIAMOND };
    int suit = card / 13, cardty = card % 13;
    if ( cardty == 8 ) {
        printf( "10%s", card_suits[suit] );
    } else {
        printf( "%c%s", card_faces[cardty], card_suits[suit] );
    }

    // Return zero
    return( 0 );
}

////////////////////////////////////////////////////////////////////////////////
//
// Function     : print_cards
// Description  : print a number of cards (no more than 13 on one line)
//
// Inputs       : cards - the array of cards to print
//                num_cards - the number of cards to print
// Outputs      : 0 always

int print_cards( int cards[], int num_cards ) {
    for ( int i = 0; i < num_cards; i++ ) {
        // print one card at a time
        print_card( cards[i] );
        printf(" ");
        // go to a new line if 13 cards are already printed
        if( ( i % 13 ) == 12 ){
            printf("\n");
        }
    }
    printf("\n");
    // Return zero
    return( 0 );
}

////////////////////////////////////////////////////////////////////////////////
//
// Function     : swap_cards
// Description  : swap the two given cards
//
// Inputs       : two card addresses
// Outputs      : 0 always

int swap_cards( int *card1, int *card2 ) {
    int temp = *card1;
    *card1 = *card2;
    *card2 = temp;
    // Return zero
    return( 0 );
}
////////////////////////////////////////////////////////////////////////////////
//
// Function     : shuffle_cards
// Description  : print a number of cards (no more than 13 on one line)
//
// Inputs       : cards - the array of cards to print
//                num_cards - the number of cards to print
// Outputs      : 0 always

int shuffle_cards( int cards[], int num_cards ) {
    int randomIndex;
    for ( int i = num_cards - 1; i > 0; i-- ) {
        // Get a random card
        randomIndex = getRandomValue( 0, i - 1 );
        // Swap the cards
        swap_cards( &cards[i], &cards[randomIndex] );
    }
    // Return zero
    return( 0 );
}
////////////////////////////////////////////////////////////////////////////////
//
// Function     : hand_value
// Description  : return the value of the hand
//
// Inputs       : cards - the array of cards in the hand
//                num_cards - the number of cards in the hand
// Outputs      : An integer representing the value of the hand

int hand_value( int cards[], int num_cards ) {
    // possible values for all card types (ace value is 1 initially)
    int card_values[] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 10, 10, 10, 1};
    int total_value = 0;
    int index;
    int ace_count = 0;
    // loop through all the cards in hand
    for( int i = 0; i < num_cards; i++) {
        index = cards[i] % 13;
        // check if the card is an ace and keep a counter
        if ( index == 12 ) {
            ace_count = ace_count + 1;
        }
        // calculate a running total hand value
        total_value = total_value + card_values[index];
    }
    // if there is an ace, check if it is possible to make its value 11 instead of 1
    if ( ace_count > 0 && (total_value + 10 <= 21)) {
        total_value = total_value + 10;
    }
    // return hand value
    return total_value;
}
////////////////////////////////////////////////////////////////////////////////
//
// Function     : sort_cards
// Description  : sort a collection of cards
//
// Inputs       : hand - the cards to sort
//                num_cards - the number of cards in the hand
// Outputs      : 0 always

int sort_cards( int hand[], int num_cards ) {
    // this is a sort using the Bubble sort algorithm
    int counter;  // the counter will keep track of the number swaps per round
    int face1, face2, suit1, suit2; // characteristics of the two cards being compared
    for (int round = 0; round < num_cards - 1; round++ ) {
        counter = 0;
        for (int i = 0; i < num_cards - 1; i++) {
            // Compare cards in index i and index i + 1
            face1 = hand[i] % 13;
            face2 = hand[i+1] % 13;
            // first compare the face values
            if ( face1 > face2 ) {
                swap_cards( &hand[i], &hand[i+1] );
                counter++;
            }
            else if ( face1 == face2 ) {
                suit1 = hand[i] / 13;
                suit2 = hand[i+1] / 13;
                // compare the suits only if the cards have the same face value
                if ( suit1 > suit2) {
                    swap_cards( &hand[i], &hand[i+1] );
                    counter++;
                }
            }
        }
        // only stop when no cards were swapped
        if (counter == 0){
            break;
        }
    }

    // Return zero
    return( 0 );
}

////////////////////////////////////////////////////////////////////////////////
//
// Function     : dealer_play
// Description  : dealer decision to hit or stand (hit on anything less 
//                than 17)
//
// Inputs       : hand - cards dealer has
//                num_cards - the number of cards in player hand
// Outputs      : 0 = stand, 1 = hit

int dealer_play( int hand[], int num_cards ) {
    int value = hand_value( hand, num_cards );
    if ( value < 17){
        return( 1 );
    }
    else {
        return( 0 );
    }
}

////////////////////////////////////////////////////////////////////////////////
//
// Function     : player_play
// Description  : player decision to hit or stand
//
// Inputs       : hand - cards player has
//                num_cards - the number of cards in player hand
//                dealer_card - the dealers face up card
// Outputs      : 0 = stand, 1 = hit

int player_play( int hand[], int num_cards, int dealer_card ) {
    // The strategy is to stand if hand greater than or equal to 17
    // And hit if it's less than or equal to 14
    // For all other hand values(15-16),
    // Hit if the dealer has a card value 7 or greater
    // Otherwise stand
    int value = hand_value( hand, num_cards );
    if ( value >= 17){
        return( 0 );
    }
    else if ( value <= 14 ){
        return( 1 );
    }
    else {
        if ( dealer_card >= 7){
            return( 1 );
        }
        else{
            return( 0 );
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
//
// Function     : play_hand
// Description  : play a hand of black jack
//
// Inputs       : deck - the whole deck of cards
//                num_cards - the number of cards in the deck
//                player_money - pointer to player amount of money
// Outputs      : 1 if player wins, 0 if dealer wins

int play_hand( int deck[], int num_cards, float *player_money ) {
    int player_hand[MAX_CARDS];
    int dealer_hand[MAX_CARDS];
    // the deck index points to the top card of the deck
    int deck_index = 0;
    // indexes always point to the next available spot in the array
    int player_index = 0;
    int dealer_index = 0;
    int player_value, dealer_value;

    // First shuffle the cards
    shuffle_cards( deck, num_cards );
    printf("---- New hand -----\n\n");

    // Deal two cards to the player and the dealer
    player_hand[0] = deck[0];
    dealer_hand[0] = deck[1];
    player_hand[1] = deck[2];
    dealer_hand[1] = deck[3];
    // update the deck index with the next unused card
    deck_index = 4;
    // update the indexes with the next available spots
    player_index = 2;
    dealer_index = 2;

    // show cards
    printf("Dealer cards: ");
    print_card( dealer_hand[0] );
    printf(" XX\n\n");
    printf("Player cards: ");
    print_cards( player_hand, player_index );

    // Check if player has 21
    player_value = hand_value( player_hand, player_index );
    if ( player_value == 21 ) {
        *player_money = *player_money + 7.5;
        printf( "Player has Blackjack!, wins $7.50\n\n" );
        return(1);
    }

    // calculate the value of the dealer's first card
    int first_card = hand_value( dealer_hand, 1 );

    // Player's Turn. Player will continue playing until they stand or bust
    while( player_play( player_hand, player_index, first_card ) != 0 ){
        // Hit: give the player the next card in the deck
        player_hand[player_index] = deck[deck_index];
        // update the player index with the next available spot
        player_index = player_index + 1;
        deck_index = deck_index + 1;
        player_value = hand_value( player_hand, player_index );
        printf( "Player hit (%d): ", player_value );
        print_cards( player_hand, player_index );
        // Check if the player busted
        if ( player_value > 21 ) {
            printf( "Player BUSTS ... dealer wins!\n\n" );
            *player_money = *player_money - 5.0;
            return ( 0 );
        }
    }
    printf("Player stands (%d): ", player_value);
    print_cards( player_hand, player_index );
    printf("\n");
    
    // Dealer's Turn. Dealer will continue playing until they stand or bust
    dealer_value = hand_value( dealer_hand, dealer_index );
    while( dealer_play( dealer_hand, dealer_index ) != 0 ){
        // Hit: give the dealer the next card in the deck
        dealer_hand[dealer_index] = deck[deck_index];
        // update the dealer index with the next available spot
        dealer_index = dealer_index + 1;
        deck_index = deck_index + 1;
        dealer_value = hand_value(dealer_hand, dealer_index);
        printf( "Dealer hit (%d): ",dealer_value );
        print_cards( dealer_hand, dealer_index );
        // Check if the dealer busted
        if ( dealer_value > 21 ) {
            printf( "Dealer BUSTS ... player wins!\n\n" );
            *player_money = *player_money + 5.0;
            return ( 1 );
        }
    }
    printf("Dealer stands (%d): ", dealer_value);
    print_cards( dealer_hand, dealer_index );
    printf("\n");

    // compare dealer and player hands
    if ( player_value > dealer_value ){
        printf("Player wins!!!\n\n");
        *player_money = *player_money + 5.0;
        return( 1 );
    }
    else{
        printf("Dealer wins!!!\n\n");
        *player_money = *player_money - 5.0;
        return ( 0 );
    }
}

////////////////////////////////////////////////////////////////////////////////
//
// Function     : show_player_money_histogram
// Description  : Display a histogram of the player funds by hands
//
// Inputs       : money_rounds - the list of player money by hand number
//                last_round - the hand number where the game stopped
// Outputs      : 0 always

int show_player_money_histogram( float money_rounds[], int last_round ) {
    // $200 is the max value shown in the histogram
    int row_value = 200;
    // Print Title
    for( int i = 0; i < 45; i++ ){
        printf(" ");
    }
    printf("Player Cash by Round\n");
    // Print Top line
    printf("     ");
    for( int i = 0; i < 100; i++ ){
        printf("-");
    }
    printf("\n");

    // Print histogram
    // Print a row every $5
    for( int row = 0; row <= 40; row++ ){
        row_value = 200 - row * 5;
        printf("%3d |", row_value);
        // Each column represents a round
        for (int column = 0; column < 100; column++){
            // Check the last round because the game could have ended before 100 rounds
            if( column < last_round ){
                // Print X if the player's money is greater than or equal to the value represented in the row
                if (row_value > money_rounds[column]){
                    printf(".");
                }
                else{
                    printf("X");
                }
            }
            else{
                printf(".");
            }
        }
        printf("|\n");
    }


    //Print Bottom Line
    printf("     ");
    for( int i = 0; i < 100; i++ ){
        printf("-");
    }
    printf("\n");
    //Print every 10 rounds marker
    printf("     ");
    for( int i = 1; i < 10; i++ ){
        printf("         %d", i);
    }
    printf("         10\n");
    //Print every round marker
    printf("     ");
    for( int i = 0; i < 10; i++ ){
        printf("1234567890");
    }
    printf("\n");

    // Return zero
    return( 0 );
}


////////////////////////////////////////////////////////////////////////////////
//
// Function     : main
// Description  : The main function for the CMPSC311 assignment #1
//
// Inputs       : argc - the number of command line parameters
//                argv - the parameters
// Outputs      : 0 if successful test, -1 if failure

int main( int argc, char **argv ) {

    /* Local variables */
    int cmp311_deck[NUM_CARDS];  // This is the deck of cards
    float player_money = 100.00;
    float histogram_values[100]; // This stores the player's money after each round
    int round_num; // Stores the number of rounds played
    int win_counter = 0; // Keeps track of the number of wins
    float win_percentage;

    /* Preamble information */
    printf( "CMPSC311 - Assignment #1 - Fall 2020\n\n" );
    srand(time(NULL)) ;

    /* Step #1 - create the deck of cards */
    for( int i = 0; i < NUM_CARDS; i++ ) {
        cmp311_deck[i] = i;
    }

    /* Step #2 - print the deck of cards */
    print_cards( cmp311_deck, NUM_CARDS );

    /* Step #4 - shuffle the deck */
    shuffle_cards( cmp311_deck, NUM_CARDS );
    
    /* Step #5 - print the shuffled deck of cards */
    print_cards( cmp311_deck, NUM_CARDS );

    /* Step #6 - sort the cards */
    sort_cards( cmp311_deck, NUM_CARDS );

    /* Step #7 - print the sorted deck of cards */
    print_cards( cmp311_deck, NUM_CARDS );

    /* Step #9 - deal the hands */
    for(int i = 0; i < 100; i++){
        win_counter =  win_counter + play_hand( cmp311_deck, NUM_CARDS, &player_money );
        printf("After hand %d player has %1.2f$ left\n", i + 1,  player_money );
        histogram_values[i] = player_money;
        round_num = i + 1;
        if ( player_money < 5.0 ){
            break;
        }
    }
    win_percentage =  100.0 * ( (float)win_counter / (float)round_num );
    printf("-------------\n");
    printf("Blackjack done - player won %d out of %d hands (%1.2f).\n\n", win_counter, round_num, win_percentage );
    /* Step 10 show histogram */
    show_player_money_histogram(histogram_values, round_num);

    /* Exit the program successfully */
    printf( "\n\nCMPSC311 - Assignment #1 - Fall 2020 Complete.\n" );
    return( 0 );
}