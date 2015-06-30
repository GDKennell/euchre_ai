#include <algorithm>
#include <iostream>
#include <fstream>
#include <deque>
#include <vector>

#include "ai.h"
#include "data_structures.h"
#include "Deck.h"
#include "util.h"

using namespace std;

const int NUM_TEST_HANDS = 20;

int main() {
  Deck deck;

  player_position_t dealer = THIS_PLAYER;
  deque<trump_decision_state_t> data_pairs(NUM_TEST_HANDS);
  for (int i = 0; i < NUM_TEST_HANDS; ++i) {
    deck.shuffle();
    hand_t hand = deck.draw(5);

    card_t flip_card = deck.draw();

    trump_decision_t human_call = input_trump_decision(hand, flip_card, dealer);
    trump_decision_t comp_call = calculate_trump_call(hand, flip_card, dealer);
    if (comp_call.call_type == human_call.call_type)
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



