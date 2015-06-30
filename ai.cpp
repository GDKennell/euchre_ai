#include <iostream>

#include "ai.h"
#include "data_structures.h"
#include "util.h"

using namespace std;

const double alone_threshold = 23.0;
const double call_it_threshold = 16.0;

// remove worst card from hand and insert the flip card
void swap_card(hand_t &hand, card_t flip_card);


double trump_evaluation(const hand_t &hand, 
                        suit_t trump);

double trump_evaluation(const hand_t &hand, 
                        card_t flip_card, 
                        player_position_t dealer);

trump_decision_t calculate_first_trump_call(const hand_t &hand, 
                                            card_t flip_card, 
                                            player_position_t dealer) {
    trump_decision_t decision;
    double heuristic_eval = trump_evaluation(hand, flip_card, dealer);
    cout<<"Evaluation: "<<heuristic_eval<<endl;
    if (heuristic_eval >= alone_threshold)
      decision.call_type = ALONE;
    if (heuristic_eval >= call_it_threshold)
      decision.call_type = PICK_IT_UP;
    else
      decision.call_type = PASS;
    return decision;
}

trump_decision_t calculate_second_trump_call(const hand_t &hand,
                                             card_t flip_card, 
                                             bool must_choose) {
    trump_decision_t decision;
    int best_suit_i = -1;
    double best_suit_eval = 0.0;
    for (int suit_i = 0; suit_i < NUM_SUITS; ++suit_i) {
      suit_t possible_suit = (suit_t)suit_i;
      if (possible_suit == flip_card.suit)
        continue;
      double this_suit_eval = trump_evaluation(hand, possible_suit);
      if (best_suit_i == -1 || best_suit_eval < this_suit_eval) {
        best_suit_i = suit_i;
        best_suit_eval = this_suit_eval;
      }
    }
    decision.suit = (suit_t)best_suit_i;
    if (best_suit_eval >= alone_threshold)
      decision.call_type = ALONE;
    else if (must_choose || best_suit_eval >= call_it_threshold)
      decision.call_type = PICK_IT_UP;
    else
      decision.call_type = PASS;
    return decision;
}

double trump_evaluation(const hand_t &hand, 
                        card_t flip_card, 
                        player_position_t dealer) {
  double total_value = 0.0;
  suit_t trump = flip_card.suit;
  hand_t test_hand(hand);
  if (dealer == THIS_PLAYER) {
    swap_card(test_hand,flip_card);
  }
  else if (dealer == PARTNER) {
    total_value += card_value(flip_card, trump);
  }
  else {
    total_value -= card_value(flip_card, trump);
  }

  total_value += trump_evaluation(test_hand, trump);
  return total_value;
}

double trump_evaluation(const hand_t &hand, 
                        suit_t trump) {
  double total_value = 0.0;

  bool suit_exists[4] = {false, false, false, false};
  int num_suits = 0;
  for (int i = 0; i < 5; ++i) {
    card_t card = hand[i];
    total_value += card_value(card, trump);

    suit_t card_suit = is_left(card, trump) ? swap_color(card.suit) : card.suit;
    if (!suit_exists[card_suit]) {
      suit_exists[card_suit] = true;
      ++num_suits;
    }
  }
  
  if (suit_exists[trump]) {
    total_value += 4.0 - (double)num_suits;
  }

  return total_value;
}

void swap_card(hand_t &hand, card_t flip_card) {
  suit_t trump = flip_card.suit;

  int worst_i = 0;
  card_t worst_card = hand[0];
  for (int i = 1; i < 5; ++i) {
    if (is_trump(worst_card, trump) 
        && !is_trump(hand[i], trump)) {
      worst_card = hand[i];
      worst_i = i;
    }
    else if (card_value(hand[i],trump) < card_value(worst_card,trump)) {
      worst_card = hand[i];
      worst_i = i;
    }
  }
  hand[worst_i] = flip_card;
}

double card_value(card_t card, suit_t trump) {
  if (is_trump(card, trump)) {
    double raw_value;
    if (card.value == JACK && card.suit == trump)
      raw_value = ACE + 3.0;
    else if (card.value == JACK && card.suit != trump)
      raw_value = ACE + 1.0;
    else if (card.value >= QUEEN)
      raw_value = card.value;
    else
      raw_value = card.value + 1.0;

    return raw_value + 1.0;
  }
  else {
    if (card.value == ACE) {
      return 3.0;
    }
    else if (card.value == KING) {
      return 1.0;
    }
    else {
      return 0.0;
    }
  }
}

