#include <iostream>

#include "ai.h"
#include "util.h"

using namespace std;

bool is_left(const card_t &card, suit_t trump) {
  return (card.value == JACK && swap_color(card.suit) == trump);
}

bool is_trump(card_t card, suit_t trump) {
  if (card.value == JACK) {
    if (trump == CLUBS || trump == SPADES)
      return (card.suit == CLUBS || card.suit == SPADES);
    else
      return (card.suit == HEARTS || card.suit == DIAMONDS);
  }
  return card.suit == trump;
}

suit_t swap_color (suit_t suit) {
   switch (suit) {
      case HEARTS:
        return DIAMONDS;
      case DIAMONDS:
        return HEARTS;
      case SPADES:
        return CLUBS;
      case CLUBS:
        return SPADES;
    }
}

bool card_compare(card_t card1, card_t card2, suit_t trump) {
  if (is_trump(card1, trump) && !is_trump(card2, trump)) {
    return true;
  }
  else if (is_trump(card2, trump) && !is_trump(card1, trump)) {
    return false;
  }
  if (is_trump(card1,trump) && is_trump(card2,trump)) {
    return card_value(card1,trump) > card_value(card2,trump);
  }
  else if(card1.suit == card2.suit) {
    return card1.value > card2.value;
  }
  else {
    return card1.suit > card2.suit;
  }
}

hand_t sort_hand(const hand_t& hand, suit_t trump) {
  hand_t sorted_hand = hand;
  for (int i = 1; i < 5; ++i) {
    for (int j = i-1; j >= 0; --j) {
      if (card_compare(sorted_hand[j+1],sorted_hand[j],trump))
        swap(sorted_hand[j+1],sorted_hand[j]);
      else
        break;
    }
  }
  return sorted_hand;
}

void display_hand(const hand_t &hand, 
                  card_t flip_card, 
                  player_position_t dealer) {
  switch (dealer) {
    case THIS_PLAYER:
      cout<<"Your deal"<<endl;
      break;
    case PARTNER:
      cout<<"Your partner's deal"<<endl;
      break;
    default:
      cout<<"Opponent's deal"<<endl;
      break;
  }
  cout<<"Flip Card: "<<card_str(flip_card)<<endl;

  cout<<"Hand:"<<endl;
  hand_t sorted_hand = sort_hand(hand, flip_card.suit);
  for (int i = 0; i < 5; ++i) {
    cout<<"\t[ "<<card_str(sorted_hand[i])<<" ]"<<endl;
  }
  cout<<endl;
}

trump_call_t input_trump_decision(const hand_t &hand, 
                                  card_t flip_card, 
                                  player_position_t dealer) {
  display_hand(hand, flip_card, dealer);

  cout<<"Choose [p]ass, [o]rder up, [a]lone: ";
  char ch;
  cin >> ch;
  switch (ch) {
    case 'p':
      return PASS;
    case 'o':
      return PICK_IT_UP;
    case 'a':
      return ALONE;
  }
  cout<<"You didn't give me 'p' 'o' or 'a' you knuckle head!"<<endl;
  throw "a tantrum";
}

