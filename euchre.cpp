#include <iostream>

#include "ai.h"
#include "data_structures.h"
#include "Deck.h"
#include "util.h"

using namespace std;

// Input Helpers
player_position_t input_first_dealer();
hand_t input_hand();
card_t input_flip_card();
card_t input_card();

// Gameplay Helpers
trump_decision_t decide_trump(hand_t hand, card_t flip_card, player_position_t dealer);
// Plays out trick with first_player playing first. returns which player takes the trick
player_position_t play_trick(hand_t hand, suit_t trump_suit, player_position_t first_player);

int main() {
  player_position_t dealer = input_first_dealer();

  while(1) {
    hand_t hand = input_hand();
    card_t flip_card = input_flip_card();

    player_position_t first_player = dealer;
    increment_position(first_player);

    trump_decision_t trump_decision = decide_trump(hand, flip_card, dealer);
    suit_t trump_suit = trump_decision.suit;
    for (int i = 0; i < 5; ++i) {
      first_player = play_trick(hand, trump_suit, first_player);
    }

    increment_position(dealer);
  }
}

// Input Helpers
player_position_t input_first_dealer() {
  cout<<"Who is first dealer? [l]eft opponent, [p]artner, [r]ight opponent, [y]ou: ";
    string input;
    getline(cin,input);
    switch(input[0]) {
    case 'l':
      return LEFT_OPPONENT;
    case 'p':
      return PARTNER;
    case 'r':
      return RIGHT_OPPONENT;
    case 'y':
      return THIS_PLAYER;
    default:
      cout<<"Bad input"<<endl;
      exit(1);
      return THIS_PLAYER;
  }
}

hand_t input_hand() {
  cout << "Input hand:"<<endl;
  hand_t hand;
  for (int i = 0; i < 5; ++i) {
    hand.push_back(input_card());
  }
  return hand;
}

card_t input_flip_card() {
  cout << "Input flip card:"<<endl;
  return input_card();
}

card_t input_card() {
  cout<<"\t:";
  string card_input_str;
  getline(cin, card_input_str);
  card_t new_card;
  bool bad_input = false;
  do {
    if (bad_input) {
      cout<<"Cannot process input. Correct format: <9/10/j/q/k/a>/<c/d/h/s>"<<endl;
      cout<<"input again\n\t";
      bad_input = false;
    }
    switch (card_input_str[0]) {
      case '9':
        new_card.value = NINE;
        break;
      case '1':
        new_card.value = TEN;
        break;
      case 'j':
        new_card.value = JACK;
        break;
      case 'q':
        new_card.value = QUEEN;
        break;
      case 'k':
        new_card.value = KING;
        break;
      case 'a':
        new_card.value = ACE;
        break;
      default:
        bad_input = true;
    }

    switch (card_input_str.back()) {
      case 'h':
        new_card.suit = HEARTS;
        break;
      case 's':
        new_card.suit = SPADES;
        break;
      case 'c':
        new_card.suit = CLUBS;
        break;
      case 'd':
        new_card.suit = DIAMONDS;
        break;
      default:
        bad_input = true;
    }
  } while(bad_input);
  return new_card;
}

// Gameplay Helpers
trump_decision_t decide_trump(hand_t hand, card_t flip_card, player_position_t dealer) {
  player_position_t current_player = dealer;
  increment_position(current_player);

  trump_decision_t decision;

  // First round
  for (int i = 0; i < 4; ++i) {
    decision.caller = current_player;
    decision.suit = flip_card.suit;

    // Computer first round decision
    if (current_player == THIS_PLAYER) {
      trump_decision_t comp_decision = calculate_first_trump_call(hand, flip_card, dealer);
      cout<<"My decision: "<<trump_call_names[comp_decision.call_type]<<endl;
      if (comp_decision.call_type == PICK_IT_UP) {
        decision.call_type = PICK_IT_UP;
        return decision;
      }
      else if (comp_decision.call_type == ALONE) {
        decision.call_type = ALONE;
        return decision;
      }
    }
    // Other players first round decision
    else {
      bool bad_input = false;
      do {
        if (bad_input) {
          cout<<"Bad input, try again"<<endl;
          bad_input = false;
        }
        cout<<"Trump decision of "<<player_position_names[current_player]<<":"<<endl;
        cout<<"\t[p]ass, [o]der up, or [a]lone: ";
        char ch;
        cin >> ch;
        switch (ch) {
          case 'p':
            continue;
          case 'o':
            decision.call_type = PICK_IT_UP;
            return decision;
          case 'a':
            decision.call_type = ALONE;
            return decision;
          default:
            bad_input = true;
        }
      } while(bad_input);
    }
    increment_position(current_player);
  }

  // Second round
  for (int i = 0; i < 4; ++i) {
    decision.caller = current_player;
    if (current_player == THIS_PLAYER) {
      auto comp_decision = calculate_second_trump_call(hand,
                                                       flip_card,
                                                       dealer == THIS_PLAYER); 
      if (comp_decision.call_type == PICK_IT_UP ||
          comp_decision.call_type == ALONE) {
        comp_decision.caller = current_player;
        return comp_decision;
      }
    }
    else {
      bool bad_input = false;
      do {
        if (bad_input) {
          cout<<"Bad input, try again"<<endl;
          bad_input = false;
        }
        cout<<"Trump decision of "<<player_position_names[current_player]<<":"<<endl;
        cout<<"\t[p]ass, suit: <c,d,h,s>, or suit alone, <ca,da,ha,sa>: ";
        string decision_str;
        getline(cin, decision_str);
        if (decision_str[0] == 'p')
          continue;

        if (decision_str.size() >= 2 &&
            decision_str[1] == 'a')
          decision.call_type = ALONE;
        else
          decision.call_type = PICK_IT_UP;
        switch (decision_str[0]) {
          case 'c':
            decision.suit = CLUBS;
            return decision;
          case 'd':
            decision.suit = DIAMONDS;
            return decision;
          case 'h':
            decision.suit = HEARTS;
            return decision;
          case 's':
            decision.suit = SPADES;
            return decision;
          default:
            bad_input = true;
        }
      } while(bad_input);
    }
  }
  cout<<"Error: somehow managed to get thru with no one calling anything"<<endl;
  exit(1);
}

// Plays out trick with first_player playing first. returns which player takes the trick
player_position_t play_trick(hand_t hand, suit_t trump_suit, player_position_t first_player) {
  return THIS_PLAYER;
}



