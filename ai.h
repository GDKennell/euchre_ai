#ifndef AI_H
#define AI_H

#include "data_structures.h"

extern const double alone_threshold;
extern const double call_it_threshold;

trump_decision_t calculate_first_trump_call(const hand_t &hand, 
                                            card_t flip_card, 
                                            player_position_t dealer);

trump_decision_t calculate_second_trump_call(const hand_t &hand,
                                             card_t flip_card, 
                                             bool must_choose);

void swap_card(hand_t &hand, card_t flip_card);

// returns heuristic value of card given trump
// TODO: will soon move back to top of ai.cpp - this should be private
double card_value(card_t card, suit_t trump);

#endif

