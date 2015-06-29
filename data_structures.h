#ifndef DATA_STRUCTURES_H
#define DATA_STRUCTURES_H

#include <deque>

enum player_position_t {THIS_PLAYER, LEFT_OPPONENT, PARTNER, RIGHT_OPPONENT};
void increment_position(player_position_t &position);

const char *trump_call_names[3] = {"Pass", "Pick it up", "Alone"};
enum trump_call_t {PASS, PICK_IT_UP, ALONE};

const char *suit_names[4] = {"Clubs", "Diamonds", "Hearts", "Spades"};
enum suit_t {CLUBS, DIAMONDS, HEARTS, SPADES};

const char *card_value_names[6] = {"9 ", "10", "J ", "Q ", "K ", "A "};
enum card_value_t {NINE, TEN, JACK, QUEEN, KING, ACE};

struct card_t {
  suit_t suit;
  card_value_t value;
};
const char* card_str(const card_t &card);

typedef std::deque<card_t> hand_t;

struct trump_decision_state_t {
  hand_t hand;
  card_t flip_card;
  player_position_t dealer;
  trump_call_t human_call;
  double heuristic_eval;
};

#endif

