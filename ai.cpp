#include <iostream>

#include "ai.h"
#include "data_structures.h"
#include "GameState.h"
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
    else if (heuristic_eval >= call_it_threshold)
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

card_t calculate_move(GameState &game_state, hand_t &hand, suit_t trump_suit) {
  if (game_state.leading_player() == THIS_PLAYER) {
    // Leading
    // Highest offsuit card(s), multiple only if tie
    deque<card_t> highest_offsuit_cards = find_highest_offsuit(hand, trump_suit);

    if (highest_offsuit_cards.empty()) {
      // if only trump in hand
      card_t highest_trump = find_highest_trump(hand, trump_suit);
      card_t lowest_trump = find_lowest_trump(hand, trump_suit);
      if (card_value(game_state.highest_unplayed_trump(), trump_suit) <
          card_value(highest_trump)) {
        // play that highest trump if we have it
        remove_card(highest_trump, hand);
        return highest_trump;
      }
      else {
        // else play lowest trump
        remove_card(lowest_trump, hand);
        return lowest_trump;
      }
    }
    else {
      // Find highest_offsuit card with lowest # of cards in its suit
      // count of cards in suit, indexed by highest_offsuit_cards
      deque<int> suit_counts = count_suits(highest_offsuit_cards, hand);
      int best_card_i = find_min_index(suit_counts);
      card_t best_card = highest_offsuit_cards[best_card_i];
      remove_card(best_card, hand);
      return best_card;
    }
  }
  else {
    // Following
    suit_t trick_suit = game_state.trick_suit();
    deque<card_t> legal_cards = legal_cards(hand, trick_suit);
    deque<card_t> winnable_cards = game_state.winnable_cards(legal_cards);
    if (winnable_cards.empty() || game_state.partner_is_winning_trick()) { 
      // If no way to win trick or partner has it, play lowest legal
      worst_legal_card = find_lowest_card(legal_cards, trump_suit);
      remove_card(worst_legal_card, hand);
      return worst_legal_card;
    }
    else {
      // If can win
      if (effective_suit(legal_cards.first()) == trick_suit) {
        // If can win following suit, play highest of suit (trump included)
        highest_winnable_card = find_highest_card(winnable_cards, trump_suit);
        remove_card(highest_winnable_card, hand);
        return highest_winnable_card;
      }
      else {
        // If can win by trumping (playing trump on offsuit trick), play lowest trump
        lowest_winnable_card = find_lowest_card(winnable_cards, trump_suit);
        remove_card(lowest_winnable_card, hand);
        return lowest_winnable_card;
      }
    }
  }
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

    if (!suit_exists[effective_suit(card)]) {
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

  card_t worst_card = find_lowest_card(hand, trump);

  // First looking to see if we can short-suit ourselves before using it
  int suit_counts[] = {0,0,0,0};
  for (int i = 0; i < 5; ++i) {
    if (!is_trump(hand[i],trump)) {
      ++suit_counts[hand[i].suit];
    }
  }

  // Array of card indices that could be discarded to short-suit ourselves
  deque<int> short_suit_cards;
  for (int i = 0; i < 4; ++i) {
    if (suit_counts[i] == 1) {
      // Find card that is only one of i suit
      for (int j = 0; j < 5; ++j) {
        if (hand[j].suit == (suit_t)i) {
          short_suit_cards.push_back(j);
          break;
        }
      }
    }
  }

  // Find lowest value short-suiting card
  if (!short_suit_cards.empty()) {
    worst_card = find_lowest_card(short_suit_cards, trump);
  }

  cout<<"Swapping worst card, "<<card_str(worst_card);
  cout<<" for flip card "<<card_str(flip_card)<<endl;
  remove_card(worst_card, hand);
  hand.push_back(flip_card);
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

