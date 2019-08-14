#include "Player.h"

#include "../utils/Constants.h"
#include "../utils/CardUtils.h"
#include "../utils/Utils.h"

Player::Player() {}

uint8_t Player::getScore() {

  return this->score;

}

uint8_t Player::getPrevScore() {

  return this->prevScore;

}

bool Player::getGo() {

  return this->go;

}

void Player::setScore(uint8_t score) {

  this->score = score;

}

void Player::setPrevScore(uint8_t score) {

  this->prevScore = score;

}

void Player::setGo(bool value) {

  this->go = value;

}

void Player::addScore(uint8_t score) {

  this->score += score;

}

void Player::addPrevScore(uint8_t score) {

  this->prevScore += score;

}

uint8_t Player::getHandCard(uint8_t index) {

  return this->hand[index];

}

uint8_t Player::getHandCardCount() {

  return this->handIdx;

}

uint8_t Player::getHandCardIndex(uint8_t card) {

  for (uint8_t x = 0; x < 6; x++) {

    if (this->hand[x] == card) return x;
  
  }

 return Constants::NoCard;

}

uint8_t Player::getCribCard(uint8_t index) {

  return this->crib[index];

}

uint8_t Player::getCribCardCount() {

  return this->cribIdx;

}

void Player::calculateHandScore(Score scores[], uint8_t turnUp) {

  CardUtils::sort(this->hand, 4);
  calculateScores(scores, this->hand);

}

void Player::calculateCribScore(Score scores[], uint8_t turnUp) {

  CardUtils::sort(this->crib, 4);
  calculateScores(scores, this->crib);

}

void Player::calculateScores(Score scores[], uint8_t calcScore[]) {

  uint8_t scoreIdx = 0;

  for (uint8_t i = 1; i < 31; i++) {

    uint8_t possibility[5];
    uint8_t possIdx = 0;

    for (uint8_t j = 0; j < 4; j++) {

      if ((j ^ 2) & i) {

        possibility[possIdx] = calcScore[j];
        possIdx++;

      }


      // Check for pairs ..

      if (possIdx == 2) {

        if (CardUtils::getCardValue(possibility[0], false) == CardUtils::getCardValue(possibility[1], false)) {

          for (uint8_t k = 0; k < 5; k++) {

            scores[scoreIdx].setHand(k, possibility[k]);

          }

          scores[scoreIdx].setScore(2);
          scoreIdx++;

        }

      }


      // Check for 15 ..

      {
        uint8_t total = 0;

        for (uint8_t k = 0; k < possIdx; k++) {

          total = total + CardUtils::getCardValue(possibility[k], true);

        }

        if (total == 15) {

          for (uint8_t k = 0; k < 5; k++) {

            scores[scoreIdx].setHand(k, possibility[k]);

          }

          scores[scoreIdx].setScore(2);
          scoreIdx++;

        }

      }


      // Check for run of 3 ..

      if (possIdx == 3) {

        if (CardUtils::getCardValue(possibility[0], false) + 1 == CardUtils::getCardValue(possibility[1], false) &&   
            CardUtils::getCardValue(possibility[1], false) + 1 == CardUtils::getCardValue(possibility[2], false)) {

          for (uint8_t k = 0; k < 3; k++) {

            scores[scoreIdx].setHand(k, possibility[k]);

          }

          scores[scoreIdx].setScore(3);
          scoreIdx++;

        }

      }


      // Check for run of 4 ..

      if (possIdx == 4) {

        if (CardUtils::getCardValue(possibility[0], false) + 1 == CardUtils::getCardValue(possibility[1], false) && 
            CardUtils::getCardValue(possibility[1], false) + 1 == CardUtils::getCardValue(possibility[2], false) && 
            CardUtils::getCardValue(possibility[2], false) + 1 == CardUtils::getCardValue(possibility[3], false)) {

          bool found = false;
          uint8_t foundIdx = 0;

          for (uint8_t k = 0; k < Constants::PlayerHandScores; k++) {

            if (scores[k].getHand(0) == possibility[0] &&
                scores[k].getHand(1) == possibility[1] &&
                scores[k].getHand(2) == possibility[2]) {

              foundIdx = k;
              found = true;

            }

          }

          if (!found){

            foundIdx = scoreIdx;
            scoreIdx++;

          }

          for (uint8_t k = 0; k < 4; k++) {

            scores[foundIdx].setHand(k, possibility[k]);

          }

          scores[scoreIdx].setScore(4);
          scoreIdx++;

        }

      }


      // Check for run of 5 ..

      if (possIdx == 5) {

        if (CardUtils::getCardValue(possibility[0], false) + 1 == CardUtils::getCardValue(possibility[1], false) && 
            CardUtils::getCardValue(possibility[1], false) + 1 == CardUtils::getCardValue(possibility[2], false) && 
            CardUtils::getCardValue(possibility[2], false) + 1 == CardUtils::getCardValue(possibility[3], false) && 
            CardUtils::getCardValue(possibility[3], false) + 1 == CardUtils::getCardValue(possibility[4], false)) {

          bool found = false;
          uint8_t foundIdx = 0;

          for (uint8_t k = 0; k < Constants::PlayerHandScores; k++) {

            if (scores[k].getHand(0) == possibility[0] &&
                scores[k].getHand(1) == possibility[1] &&
                scores[k].getHand(2) == possibility[2] &&
                scores[k].getHand(3) == possibility[4]) {

              foundIdx = k;
              found = true;

            }

          }

          if (!found){

            foundIdx = scoreIdx;
            scoreIdx++;

          }

          for (uint8_t k = 0; k < 4; k++) {

            scores[foundIdx].setHand(k, possibility[k]);

          }

          scores[scoreIdx].setScore(5);
          scoreIdx++;

        }

      }

    }

  }

}


