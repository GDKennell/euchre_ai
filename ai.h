#ifndef AI_H
#define AI_H

#include "data_structures.h"
#include "GameState.h"

extern const double alone_threshold;
extern const double call_it_threshold;

trump_decision_t calculate_first_trump_call(const hand_t &hand, 
                                            card_t flip_card, 
                                            player_position_t dealer);

trump_decision_t calculate_second_trump_call(const hand_t &hand,
                                             card_t flip_card, 
                                             bool must_choose);

card_t calculate_move(GameState &game_state, hand_t &hand, suit_t trump_suit);

void swap_card(hand_t &hand, card_t flip_card);

// returns heuristic value of card given trump
// TODO: will soon move back to top of ai.cpp - this should be private
double card_value(card_t card, suit_t trump);

#endif

