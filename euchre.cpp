#include <algorithm>
#include <iostream>
#include <fstream>
#include <deque>
#include <vector>

#include "ai.h"
#include "data_structures.h"

using namespace std;

const double alone_threshold = 23.0;

const double call_it_threshold = 16.0;

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

trump_call_t input_trump_decision(const hand_t &hand, 
                                  card_t flip_card, 
                                  player_position_t dealer);


const int NUM_TEST_HANDS = 20;

bool is_trump(card_t card, suit_t trump);

void display_hand(const hand_t &hand, 
                  card_t flip_card, 
                  player_position_t dealer);

int main() {
  Deck deck;

  player_position_t dealer = THIS_PLAYER;
  deque<trump_decision_state_t> data_pairs(NUM_TEST_HANDS);
  for (int i = 0; i < NUM_TEST_HANDS; ++i) {
    deck.shuffle();
    hand_t hand = deck.draw(5);

    card_t flip_card = deck.draw();

    trump_call_t human_call = input_trump_decision(hand, flip_card, dealer);
    double heuristic_eval = trump_evaluation(hand, flip_card, dealer);
    trump_call_t comp_call = PASS;
    if (heuristic_eval >= call_it_threshold)
      comp_call = PICK_IT_UP;
    if (heuristic_eval >= alone_threshold)
      comp_call = ALONE;
    cout<<"Evaluation: "<<heuristic_eval<<endl;
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
    data_pairs[i].heuristic_eval = heuristic_eval;

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

bool is_trump(card_t card, suit_t trump) {
  if (card.value == JACK) {
    if (trump == CLUBS || trump == SPADES)
      return (card.suit == CLUBS || card.suit == SPADES);
    else
      return (card.suit == HEARTS || card.suit == DIAMONDS);
  }
  return card.suit == trump;
}

double card_value(card_t card, suit_t trump) {
  if (is_trump(card, trump)) {
    double raw_value;
    if (card.value == JACK && card.suit == trump)
      raw_value = ACE + 3.0;
    else if (card.value == JACK && card.suit != trump)
      raw_value = ACE + 1.0;
    else if (card.value >= QUEEN)
      raw_value = card.value;
    else
      raw_value = card.value + 1.0;

    return raw_value + 1.0;
  }
  else {
    if (card.value == ACE) {
      return 3.0;
    }
    else if (card.value == KING) {
      return 1.0;
    }
    else {
      return 0.0;
    }
  }
}

void swap_card(hand_t &hand, card_t flip_card) {
  suit_t trump = flip_card.suit;

  int worst_i = 0;
  card_t worst_card = hand[0];
  for (int i = 1; i < 5; ++i) {
    if (is_trump(worst_card, trump) 
        && !is_trump(hand[i], trump)) {
      worst_card = hand[i];
      worst_i = i;
    }
    else if (card_value(hand[i],trump) < card_value(worst_card,trump)) {
      worst_card = hand[i];
      worst_i = i;
    }
  }
  hand[worst_i] = flip_card;
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

bool is_left(const card_t &card, suit_t trump) {
  return (card.value == JACK && swap_color(card.suit) == trump);
}

double trump_evaluation(const hand_t &hand, 
                        card_t flip_card, 
                        player_position_t dealer) {
  double total_value = 0.0;
  suit_t trump = flip_card.suit;
  hand_t test_hand(hand);
  if (dealer == THIS_PLAYER) {
    swap_card(test_hand,flip_card);
  }
  else if (dealer == PARTNER) {
    total_value += card_value(flip_card, trump);
  }
  else {
    total_value -= card_value(flip_card, trump);
  }

  bool suit_exists[4] = {false, false, false, false};
  int num_suits = 0;
  for (int i = 0; i < 5; ++i) {
    card_t card = test_hand[i];
    total_value += card_value(card, trump);

    suit_t card_suit = is_left(card, trump) ? swap_color(card.suit) : card.suit;
    if (!suit_exists[card_suit]) {
      suit_exists[card_suit] = true;
      ++num_suits;
    }
  }
  
  if (suit_exists[trump]) {
    total_value += 4.0 - (double)num_suits;
  }


  return total_value;
}



