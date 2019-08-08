#pragma once

#include "../utils/GameContext.h"
#include "../utils/GameState.h"
#include "../utils/Enums.h"
#include "../images/Images.h"
#include "BaseState.h"

class PlayGameState : public BaseState {

  private:

    enum class ViewState : uint8_t {
      SwapPlayer,
      RollDice
    };

    ViewState viewState = ViewState::RollDice;

  public:	
  
    void activate(StateMachine & machine) override;
    void update(StateMachine & machine) override;
    void render(StateMachine & machine) override;

};