void Player::discardToCrib(uint8_t &card1, uint8_t &card2) {
  
  card1 = Constants::NoCard;
  card2 = Constants::NoCard;
  
  uint8_t discards[6] = {255, 255, 255, 255, 255, 255 };


  // Never discard 5's ..

  for (uint8_t x = 0; x < 6; x++) {

    if (CardUtils::getCardValue(this->hand[x], false) == 5) discards[x] = 1; // Never

  }


  // Avoid discarding pairs ..

  for (uint8_t x = 0; x < 5; x++) {

    if (CardUtils::getCardValue(this->hand[x], false) == CardUtils::getCardValue(this->hand[x + 1], false)) {
      if (discards[x] == 255) discards[x] = 2; // Avoid
      if (discards[x + 1] == 255) discards[x + 1] = 2; // Avoid
    }

  }


  // Avoid discarding seq ..

  for (uint8_t x = 0; x < 5; x++) {

    for (uint8_t y = x + 1; y < 6; y++) {

      if (CardUtils::getCardValue(this->hand[x], false) + 1 == CardUtils::getCardValue(this->hand[y], false)) {

        if (discards[x] == 255) discards[x] = 3; //avoid
        if (discards[y] == 255) discards[y] = 3; //avoid

      }

    }

  }


  // Avoid small cards ..

  for (uint8_t x = 0; x < 6; x++) {

    if (discards[x] == 255) {

      uint8_t cardValue = CardUtils::getCardValue(hand[x], false);

      switch (cardValue) {

        case 1 ... 4:
          discards[x] = cardValue + 4;
          break;

        case 7 ... 8:
          discards[x] = 6;
          break;

        case 10 ... 13:
          discards[x] = 8;
          break;
          
      }

    }

  }


  // Return two cards ..

  for (uint8_t j = 255; j > 0; j--) {

    for (uint8_t i = 0; i < 5; i++) {

      if (discards[i] == j) {

        if (card1 == Constants::NoCard) { card1 = this->hand[i]; }
        else 
        if (card1 != discards[i] && card2 == Constants::NoCard) { card2 = this->hand[i]; }

      }

    }

  }


  // No cards have been chosen so randomly select some!

  if (card1 == Constants::NoCard) { card1 = random(0, 6); }

  if (card2 == Constants::NoCard) { 

    while (card1 != card2) {
      card2 = random(0, 6); 
    }

  }

  #ifdef COMPUTER_DISCARDS
    Serial.print("Comp Discards: ");
    for (uint8_t x=0; x < 6; x++) {      
      Serial.print(discards[x]);
      Serial.print(" ");
    }
    Serial.println(" > ");
    Serial.print(card1);
    Serial.print(", ");
    Serial.println(card2);
  #endif

}

void Player::addToHand(uint8_t card) {
    
  this->hand[this->handIdx] = card;
  this->handIdx++;

  CardUtils::sort(this->hand, this->handIdx);

}

void Player::addToCrib(uint8_t card) {

  this->crib[this->cribIdx] = card;
  this->cribIdx++;

  CardUtils::sort(this->crib, this->cribIdx);

}

