#include "PlayGameState.h"
#include "../images/Images.h"
#include "PlayGameState.h"
#include "../images/Images.h"


constexpr const static uint8_t DEALER_COMMENT_LENGTH = 64;
constexpr const static uint8_t DEALER_BLINK_IMAGE = 3;
constexpr const static uint8_t DEALER_COMMENT_YPOS_TOP = 6;
constexpr const static uint8_t DEALER_COMMENT_YPOS_MID = 19;
constexpr const static uint8_t DEALER_COMMENT_YPOS_BOT = 33;

constexpr const static uint8_t CARD_LARGE_SPACING = 12;
constexpr const static uint8_t CARD_LARGE_SPACING_DEALER = 12;
constexpr const static uint8_t CARD_LARGE_SPACING_FULL = 22;
constexpr const static uint8_t CARD_LARGE_SPACING_ROTATED = 9;
constexpr const static uint8_t CARD_HAND_SPACING = 12;
constexpr const static uint8_t CARD_DEALER_CENTER = 58;
constexpr const static uint8_t CARD_PLAYER_CENTER = 58;
constexpr const static uint8_t CARD_LARGE_TOP_PLAYER = 41;
constexpr const static uint8_t CARD_SMALL_TOP_PLAYER = 37;
constexpr const static uint8_t CARD_LARGE_TOP_DEALER = 0;
constexpr const static uint8_t PLAY_CENTER = 62;


// ---------------------------------------------------------------------------------------------------------------------------
//  Render the state .. 
// ---------------------------------------------------------------------------------------------------------------------------
// 
void PlayGameState::render(StateMachine & machine) {

	auto & arduboy = machine.getContext().arduboy;
	auto & ardBitmap = machine.getContext().ardBitmap;

  ardBitmap.drawCompressed(87, 18, Images::Dealer_Mask, BLACK, ALIGN_NONE, MIRROR_NONE);
  ardBitmap.drawCompressed(87, 18, Images::Dealer_BlankFace, WHITE, ALIGN_NONE, MIRROR_NONE);
  ardBitmap.drawCompressed(87 + 12, 28, Images::Dealer_Faces[2], WHITE, ALIGN_NONE, MIRROR_NONE);

  drawPlayerHands(machine);
  arduboy.drawFastHLine(0, 61, 128);
  arduboy.drawFastHLine(0, 60, 128, BLACK);
  arduboy.drawHorizontalDottedLine(0, 127, 63);

	switch (this->viewState) {

		case ViewState::DealCards:
      drawCrib(machine, this->cribState);
      drawTurnUp(machine, TurnUpState::Empty);
			break;

		case ViewState::DiscardCribPlayer:
      drawHighlight(machine, this->highlightCard);
      drawTurnUp(machine, TurnUpState::Empty);
      drawCrib(machine, this->cribState);
			break;

		case ViewState::DiscardCribComputer:
      drawTurnUp(machine, TurnUpState::Empty);
      drawCrib(machine, this->cribState);
			break;

    case ViewState::TurnUp:
      drawTurnUp(machine, (this->turnUp != Constants::NoCard ? TurnUpState::Visible : TurnUpState::Hidden));
      drawCrib(machine, this->cribState);
			break;

    case ViewState::PlayersTurn:
      drawSmallCard(machine, 0, 0, this->turnUp, false);
      drawHighlight(machine, this->highlightCard);
      drawPlay(machine);
      drawCrib(machine, this->cribState);
			break;

    case ViewState::ComputersTurn:
      drawSmallCard(machine, 0, 0, this->turnUp, false);
      drawHighlight(machine, this->highlightCard);
      drawPlay(machine);
      drawCrib(machine, this->cribState);
			break;

	}

  // arduboy.fillRect(0, 52, WIDTH, HEIGHT, BLACK);
  // ardBitmap.drawCompressed(0, 51, Images::Background, WHITE, ALIGN_NONE, MIRROR_NONE); 
  // drawPlayerHands_Lines(machine);
	// drawButtons(machine);
  // drawStats(machine, this->highlightEndOfGame);

  // if (!dealer.noComment()) renderDealer(machine);

  if (this->message.renderRequired) {
    drawMessageBox(machine, this->message.message, this->message.lines, this->message.width, this->message.alignment);
    this->message.renderRequired = false;
  }

}

