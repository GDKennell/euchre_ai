#include <algorithm>
#include <iostream>
#include <fstream>
#include <deque>
#include <vector>

#include "ai.h"
#include "data_structures.h"
#include "util.h"

using namespace std;

class Deck {
  public:
    // Initializes deck in order and full
    Deck();

    // Refills deck and shuffles it
    void shuffle();

    // draws num_cards cards
    deque<card_t> draw(int num_cards);

    // draws one card
    card_t draw();

  private:
    deque<card_t> cards;
    void reset();
};

Deck::Deck() {
  reset();
}

int my_random(int i) {
  return rand() % i;
}

void Deck::shuffle() {
  reset();
  srand ( unsigned ( std::time(0) ) );
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

const int NUM_TEST_HANDS = 20;

int main() {
  Deck deck;

  player_position_t dealer = THIS_PLAYER;
  deque<trump_decision_state_t> data_pairs(NUM_TEST_HANDS);
  for (int i = 0; i < NUM_TEST_HANDS; ++i) {
    deck.shuffle();
    hand_t hand = deck.draw(5);

    card_t flip_card = deck.draw();

    trump_call_t human_call = input_trump_decision(hand, flip_card, dealer);
    trump_call_t comp_call = calculate_trump_call(hand, flip_card, dealer);
    if (comp_call == human_call)
      cout<<"I agree"<<endl<<endl;
    else {
      switch(comp_call){
        case PASS:
          cout<<"I think we should pass";
          break;
        case PICK_IT_UP:
          cout<<"I think we should order it up";
          break;
        case ALONE:
          cout<<"I think we should go alone";
          break;
      }
      cout<<endl<<endl;
    }

    data_pairs[i].hand = hand;
    data_pairs[i].flip_card = flip_card;
    data_pairs[i].dealer = dealer;
    data_pairs[i].human_call = human_call;
    data_pairs[i].heuristic_eval = 0.0;

    increment_position(dealer);
  }

  ofstream csv_out;
  csv_out.open("trump_call_data.csv", fstream::out | fstream::app);

  for(int i = 0; i < NUM_TEST_HANDS; ++i) {
    csv_out<<data_pairs[i].heuristic_eval<<','<<trump_call_names[data_pairs[i].human_call]<<',';
    csv_out<<card_str(data_pairs[i].flip_card)<<',';
    switch (data_pairs[i].dealer) {
      case THIS_PLAYER:
        csv_out<<"Your deal";
        break;
      case PARTNER:
        csv_out<<"Your partner's deal";
      break;
      default:
        csv_out<<"Opponent's deal";
        break;
    } 
    for (int j = 0; j < 5; ++j) {
      csv_out<<','<<card_str(data_pairs[i].hand[j]);
    }
    csv_out<<endl;
  }
  csv_out.close();
}



