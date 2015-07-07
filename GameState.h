#ifndef GAME_STATE_H
#define GAME_STATE_H

#include "data_structures.h"

const int NUM_CARDS = 24;

class GameState {
  public:
    GameState(player_position_t first_dealer);

    void record_flip_card(card_t flip_card);

    void record_trump_call(trump_decision_t trump_call);

    // Records that player has played play_card
    void record_play(card_t play_card, player_position_t player);

    // Returns the player that lead this trick
    player_position_t leading_player() {
      return current_leader;
    }

    // Returns the suit led for the current trick
    suit_t trick_suit() {
      return current_trick_suit;
    }

    // Returns the cards in hand that could 
    std::deque<card_t> winnable_cards(const hand_t &hand);

    // Returns true if partner has played and to date has the winning card played
    bool partner_is_winning_trick() {
      return trick_winner == PARTNER;
    }

    // Returns the highest trump that has not yet been played this hand
    // does not cound trump in computer's hand - only those played or shown to all
    card_t highest_unplayed_trump();

    // Returns the winner of the most recent trick
    player_position_t last_trick_winner() {
      return trick_winner;
    }

  private:
    player_position_t trump_caller;
    suit_t current_trump;
    suit_t current_trick_suit;
    player_position_t current_dealer;
    player_position_t current_leader;
    card_t current_winning_card;
    player_position_t trick_winner;
    bool cards_played[NUM_CARDS];

    int tricks_played;

    // list, in order from lead, of cards played in this trick
    std::deque<card_t> trick_cards;

    // Helpers
    // returns true if card beats current_winning_card
    bool card_can_win(card_t card);

    int card_hash(card_t card);
};

#endif

