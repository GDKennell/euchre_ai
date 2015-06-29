#ifndef AI_H
#define AI_H

#include "data_structures.h"

double trump_evaluation(const hand_t &hand, 
                        card_t flip_card, 
                        player_position_t dealer);

// remove worst card from hand and insert the flip card
void swap_card(hand_t &hand, card_t flip_card);

// returns heuristic value of card given trump
double card_value(card_t card, suit_t trump);

#endif

