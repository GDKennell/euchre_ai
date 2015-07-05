#ifndef GAME_STATE_H
#define GAME_STATE_H

#include "data_structures.h"

const int NUM_CARDS = 24;

class GameState {
  public:
    GameState(player_position_t first_dealer);

    void record_flip_card(card_t flip_card);

    void record_trump_call(suit_t trump, player_position_t caller, call_type_t call_type);

    // Records that player has played playCard
    void record_play(card_t playCard, player_position_t player);

    // Returns the player that lead this trick
    player_position_t leading_player();

    // Returns the suit lead for the current trick
    suit_t trick_suit();

    // Returns the cards in hand that could 
    deque<card_t> winnable_cards(const hand_t &hand);

    // Returns true if partner has played and to date has the winning card played
    bool partner_is_winning_trick();

    // Returns the highest trump that has not yet been played this hand
    // does not cound trump in computer's hand - only those played or shown to all
    card_t highest_unplayed_trump();

    // Returns the winner of the most recent trick
    player_position_t last_trick_winner();

  private:
    suit_t current_trump;
    player_position_t current_leader;
    player_position_t trick_winner;
    bool card_played[NUM_CARDS];

    // list, in order from lead, of cards played in this trick
    deque<card_t> trick_cards;
}

#endif

