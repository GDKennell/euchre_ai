#ifndef DECK_H
#define DECK_H

#include "data_structures.h"

class Deck {
  public:
    // Initializes deck in order and full
    Deck();

    // Refills deck and shuffles it
    void shuffle();

    // draws num_cards cards
    std::deque<card_t> draw(int num_cards);

    // draws one card
    card_t draw();

  private:
    std::deque<card_t> cards;
    void reset();
};

#endif