uint8_t Player::removeFromHand(uint8_t index) {

  uint8_t card = this->hand[index];

  for (uint8_t x = index; x < 6; x++) {

    this->hand[x] = this->hand[x + 1];

  }

  this->hand[5] = Constants::NoCard;
  this->handIdx--;

  return card;

}

void Player::resetHand() {

  this->handIdx = 0;
  this->cribIdx = 0;
  
  memset(this->hand, 0, (sizeof(this->hand) / sizeof(this->hand[0])));
  memset(this->crib, 0, (sizeof(this->crib) / sizeof(this->crib[0])));

  this->go = false;

  // for (uint8_t x = 0; x < Constants::PlayerHandScores; x++) {

  //   this->scores[x].reset();

  // }

}


void Player::printHand(uint8_t playerNo) {

  Serial.print("Player ");
  Serial.print(playerNo);
  Serial.print(": ");

  for (uint8_t x=0; x < this->handIdx; x++) {
    
    Serial.print("(");
    Serial.print(this->hand[x]);
    Serial.print(") ");
    Serial.print(CardUtils::getCardValue(this->hand[x], false));
    Suit suit = CardUtils::getCardSuit(this->hand[x]);

    switch (suit) {

      case Suit::Spades:    Serial.print("S, "); break;
      case Suit::Clubs:     Serial.print("C, "); break;
      case Suit::Diamonds:  Serial.print("D, "); break;
      case Suit::Hearts:    Serial.print("H, "); break;

    }

  }

  Serial.println("");
  
}


