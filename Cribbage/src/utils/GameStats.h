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

    uint8_t playersTurn = 0;


  //     Player player3;
  //     Player player4;

  //     Player * players[4] = { &player1, &player2, &player3, &player4 };

  //     uint8_t numberOfPlayers = 1;
  //     uint8_t playerBeingEdited = 1;

  void resetGame() {

    player1.resetHand();
    player2.resetHand();

  //       numberOfPlayers = 1;
  //       playerBeingEdited = 1;

  //       player1.setIcon(1);
  //       player2.setIcon(0);
  //       player3.setIcon(0);
  //       player4.setIcon(0);

  }

};