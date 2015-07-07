#include "data_structures.h"

bool is_left(const card_t &card, suit_t trump);

bool is_trump(card_t card, suit_t trump);

// if card is left, switches its color, else returns the card's suit
suit_t effective_suit(card_t card, suit_t trump);

suit_t swap_color (suit_t suit);

void display_hand(const hand_t &hand, 
                  card_t flip_card, 
                  player_position_t dealer);

trump_call_t input_trump_decision(const hand_t &hand, 
                                  card_t flip_card, 
                                  player_position_t dealer);

// returns random number between 0 and i-1 (inclusive)
int my_random(int i);

// Expects to be passed cards of same effective suit
// If multiple offsuits present, returns highest value with
// tie broken arbitrarily
card_t find_highest_card(const hand_t &hand, suit_t trump);
int find_highest_card_index(const hand_t &hand, suit_t trump);

// Same as find_highest_card but with lowest
card_t find_lowest_card(const hand_t &hand, suit_t trump);

std::deque<card_t> find_highest_offsuit(const hand_t &hand, suit_t trump);
card_t find_highest_trump(const hand_t &hand, suit_t trump);
card_t find_lowest_trump(const hand_t &hand, suit_t trump);

// Finds card in hand and removes it
void remove_card(card_t card, hand_t &hand);

// Returns a deque same size as card_list with a count of how many
// cards of that card's suit are in the hand (min 1 because of counting that card)
std::deque<int> count_suits(const std::deque<card_t> &card_list, const hand_t &hand, suit_t trump);

// Iterates through list and returns the index of the minimum
int find_min_index(const std::deque<int>& list);

// returns playabale cards from hand given suit that was led
std::deque<card_t> find_legal_cards(const hand_t &hand, suit_t trick_suit, suit_t trump);



