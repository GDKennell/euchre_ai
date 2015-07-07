#ifndef DATA_STRUCTURES_H
#define DATA_STRUCTURES_H

#include <deque>
extern const char *player_position_names[4];
const int NUM_PLAYERS = 4;
enum player_position_t {THIS_PLAYER, LEFT_OPPONENT, PARTNER, RIGHT_OPPONENT};
player_position_t increment_position(player_position_t &position);

extern const char *trump_call_names[3];
enum trump_call_t {PASS, PICK_IT_UP, ALONE};

extern const char *suit_names[4];
enum suit_t {CLUBS, DIAMONDS, HEARTS, SPADES};

const int NUM_SUITS = 4;
const int CARDS_PER_SUIT = 6;
extern const char *card_value_names[6];
enum card_value_t {NINE, TEN, JACK, QUEEN, KING, ACE};

struct card_t {
  suit_t suit;
  card_value_t value;
};
const char* card_str(const card_t &card);

typedef std::deque<card_t> hand_t;

struct trump_decision_t {
  trump_call_t call_type;
  suit_t suit;
  player_position_t caller;
};

#endif

