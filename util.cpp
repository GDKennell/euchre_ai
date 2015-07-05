#include <iostream>

#include "ai.h"
#include "util.h"

using namespace std;

typedef bool (*)(card_t, card_t, suit_t) card_comp_fn_t

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

suit_t effective_suit(card_t card, suit_t trump) {
  if (card.value == JACK && card.suit == swap_color(trump))
    return trump;
  return card.suit;
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

int my_random(int i) {
  return rand() % i;
}

bool card_is_lower(card_t card1, card_t card2, suit_t trump) {
  if (is_trump(card1, trump) && !is_trump(card2, trump)) {
    return false;
  }
  else if (is_trump(card2, trump) && !is_trump(card1, trump)) {
    return true;
  }
  else if (is_trump(card1, trump) && is_trump(card2, trump)) {
    return card_value(card1, trump) < card_value(card2, trump);
  }
  else {
    return card1.value < card2.value;
  }
}

bool card_is_higher(card_t card1, card_t card2, suit_t trump) {
  return !card_is_lower(card1, card2, trump);
}

card_t find_extreme_card(const hand_t &hand, suit_t trump, card_comp_fn_t compare) {
  card_t extreme_card = hand[0];
  for (int i = 1; i < hand.size(); ++i) {
    if (compare(hand[i], extreme_card)) {
      extreme_card = hand[i];
    }
  }
  return extreme_card;
}

card_t find_highest_card(const hand_t &hand, suit_t trump) {
  return find_extreme_card(hand, trump, card_is_higher);
}

card_t find_lowest_card(const hand_t &hand, suit_t trump) {
  return find_extreme_card(hand, trump, card_is_lower);
}

deque<card_t> find_highest_offsuit(const hand_t &hand, suit_t trump) {
  hand_t highest_offsuit;
  for (auto card: hand) {
    if (is_trump(card, trump))
      continue;
    if (highest_offsuit.empty() || card.value == highest_offsuit.first().value) {
      highest_offsuit.push_back(card);
    }
    else if (!highest_offsuit.empty() && card.value > highest_offsuit.first().value) {
      highest_offsuit.clear();
      highest_offsuit.push_back(card);
    }
  }
  return highest_offsuit;
}

card_t find_highest_trump(const hand_t &hand, suit_t trump) {
  hand_t trump_cards;
  for (auto card: hand) {
    if (is_trump(card, trump))
      trump_cards.push_back(card);
  }
  return find_highest_card(trump_cards, trump);
}

card_t find_lowest_trump(const hand_t &hand, suit_t trump) {
  hand_t trump_cards;
  for (auto card: hand) {
    if (is_trump(card, trump))
      trump_cards.push_back(card);
  }
  return find_lowest_card(trump_cards, trump);
}

// Finds card in hand and removes it
void remove_card(card_t card, hand_t &hand) {
  for (int i = 0; i < hand.size(); ++i) {
    if (hand[i] == card) {
      hand.erase(hand.begin() + i);
      break;
    }
  }
}

// Returns a deque same size as card_list with a count of how many
// cards of that card's suit are in the hand (min 1 because of counting that card)
deque<int> count_suits(const deque<card_t> &card_list, const hand_t &hand) {
  deque<int> suit_counts;
  suit_counts.resize(card_list.size());
  for (int i = 0; i < card_list.size(); ++i) {
    card_t list_card = card_list[i];
    for (auto hand_card: hand) {
      if (effective_suit(list_card) == effective_suit(hand_card)) {
        ++suit_counts[i];
      }
    }
  }
  return suit_counts;
}

// Iterates through list and returns the index of the minimum
int find_min_index(const deque<int>& list) {
  int min_i = 0, min_val = list[0];
  for (int i = 1; i < list.size(); ++i) {
    if (list[i] < min_val) {
      min_i = i;
      min_val = list[i];
    }
  }
  return min_i;
}

// returns playabale cards from hand given suit that was led
deque<card_t> legal_cards(const hand_t &hand, suit_t trick_suit) {
  bool can_follow_suit = false;
  deque<card_t> cards_of_suit;
  for (auto card: hand) {
    if (effective_suit(card) == trick_suit) {
      can_follow_suit = true;
      cards_of_suit.push_back(card);
    }
  }
  if (can_follow_suit) {
    return cards_of_suit;
  }
  else {
    return hand;
  }
}