void PlayGameState::drawPlayerHands(StateMachine & machine) {

  auto & gameStats = machine.getContext().gameStats;

  uint8_t leftHand = 0;
  uint8_t widthTot = (gameStats.player1.getHandCardCount() * CARD_LARGE_SPACING) + (CARD_LARGE_SPACING_FULL - CARD_LARGE_SPACING);


  // Determine left hand side of each hand ..

  leftHand = CARD_PLAYER_CENTER - (widthTot / 2);


  // Render hand ..

  for (uint8_t x = 0; x < gameStats.player1.getHandCardCount(); x++) {
    
    if (x < gameStats.player1.getHandCardCount() - 1) {
      drawCard(machine, leftHand + (x * CARD_LARGE_SPACING), CARD_LARGE_TOP_PLAYER, gameStats.player1.getHandCard(x), false);   
    }
    else {
      drawCard(machine, leftHand + (x * CARD_LARGE_SPACING), CARD_LARGE_TOP_PLAYER, gameStats.player1.getHandCard(x), true);   
    }
    
  }



  // Computer ..

  uint8_t width = (gameStats.player2.getHandCardCount() * CARD_LARGE_SPACING_DEALER) + (CARD_LARGE_SPACING_FULL - CARD_LARGE_SPACING_DEALER);
  uint8_t rightHandSide = CARD_DEALER_CENTER + (width / 2);

  for (int x = 0; x < gameStats.player2.getHandCardCount(); x++) {
			
		if (x < gameStats.player2.getHandCardCount() - 1) {

			drawComputerCard(machine, rightHandSide - (x * CARD_LARGE_SPACING_DEALER), CARD_LARGE_TOP_DEALER, false);   

		}
		else {

			drawComputerCard(machine, rightHandSide - (x * CARD_LARGE_SPACING_DEALER) + 2, CARD_LARGE_TOP_DEALER, true);   

		}
    
  }
      
}

void PlayGameState::drawComputerCard(StateMachine & machine, uint8_t xPos, uint8_t yPos, bool fullSizeCard) {

	auto & ardBitmap = machine.getContext().ardBitmap;

	if (fullSizeCard) {

		ardBitmap.drawCompressed(xPos - CARD_LARGE_SPACING_FULL, yPos - 17, Images::Card_Outline_Full, WHITE, ALIGN_NONE, MIRROR_HOR_VER);
		ardBitmap.drawCompressed(xPos - 18, yPos - 11, Images::Card_Background_Full, BLACK, ALIGN_NONE, MIRROR_NONE);

	}
	else {

		ardBitmap.drawCompressed(xPos - CARD_LARGE_SPACING, yPos - 17, Images::Card_Outline_Half, WHITE, ALIGN_NONE, MIRROR_HOR_VER);
		ardBitmap.drawCompressed(xPos - 7, yPos - 11, Images::Card_Background_Half, BLACK, ALIGN_NONE, MIRROR_NONE);

	}

}

void PlayGameState::drawCard(StateMachine & machine, uint8_t xPos, uint8_t yPos, uint8_t card, bool fullSizeCard) {

	auto & ardBitmap = machine.getContext().ardBitmap;

	uint8_t cardNumber = card % 13;

  if (fullSizeCard) {

		ardBitmap.drawCompressed(xPos, yPos, Images::Card_Outline_Full, WHITE, ALIGN_NONE, MIRROR_NONE);
		ardBitmap.drawCompressed(xPos + 8, yPos + 11, Images::Suits[card / 13], BLACK, ALIGN_NONE, MIRROR_NONE);
		ardBitmap.drawCompressed(xPos + 3, yPos + 4, Images::Pips[cardNumber], BLACK, ALIGN_NONE, MIRROR_NONE);

	}
  else {

		ardBitmap.drawCompressed(xPos, yPos, Images::Card_Outline_Half, WHITE, ALIGN_NONE, MIRROR_NONE);
		ardBitmap.drawCompressed(xPos + 3, yPos + 11, Images::Suits[card / 13], BLACK, ALIGN_NONE, MIRROR_NONE);
		ardBitmap.drawCompressed(xPos + 3, yPos + 4, Images::Pips[cardNumber], BLACK, ALIGN_NONE, MIRROR_NONE);

  }

}

