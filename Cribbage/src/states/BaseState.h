#pragma once

#include "../utils/GameContext.h"
#include "../utils/GameState.h"
#include "../utils/Enums.h"
#include "../fonts/Font3x5.h"
#include "../fonts/Font4x6.h"

class BaseState : public GameState<GameContext, GameStateType> {

  protected:

    Font3x5 font3x5 = Font3x5();
    Font4x6 font4x6 = Font4x6();

    void renderScore(StateMachine & machine, int16_t score, uint8_t x, uint8_t y);
 
    void drawMessageBox(StateMachine & machine, String message, uint8_t lines, Alignment alignment);
    void drawMessageBox(StateMachine & machine, String message, uint8_t lines, uint8_t width, Alignment alignment);

	private:
   
};