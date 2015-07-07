#include <iostream>

#include "GameState.h"

#include "ai.h"
#include "util.h"


using namespace std;

GameState::GameState(player_position_t first_dealer) {
  current_dealer = first_dealer;
  current_leader = increment_position(first_dealer);
  for (int i = 0; i < NUM_CARDS; ++i) {
    cards_played[i] = false;
  }
  tricks_played = 0;
}

void GameState::record_flip_card(card_t flip_card) {
  cards_played[card_hash(flip_card)] = true;
}

void GameState::record_trump_call(trump_decision_t trump_call) {
  trump_caller = trump_call.caller;
  current_trump = trump_call.suit;
  cout<<"Recorded trump suit of "<<suit_names[current_trump]<<endl;
}

void GameState::record_play(card_t play_card, player_position_t player) {
  cards_played[card_hash(play_card)] = true;
  trick_cards.push_back(play_card);

  if (trick_cards.size() == 1) {
    current_trick_suit = effective_suit(play_card, current_trump);
    current_winning_card = play_card;
    trick_winner = player;
  }
  else if (card_can_win(play_card)) {
    current_winning_card = play_card;
    trick_winner = player;
    cout<<"winning card"<<endl;
  }

  if (trick_cards.size() == NUM_PLAYERS) {
    trick_cards.clear();
    for (int i = 0; i < NUM_CARDS; ++i) {
      cards_played[i] = false;
    }
    ++tricks_played;
    cout<<"Finished trick "<<tricks_played<<endl;
    if (tricks_played == 5) {
      increment_position(current_dealer);
      current_leader = current_dealer;
      increment_position(current_leader);
      cout<<"\n\nNew dealer: "<<player_position_names[current_dealer]<<", leader: "<<player_position_names[current_leader]<<endl;
      tricks_played = 0;
    }
    else {
      current_leader = trick_winner;
      cout<<"New leader = winner = "<<player_position_names[current_leader]<<endl;
    }
  }  
}

deque<card_t> GameState::winnable_cards(const hand_t &hand) {
  deque<card_t> winning_cards;
  for (card_t card: hand) {
    if (card_can_win(card))
      winning_cards.push_back(card);
  }
  return winning_cards;
}

// Returns the highest trump that has not yet been played this hand
// does not cound trump in computer's hand - only those played or shown to all
card_t GameState::highest_unplayed_trump() {
  const int NUM_TRUMP_CARDS = 7;
  const int LEFT_INDEX = 5;
  card_value_t trump_values[NUM_TRUMP_CARDS] = {NINE, TEN, QUEEN, KING, ACE, JACK, JACK};
  card_t highest_trump;
  highest_trump.value = NINE;
  highest_trump.suit = current_trump;
  for (int i = 1; i < NUM_TRUMP_CARDS; ++i) {
    card_t this_card;
    this_card.value = trump_values[i];
    this_card.suit = (i == LEFT_INDEX) ? swap_color(current_trump) : current_trump;
    bool this_card_played = cards_played[card_hash(this_card)];
    if (!this_card_played)
      highest_trump = this_card;
  }
  return highest_trump;
}

// returns true if card beats card2 in trick_suit
bool GameState::card_can_win(card_t card) {
  if (is_trump(card, current_trump)) {
    if (!is_trump(current_winning_card, current_trump) ||
        card_value(card, current_trump) >
        card_value(current_winning_card, current_trump)) {
      return true;
    }
  }
  else if (card.suit == current_trick_suit &&
           !is_trump(current_winning_card, current_trump) &&
           card.value > current_winning_card.value) {
    return true;
  }
  return false;
}

int GameState::card_hash(card_t card) {
  return (card.value + CARDS_PER_SUIT * card.suit);
}