void PlayGameState::drawSmallCard(StateMachine & machine, uint8_t xPos, uint8_t yPos, uint8_t card, bool leftAlign) {

	auto & ardBitmap = machine.getContext().ardBitmap;
	uint8_t cardNumber = card % 13;

  uint8_t xOffset = (leftAlign ? 4 : 6);

  SpritesB::drawExternalMask(xPos, yPos, Images::TurnUp, Images::TurnUp_Mask, 2, 0);
  ardBitmap.drawCompressed(xPos + xOffset, yPos + 12, Images::Suits[card / 13], BLACK, ALIGN_NONE, MIRROR_NONE);
  ardBitmap.drawCompressed(xPos + xOffset, yPos + 5, Images::Pips[cardNumber], BLACK, ALIGN_NONE, MIRROR_NONE);

}

void PlayGameState::drawHighlight(StateMachine & machine, uint8_t hghlightCard) {

  auto & gameStats = machine.getContext().gameStats;

  uint8_t leftHand = 0;
  uint8_t widthTot = (gameStats.player1.getHandCardCount() * CARD_LARGE_SPACING) + (CARD_LARGE_SPACING_FULL - CARD_LARGE_SPACING);


  // Determine left hand side of each hand ..

  leftHand = CARD_PLAYER_CENTER - (widthTot / 2);

  if (gameStats.player1.getHandCardCount() - 1 == hghlightCard) {
    SpritesB::drawOverwrite(leftHand + (hghlightCard * CARD_LARGE_SPACING) - 1, 60, Images::Highlight_Large, 0);
  }
  else {
    SpritesB::drawOverwrite(leftHand + (hghlightCard * CARD_LARGE_SPACING) - 1, 60, Images::Highlight_Small, 0);
  }

}

void PlayGameState::drawCrib(StateMachine & machine, CribState cribState) {

  auto & gameStats = machine.getContext().gameStats;
  Player player;

  if (gameStats.playersTurn == 0) {
    player = gameStats.player1;
  }
  else {
    player = gameStats.player2;
  }


  uint8_t yPos = (gameStats.playersTurn == 0 ? 37 : 0);

  switch (cribState) {

    case CribState::Empty:
      SpritesB::drawSelfMasked(0, yPos, Images::Crib_Blank, 0);
      break;

    case CribState::Hidden:

      if (player.getCribCardCount() > 0) {
        
        SpritesB::drawSelfMasked(0, yPos, Images::Crib_First, 0);

        for (uint8_t x = 1; x < player.getCribCardCount(); x++) {
      
          SpritesB::drawSelfMasked(2 + (x * 2), yPos + 2 + (x * 2), Images::Crib_Outline, 0);

        }

      }
      break;

    case CribState::Visible:
      break;

  }

}

void PlayGameState::drawTurnUp(StateMachine & machine, TurnUpState turnUpState) {

  auto & gameStats = machine.getContext().gameStats;

  uint8_t yPos = (gameStats.playersTurn == 0 ? 0: 37);

  switch (turnUpState) {

    case TurnUpState::Empty:
      SpritesB::drawSelfMasked(0, yPos, Images::TurnUp, 0);
      break;

    case TurnUpState::Hidden:
      SpritesB::drawSelfMasked(0, yPos, Images::TurnUp, 1);
      break;

    case TurnUpState::Visible:
      drawSmallCard(machine, 0, yPos, this->turnUp, false);
      break;

  }

}

void PlayGameState::drawPlay(StateMachine & machine) {

  uint8_t xLeft = PLAY_CENTER - ((16 + (playIdx * 10)) / 2);

  for (uint8_t x = 0; x < 8; x++) {

    bool leftAlign = !(x == 8 || x == playIdx - 1);

    if (this->playedCards[x] != Constants::NoCard) {

      drawSmallCard(machine, xLeft + (x * 10), 13, this->playedCards[x], leftAlign);

    }

  }

}
