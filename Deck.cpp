#include <stdlib.h>

#include "Deck.h"
#include "util.h"

using namespace std;

Deck::Deck() {
  reset();
}

void Deck::shuffle() {
  reset();
  srand ( unsigned ( time(0) ) );
  random_shuffle(cards.begin(), cards.end(), my_random);
}

deque<card_t> Deck::draw(int num_cards) {
  deque<card_t> cards(num_cards);
  for (int i = 0; i < num_cards; ++i) {
    cards[i] = draw();
  }
  return cards;
}

card_t Deck::draw() {
  card_t result = cards.front();
  cards.pop_front();
  return result;
}

void Deck::reset() {
  cards.resize(24);
  for (int suit_i = 0; suit_i < 4; ++suit_i) {
    for (int val_i = 0; val_i < 6; ++val_i) {
      int i = val_i + (6 * suit_i);
      cards[i].suit = (suit_t)suit_i;
      cards[i].value = (card_value_t)val_i;
    }
  }
}


