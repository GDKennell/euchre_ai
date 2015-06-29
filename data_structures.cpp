#include "data_structures.h"

#include <string>

using namespace std;

const char *trump_call_names[3] = {"Pass", "Pick it up", "Alone"};
const char *suit_names[4] = {"Clubs", "Diamonds", "Hearts", "Spades"};
const char *card_value_names[6] = {"9 ", "10", "J ", "Q ", "K ", "A "};

void increment_position(player_position_t &position) {
  switch (position) {
    case THIS_PLAYER:
      position = LEFT_OPPONENT;
      break;
    case LEFT_OPPONENT:
      position = PARTNER;
      break;
    case PARTNER: 
      position = RIGHT_OPPONENT;
      break;
    case RIGHT_OPPONENT:
      position = THIS_PLAYER;
      break;
  }
}

const char* card_str(const card_t &card) {
  string value_str = string(card_value_names[card.value]);
  string of_str = string(" of ");
  string suit_str = string(suit_names[card.suit]);

  string full_str = value_str + of_str + suit_str;
  return full_str.c_str();
}

