#include "data_structures.h"

bool is_left(const card_t &card, suit_t trump);

bool is_trump(card_t card, suit_t trump);

suit_t swap_color (suit_t suit);

void display_hand(const hand_t &hand, 
                  card_t flip_card, 
                  player_position_t dealer);

