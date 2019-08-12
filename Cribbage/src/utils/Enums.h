#pragma once

#include <stdint.h>
#include "Utils.h"
#include "../entities/Entities.h"


enum class CribState : uint8_t {
  Empty,
  Hidden,
  Visible
};

enum class TurnUpState : uint8_t {
  Empty,
  Hidden,
  Visible
};

enum class Suit : uint8_t {
  Hearts,
  Diamonds,
  Spades,
  Clubs,
};

enum class PlayerTile : uint8_t {
  Player1,
  Player2,
  Original
};

enum class GameStateType : uint8_t {
	SplashScreen,
	TitleScreen,
  PlayerNames,
  PlayGame,
  DisplayScoreState,
  GameOver,
  HighScore
};

enum class Alignment : uint8_t {
	Player,
	Computer,
  None
};
