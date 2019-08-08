#pragma once

#include "../utils/GameContext.h"
#include "../utils/GameState.h"
#include "../utils/Enums.h"
#include "../images/Images.h"
#include "BaseState.h"

class DisplayScoreState : public BaseState {

  private:

    enum class ViewState : uint8_t {
      SwapPlayer,
      RollDice
    };

    void drawBoard(int8_t xOffset, int8_t yOffset);
    void drawPlayer_1(int8_t xOffset, int8_t yOffset, uint8_t position, PlayerTile playerTile);

  public:	
  
    void activate(StateMachine & machine) override;
    void update(StateMachine & machine) override;
    void render(StateMachine & machine) override;

  private:

    ViewState viewState = ViewState::RollDice;

    uint8_t player1StartPos = 0;
    uint8_t player1EndPos = 0;
    uint8_t player1Counter = 0;

    int8_t xOffset = 0;
    int8_t yOffset = 0;

};

