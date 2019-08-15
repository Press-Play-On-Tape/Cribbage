#pragma once

#include "../utils/GameContext.h"
#include "../utils/GameState.h"
#include "../utils/Enums.h"
#include "../images/Images.h"
#include "../locations/Board.h"
#include "BaseState.h"

class PlayGameState : public BaseState {

  private:

    struct Message {
      String message;
      uint8_t lines;
      uint8_t width;
      BubbleAlignment alignment = BubbleAlignment::None;
      bool renderRequired = false;
    };

    enum class ViewState : uint8_t {
      DealCards,
      DiscardCribPlayer,
      DiscardCribComputer,
      TurnUp,
      PlayersTurn,
      PlayersTurn_Normal,
      PlayersTurn_Go,
      ComputersTurn,
      DisplayScore_Board,
      DisplayScore_Other,
      DisplayScore_Dealer,
      DisplayScore_Crib,
    };

    ViewState viewState = ViewState::DealCards;
    ViewState prevViewState = ViewState::DealCards;
    CribState cribState = CribState::Empty;
    Message message;

    uint8_t highlightCard = 0;
    uint8_t counter = 0;
    uint8_t computerDiscard1 = 0;
    uint8_t computerDiscard2 = 0;
    uint8_t playedCards[8];
    uint8_t playIdx;
    uint8_t turnUp;
    bool highlight = true;

  public:	
  
    void activate(StateMachine & machine) override;
    void update(StateMachine & machine) override;
    void render(StateMachine & machine) override;

  private:

    void drawPlayerHands(StateMachine & machine);
    void drawCard(uint8_t xPos, uint8_t yPos, uint8_t card, bool fullSizeCard);
    void drawSmallCard(uint8_t xPos, uint8_t yPos, uint8_t card, bool leftAlign);
    void drawCrib(StateMachine & machine, CribState cribState);
    void drawHighlight(StateMachine & machine, uint8_t hghlightCard);
    void drawTurnUp(StateMachine & machine, TurnUpState turnUpState);
    void drawComputerCard(uint8_t xPos, uint8_t yPos, bool fullSizeCard);
    void drawPlay();
    void drawScores(StateMachine & machine);

    void resetHand(StateMachine & machine);
    void resetPlay(StateMachine & machine);
    void saveMessage(String message, uint8_t lines, BubbleAlignment alignment);
    void saveMessage(String message, uint8_t lines, uint8_t width, BubbleAlignment alignment);
    void saveMessageWithScore(uint8_t playedValue, uint8_t points, BubbleAlignment alignment);
    uint8_t getScore(StateMachine & machine, Player &player, bool player2Go);
    bool isEndOfHand(StateMachine & machine);

    uint8_t getBoardValue();

    void drawPlayer_Upper(uint8_t oldPosition, uint8_t newPosition, bool flash);
    void drawPlayer_Lower(uint8_t oldPosition, uint8_t newPosition, bool flash);

    uint8_t player1Counter = 0;
    uint8_t player2Counter = 0;

};