void Player::playCard(uint8_t playedCards[], uint8_t &card, uint8_t &points) {

  uint8_t discards[6] = {255, 255, 255, 255, 255, 255 };

  uint8_t pointsToDate = 0;
  uint8_t cardsPlayed = 0;

  for (uint8_t x = 0; x < 8; x++) {
    if (playedCards[x] != Constants::NoCard) {
      cardsPlayed = x + 1;
      pointsToDate = pointsToDate + CardUtils::getCardValue(playedCards[x], true);
    }
  }




  // Cards already played ..

Serial.print("Hand Idx: ");
Serial.println(this->handIdx);

  // Convert a pair to a Royal pair ..

  if (cardsPlayed >= 2) {
  
    uint8_t card1 = CardUtils::getCardValue(playedCards[cardsPlayed - 2], false);
    uint8_t card2 = CardUtils::getCardValue(playedCards[cardsPlayed - 1], false);
    uint8_t cardVal = CardUtils::getCardValue(playedCards[cardsPlayed - 2], true);

    if ((card1 == card2) && (pointsToDate + cardVal <= 31)) {
  
      for (uint8_t y = 0; y < this->handIdx; y++) {

        if (card1 == CardUtils::getCardValue(this->hand[y], false)) {
Serial.println("Royal Pair");
          card = removeFromHand(y);
          points = 6;
          return;

        }

      }

    }

  }



  // Convert a pair to a Double Royal pair ..

  if (cardsPlayed >= 3) {
  
    uint8_t card1 = CardUtils::getCardValue(playedCards[cardsPlayed - 3], false);
    uint8_t card2 = CardUtils::getCardValue(playedCards[cardsPlayed - 2], false);
    uint8_t card3 = CardUtils::getCardValue(playedCards[cardsPlayed - 1], false);
    uint8_t cardVal = CardUtils::getCardValue(playedCards[cardsPlayed - 3], true);

    if (card1 == card2 && card1 == card3 && (pointsToDate + cardVal <= 31)) {
  
      for (uint8_t y = 0; y < this->handIdx; y++) {

        if (card1 == CardUtils::getCardValue(this->hand[y], false)) {
Serial.println("Double Royal Pair");
          card = removeFromHand(y);
          points = 12;
          return;

        }

      }

    }

  }


  // Check for 15 or 31 ..

  for (uint8_t y = 0; y < this->handIdx; y++) {
    
    uint8_t total = pointsToDate + CardUtils::getCardValue(this->hand[y], true);

    if (total == 15 || total == 31) {
Serial.println("15 or 31");
      card = removeFromHand(y);
      points = 2;
      return;

    }

  }


  // Check for run of three ..

  if (cardsPlayed >= 3) {

    uint8_t card1 = CardUtils::getCardValue(playedCards[cardsPlayed - 2], false);
    uint8_t card2 = CardUtils::getCardValue(playedCards[cardsPlayed - 1], false);
    
    for (uint8_t y = 0; y < this->handIdx; y++) {
    
      uint8_t card3 = CardUtils::getCardValue(this->hand[y], false);
      uint8_t cardVal = CardUtils::getCardValue(this->hand[y], true);

      if (pointsToDate + cardVal <= 31 &&
          (card1 != card2 && card1 != card3 && card2 != card3) &&
          diffT(card1, card2) <= 2 && diffT(card1, card3) <=2) {
Serial.println("Run of three");
        card = removeFromHand(y);
        points = 3;
        return;

      }

    }

  }


  // Check for run of four ..

  if (cardsPlayed >= 4) {

    uint8_t card1 = CardUtils::getCardValue(playedCards[cardsPlayed - 3], false);
    uint8_t card2 = CardUtils::getCardValue(playedCards[cardsPlayed - 2], false);
    uint8_t card3 = CardUtils::getCardValue(playedCards[cardsPlayed - 1], false);
        
    for (uint8_t y = 0; y < this->handIdx; y++) {

      uint8_t card4 = CardUtils::getCardValue(this->hand[y], false);
      uint8_t cardVal = CardUtils::getCardValue(this->hand[y], true);

      if (pointsToDate + cardVal <= 31 &&
        (card1 != card2 && card1 != card3 && card1 != card4 && card2 != card3 && card2 != card4 && card3 != card4) &&
        diffT(card1, card2) <= 3 && diffT(card1, card3) <=3 && diffT(card1, card4) <=3 && diffT(card2, card3) <=3 && diffT(card2, card4) <=3 && diffT(card3, card4) <=3) {
Serial.println("Run of four");
        card = removeFromHand(y);
        points = 4;
        return;

      }

    }

  }           


  // Make an 11 if you have another 10 in the hand ..
  
  for (uint8_t x = 0; x < this->handIdx; x++) {

    if (pointsToDate + CardUtils::getCardValue(hand[x], true) == 11) {

      for (uint8_t y = 0; y < this->handIdx; y++) {

        if (x != y && CardUtils::getCardValue(hand[y], true) == 10) {

          card = removeFromHand(x);
          points = 0;
          return;
          
        }

      }

    }

  }


  // --------------------------------------------------------------------------------

  // Lead a 4 ..

  if (playedCards[0] == Constants::NoCard) {

    for (uint8_t x = 0; x < this->handIdx; x++) {

      if (CardUtils::getCardValue(hand[x], false) == 4) discards[x] = 50;

    }

  }


  // Do not play a card that has just been played unless you have two or more of them!

  uint8_t lastCard = CardUtils::getCardValue(playedCards[cardsPlayed - 1], false);
  uint8_t sameVal = 0;

  for (uint8_t x = 0; x < this->handIdx; x++) {

    if (CardUtils::getCardValue(hand[x], false) == lastCard) sameVal++;

  }

  if (sameVal == 1) {

    for (uint8_t x = 0; x < this->handIdx; x++) {

      if ((CardUtils::getCardValue(hand[x], false) == lastCard) && (pointsToDate + CardUtils::getCardValue(this->hand[x], true) <= 31)) discards[x] = 1;  // Never

    }

  }


  // Play high cards first ..

  for (uint8_t x = 0; x < this->handIdx; x++) {

    if ((discards[x] == Constants::NoCard) && (pointsToDate + CardUtils::getCardValue(this->hand[x], true) <= 31)) {
      discards[x] = 20 - CardUtils::getCardValue(this->handIdx, false);
    }

  }


  // Return a card ..
Serial.println("Return a card");
  for (uint8_t j = 255; j > 0; j--) {

    for (uint8_t i = 0; i < this->handIdx; i++) {

      if (discards[i] == j && (pointsToDate + CardUtils::getCardValue(this->hand[i], true) <= 31)) {

        card = removeFromHand(i);
        points = 0;
        return;

      }

    }

  }

  card = Constants::NoCard;
  this->go = true;
  return;
  

}

bool Player::canPlay(uint8_t playedCards[]) {

  uint8_t pointsToDate = 0;
  uint8_t cardsPlayed = 0;

  for (uint8_t x = 0; x < 8; x++) {
    if (playedCards[x] != Constants::NoCard) {
      cardsPlayed = x + 1;
      pointsToDate = pointsToDate + CardUtils::getCardValue(playedCards[x], true);
    }
  }

  for (uint8_t i = 0; i < this->handIdx; i++) {

    if (pointsToDate + CardUtils::getCardValue(this->hand[i], true) <= 31) {

      return true;

    }

  }

  return false;

}
