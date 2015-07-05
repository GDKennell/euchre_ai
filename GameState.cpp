#include "GameState.h"

int card_hash(card_t card) {
  return (card.value + CARDS_PER_SUIT * card.suit);
}

GameState::GameState(player_position_t first_dealer) {
  current_leader = increment_position(first_dealer);
  for (int i = 0; i < NUM_CARDS; ++i) {
    card_played[i] = false;
  }
}

void GameState::record_flip_card(card_t flip_card) {
  card_played[card_hash(flip_card)] = true;
}

void GameState::record_play(card_t playCard, player_position_t player) {
  card_played[card_hash(playCard)] = true;
  trick_cards.push_back(playCard);
  if (trick_cards.size() == NUM_PLAYERS) {
    int winning_index = find_highest_card_index(trick_cards, current_trump);
    trick_winner = (player_position_t)((current_leader + winning_index) % NUM_PLAYERS);
    cards_played.clear();
    for (int i = 0; i < NUM_CARDS; ++i) {
      card_played[i] = false;
    }
  }
}


