#pragma once

#include "../utils/GameContext.h"
#include "../utils/GameState.h"
#include "../utils/Enums.h"
#include "../fonts/Font3x5.h"

class BaseState : public GameState<GameContext, GameStateType> {

  protected:

    Font3x5 font3x5 = Font3x5();

    void drawScore(StateMachine & machine, uint8_t x, int8_t y, uint8_t score);
    void drawMessageBox(StateMachine & machine, String message, uint8_t lines, BubbleAlignment alignment);
    void drawMessageBox(StateMachine & machine, String message, uint8_t lines, uint8_t width, BubbleAlignment alignment);

	private:
   
};