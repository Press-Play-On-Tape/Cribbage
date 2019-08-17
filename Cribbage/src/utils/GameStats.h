#pragma once

#include <stdint.h>
#include "Utils.h"
#include "../entities/Entities.h"


// struct Player {

//   public: 
   
//     Player() {

//       for (uint8_t x = 1; x < NAME_LENGTH; x++) {
//         name[x] = ' ';
//       }

//       name[NAME_LENGTH - 1] = 0;

//     };

//     uint8_t getIcon() {
//       return static_cast<uint8_t>(this->name[0]);
//     }

//     void setIcon(uint8_t val) {
//       this->name[0] = static_cast<char>(val);
//     }

//     char name[NAME_LENGTH + 1];
//     int16_t score = 0;

// };

class GameStats {

  public: 

    GameStats() { };

    Player player1;
    Player player2;
    Deck deck;

    WhichPlayer playerDealer = WhichPlayer::Player2;
    WhichPlayer playersTurn = WhichPlayer::Player1;
    Score scores[Constants::PlayerHandScores];


  uint8_t getNumberOfScores() {

    uint8_t count = 0;

    for (uint8_t i = 0; i < Constants::PlayerHandScores; i++) {
      if (scores[i].getCard(0) != Constants::NoCard) count++;
    }

    return count;
    
  }

  uint8_t getScoresTotal() {

    uint8_t total = 0;

    for (uint8_t i = 0; i < Constants::PlayerHandScores; i++) {
      if (scores[i].getCard(0) != Constants::NoCard) total = total + scores[i].getScore();
    }

    return total;
    
  }

  void resetGame() {

    player1.resetHand();
    player2.resetHand();

  }

};