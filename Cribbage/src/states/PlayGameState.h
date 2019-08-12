#pragma once

#include "../utils/GameContext.h"
#include "../utils/GameState.h"
#include "../utils/Enums.h"
#include "../images/Images.h"
#include "BaseState.h"

class PlayGameState : public BaseState {

  private:

    struct Message {
      String message;
      uint8_t lines;
      uint8_t width;
      Alignment alignment = Alignment::None;
      bool renderRequired = false;
    };

    enum class ViewState : uint8_t {
      DealCards,
      DiscardCribPlayer,
      DiscardCribComputer,
      TurnUp,
      PlayersTurn,
      ComputersTurn
    };

    ViewState viewState = ViewState::DealCards;
    CribState cribState = CribState::Empty;
    Message message;

    uint8_t highlightCard = 0;
    uint8_t counter = 0;
    uint8_t computerDiscard1 = 0;
    uint8_t computerDiscard2 = 0;
    uint8_t play[8];
    uint8_t playIdx;
    uint8_t turnUp;

  public:	
  
    void activate(StateMachine & machine) override;
    void update(StateMachine & machine) override;
    void render(StateMachine & machine) override;

  private:

    void drawPlayerHands(StateMachine & machine);
    void drawCard(StateMachine & machine, uint8_t xPos, uint8_t yPos, uint8_t card, bool fullSizeCard);
    void drawSmallCard(StateMachine & machine, uint8_t xPos, uint8_t yPos, uint8_t card, bool leftAlign);
    void drawCrib(StateMachine & machine, CribState cribState);
    void drawHighlight(StateMachine & machine, uint8_t hghlightCard);
    void drawTurnUp(StateMachine & machine, TurnUpState turnUpState);
    void drawComputerCard(StateMachine & machine, uint8_t xPos, uint8_t yPos, bool fullSizeCard);
    void drawPlay(StateMachine & machine);

    void resetHand(StateMachine & machine);
    void saveMessage(String message, uint8_t lines, Alignment alignment);
    void saveMessage(String message, uint8_t lines, uint8_t width, Alignment alignment);

    uint8_t playValue();

};

