#include "Deck.h"
#include "../utils/Constants.h"

Deck::Deck() {}

uint8_t Deck::getCard() {

  return this->deck[this->index++];

}

void Deck::shuffle() {

  for (uint8_t x = 0; x < Constants::NumberOfCards; x++) {
      
    this->deck[x] = x;
            
  }
    
  
  // Randomly switch bones in the array with each other ..
  
  for (uint8_t x = Constants::NumberOfCards - 1; x > 0; x--) {
      
    uint8_t y = random(x + 1);

    uint8_t z = this->deck[y];
    this->deck[y] = this->deck[x];
    this->deck[x] = z;

  }
  
  this->index = 0;

}