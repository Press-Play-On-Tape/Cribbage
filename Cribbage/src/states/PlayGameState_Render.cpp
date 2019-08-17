#include "PlayGameState.h"
#include "../images/Images.h"
#include "../utils/CardUtils.h"


constexpr const static uint8_t DEALER_COMMENT_LENGTH = 64;
constexpr const static uint8_t DEALER_BLINK_IMAGE = 3;
constexpr const static uint8_t DEALER_COMMENT_YPOS_TOP = 6;
constexpr const static uint8_t DEALER_COMMENT_YPOS_MID = 19;
constexpr const static uint8_t DEALER_COMMENT_YPOS_BOT = 33;

constexpr const static uint8_t CARD_LARGE_SPACING = 10;
constexpr const static uint8_t CARD_LARGE_SPACING_DEALER = 10;
constexpr const static uint8_t CARD_LARGE_SPACING_FULL = 22;
constexpr const static uint8_t CARD_LARGE_SPACING_ROTATED = 9;
constexpr const static uint8_t CARD_HAND_SPACING = 12;
constexpr const static uint8_t CARD_DEALER_CENTER = 54;
constexpr const static uint8_t CARD_PLAYER_CENTER = 52;
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
  auto & gameStats = machine.getContext().gameStats;
	auto & player1 = gameStats.player1;
	auto & player2 = gameStats.player2;

  bool flashScore = arduboy.getFrameCountHalf(32);

	switch (this->viewState) {

    case ViewState::DisplayScore_Board:
    case ViewState::DisplayScore_Other:
    case ViewState::DisplayScore_Dealer:
    case ViewState::DisplayScore_Crib:
      break;

    default:
      SpritesB::drawOverwrite(87, 18, Images::Dealer, 0);
      drawPlayerHands(machine);
      arduboy.drawFastHLine(0, 61, 128);
      arduboy.drawFastHLine(0, 60, 128, BLACK);
      arduboy.drawHorizontalDottedLine(0, 127, 63);
      break;

  }

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
    case ViewState::PlayersTurn_Go:
    case ViewState::PlayersTurn_Normal:
      drawTurnUp(machine, TurnUpState::Visible);
      if (this->counter == 0) drawHighlight(machine, this->highlightCard);
      drawPlay();
      drawCrib(machine, this->cribState);
			break;

    case ViewState::ComputersTurn:
      drawTurnUp(machine, TurnUpState::Visible);
//      drawHighlight(machine, this->highlightCard);
      drawPlay();
      drawCrib(machine, this->cribState);
			break;

    case ViewState::DisplayScore_Board:
      {
        SpritesB::drawOverwrite(0, 22, Images::Dealer, 0);
        SpritesB::drawSelfMasked(43, 0, Images::Divider, 0);
        SpritesB::drawSelfMasked(51, 4, Images::Board, 0);

        bool playerFlash1 = (player1.getPrevScore() != player1.getScore()) && this->highlight;
        bool playerFlash2 = (player2.getPrevScore() != player2.getScore()) && this->highlight;
        this->drawPlayer_Upper(player2.getPrevScore(), this->player2Counter, playerFlash2);
        this->drawPlayer_Lower(player1.getPrevScore(), this->player1Counter, playerFlash1);
        this->drawScores_TopLeft(machine, true, true);

      }
      break;

    case ViewState::DisplayScore_Other:
    case ViewState::DisplayScore_Dealer:
    case ViewState::DisplayScore_Crib:
      {
        SpritesB::drawOverwrite(0, 22, Images::Dealer, 0);
        SpritesB::drawSelfMasked(43, 0, Images::Divider, 0);


        // Flash the appropriate player's score ..

        switch (this->viewState) {

          case ViewState::DisplayScore_Other:
            if (gameStats.playerDealer == 0) {
              this->drawScores_TopLeft(machine, true, flashScore);
            }
            else {
              this->drawScores_TopLeft(machine, flashScore, true);
            }
            break;

          case ViewState::DisplayScore_Dealer:
            if (gameStats.playerDealer == 0) {
              this->drawScores_TopLeft(machine, flashScore, true);
            }
            else {
              this->drawScores_TopLeft(machine, true, flashScore);
            }
            break;

          case ViewState::DisplayScore_Crib: 
            if (gameStats.playerDealer == 0) {
              this->drawScores_TopLeft(machine, flashScore, true);
            }
            else {
              this->drawScores_TopLeft(machine, true, flashScore);
            }
            break;

          default: break;

        }

        switch (this->counter) {

          case 50:
            drawHandScores(machine);
            break;

          default:

            SpritesB::drawSelfMasked(51, 4, Images::Board, 0);
            bool playerFlash1 = (player1.getPrevScore() != player1.getScore()) && this->highlight;
            bool playerFlash2 = (player2.getPrevScore() != player2.getScore()) && this->highlight;
            this->drawPlayer_Upper(player2.getPrevScore(), this->player2Counter, playerFlash2);
            this->drawPlayer_Lower(player1.getPrevScore(), this->player1Counter, playerFlash1);
            break;

        }

      }
      break;

	}

  switch (this->viewState) {
    
    case ViewState::DisplayScore_Board:
    case ViewState::DisplayScore_Other:
    case ViewState::DisplayScore_Dealer:
    case ViewState::DisplayScore_Crib:
      break;

    default:
      drawScore(machine, 114, -1, player2.getScore(), true);
      drawScore(machine, 114, 52, player1.getScore(), true);
      break;

  }

  if (this->message.renderRequired) {
    drawMessageBox(machine, this->message.message, this->message.lines, this->message.width, this->message.alignment);
    this->message.renderRequired = false;
  }

}


// Draw scoring sequences from hand or crib in RHS ..

void PlayGameState::drawHandScores(StateMachine & machine) {

	auto & arduboy = machine.getContext().arduboy;
  auto & gameStats = machine.getContext().gameStats;


  // Render hand details ..

  font3x5.setCursor(51, 0);

  switch (this->viewState) {

    case ViewState::DisplayScore_Other:
      if (gameStats.playerDealer == 0) {
        font3x5.print(F("My Hand:"));
      }
      else {
        font3x5.print(F("Your Hand:"));
      }
      break;

    case ViewState::DisplayScore_Dealer:
      if (gameStats.playerDealer == 0) {
        font3x5.print(F("Your Hand:"));
      }
      else {
        font3x5.print(F("My Hand:"));
      }
      break;

    case ViewState::DisplayScore_Crib: 
      if (gameStats.playerDealer == 0) {
        font3x5.print(F("Your Crib:"));
      }
      else {
        font3x5.print(F("My Crib:"));
      }
      break;

    default: break;

  }



  if (gameStats.scores[0].getCard(0) == Constants::NoCard) {

    font3x5.setCursor(72, 28);
    font3x5.print(F("Nothing !"));

  }
  else {

    uint8_t numberOfScores = gameStats.getNumberOfScores();
    uint8_t renderLine = 0;

    // Render upper arrow if needed ..

    if (numberOfScores > 4) {
      SpritesB::drawSelfMasked(120, 1, Images::Arrow_Up, !this->scoreUpperRow);
    }

    for (uint8_t i = 0; i < numberOfScores; i++) {

      if (i >= this->scoreUpperRow && i <= this->scoreUpperRow + 5) {

        uint8_t handWidth = 0;

        for (uint8_t j = 0; j < 5; j++) {

          if (gameStats.scores[i].getCard(j) != Constants::NoCard) {

            handWidth++;
            uint8_t cardVal = CardUtils::getCardValue(gameStats.scores[i].getCard(j), false);
            SpritesB::drawSelfMasked(50 + (j * 14) - (cardVal == 10 ? 1 : 0), 10 + (renderLine * 8), Images::Pips[cardVal - 1], 0);
            SpritesB::drawSelfMasked(55 + (j * 14), 10 + (renderLine * 8), Images::Suits, static_cast<uint8_t>(CardUtils::getCardSuit(gameStats.scores[i].getCard(j))));

          }

        } 

        arduboy.drawHorizontalDottedLine(50 + (handWidth * 14), 114, 15 + (renderLine * 8));

        uint8_t score = gameStats.scores[i].getScore();
        font3x5.setCursor(120, 10 + (renderLine * 8) - 1);
        if (score < 10) font3x5.print("  ");
        font3x5.print(score);
        renderLine++;

      }

    }


    // Render lower arrow ..
    if (numberOfScores > 4) {
      SpritesB::drawSelfMasked(120, 60, Images::Arrow_Down, !(this->scoreUpperRow + 5 < numberOfScores));
    }
    if (this->scoreUpperRow + 5 >= numberOfScores) {
      font3x5.setTextColor(WHITE);
      font3x5.setCursor(93, 10 + (renderLine * 8));
      font3x5.print(F("Total:"));
      font3x5.setCursor(120, 10 + (renderLine * 8));
      if (gameStats.getScoresTotal() < 10) font3x5.print("  ");
      font3x5.print(gameStats.getScoresTotal());
    }

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
      drawCard(leftHand + (x * CARD_LARGE_SPACING), CARD_LARGE_TOP_PLAYER, gameStats.player1.getHandCard(x), false);   
    }
    else {
      drawCard(leftHand + (x * CARD_LARGE_SPACING), CARD_LARGE_TOP_PLAYER, gameStats.player1.getHandCard(x), true);   
    }
    
  }



  // Computer ..

  uint8_t width = (gameStats.player2.getHandCardCount() * CARD_LARGE_SPACING_DEALER) + (CARD_LARGE_SPACING_FULL - CARD_LARGE_SPACING_DEALER);
  uint8_t rightHandSide = CARD_DEALER_CENTER + (width / 2);

  for (uint8_t x = 0; x < gameStats.player2.getHandCardCount(); x++) {
			
		if (x < gameStats.player2.getHandCardCount() - 1) {

			drawComputerCard(rightHandSide - (x * CARD_LARGE_SPACING_DEALER), CARD_LARGE_TOP_DEALER, false);   

		}
		else {

			drawComputerCard(rightHandSide - (x * CARD_LARGE_SPACING_DEALER) + 2, CARD_LARGE_TOP_DEALER, true);   

		}
    
  }
      
}

void PlayGameState::drawComputerCard(uint8_t xPos, uint8_t yPos, bool fullSizeCard) {

	if (fullSizeCard) {

		SpritesB::drawSelfMasked(xPos - CARD_LARGE_SPACING_FULL, yPos - 8, Images::Computer_Full, 0);

	}
	else {

	  SpritesB::drawSelfMasked(xPos - CARD_LARGE_SPACING, yPos - 8, Images::Computer_Half, 0);

	}

}

void PlayGameState::drawCard(uint8_t xPos, uint8_t yPos, uint8_t card, bool fullSizeCard) {

	uint8_t cardNumber = card % 13;

  if (fullSizeCard) {

		SpritesB::drawSelfMasked(xPos, yPos, Images::Card_Outline_Full, 0);
		SpritesB::drawErase(xPos + 8, yPos + 11, Images::Suits, card / 13);
		SpritesB::drawErase(xPos + 3, yPos + 4, Images::Pips[cardNumber], 0);

	}
  else {

		SpritesB::drawSelfMasked(xPos, yPos, Images::Card_Outline_Half, 0);
		SpritesB::drawErase(xPos + 3, yPos + 11, Images::Suits, card / 13);
		SpritesB::drawErase(xPos + 3, yPos + 4, Images::Pips[cardNumber], 0);

  }

}

void PlayGameState::drawSmallCard(uint8_t xPos, uint8_t yPos, uint8_t card, bool leftAlign) {

	uint8_t cardNumber = CardUtils::getCardValue(card, false);
	Suit suit = CardUtils::getCardSuit(card);

  uint8_t xOffset = (leftAlign ? 4 : 6);

  SpritesB::drawExternalMask(xPos, yPos, Images::TurnUp, Images::TurnUp_Mask, 2, 0);
  SpritesB::drawErase(xPos + xOffset, yPos + 12, Images::Suits, static_cast<uint8_t>(suit));
  SpritesB::drawErase(xPos + xOffset + (cardNumber == 10 ? 0 : 1), yPos + 5, Images::Pips[cardNumber - 1], 0);

}

void PlayGameState::drawHighlight(StateMachine & machine, uint8_t hghlightCard) {

  auto & gameStats = machine.getContext().gameStats;

  uint8_t leftHand = 0;
  uint8_t widthTot = (gameStats.player1.getHandCardCount() * CARD_LARGE_SPACING) + (CARD_LARGE_SPACING_FULL - CARD_LARGE_SPACING);


  // Determine left hand side of each hand ..

  leftHand = CARD_PLAYER_CENTER - (widthTot / 2);

  // if (gameStats.player1.getHandCardCount() - 1 == hghlightCard) {
  //   SpritesB::drawOverwrite(leftHand + (hghlightCard * CARD_LARGE_SPACING) - 1, 60, Images::Highlight_Large, 0);
  // }
  // else {
  //   SpritesB::drawOverwrite(leftHand + (hghlightCard * CARD_LARGE_SPACING) - 1, 60, Images::Highlight_Small, 0);
  // }

  SpritesB::drawExternalMask(leftHand + (hghlightCard * CARD_LARGE_SPACING) + 1, 51, Images::Hand, Images::Hand_Mask, 0, 0);

}

void PlayGameState::drawCrib(StateMachine & machine, CribState cribState) {

  auto & gameStats = machine.getContext().gameStats;
  Player player;

  if (gameStats.playerDealer == 0) {
    player = gameStats.player1;
  }
  else {
    player = gameStats.player2;
  }


  uint8_t yPos = (gameStats.playerDealer == 0 ? 37 : 0);

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

  uint8_t yPos = (gameStats.playerDealer == 0 ? 0 : 37);

  switch (turnUpState) {

    case TurnUpState::Empty:
      //SpritesB::drawSelfMasked(0, yPos, Images::TurnUp, 0);
      break;

    case TurnUpState::Hidden:
      SpritesB::drawSelfMasked(0, yPos, Images::TurnUp, 1);
      break;

    case TurnUpState::Visible:
      drawSmallCard(0, yPos, this->turnUp, false);
      break;

  }

}

void PlayGameState::drawPlay() {

  uint8_t xLeft = PLAY_CENTER - ((16 + (playIdx * 10)) / 2);

  for (uint8_t x = 0; x < 8; x++) {

    bool leftAlign = !(x == 8 || x == playIdx - 1);

    if (this->playedCards[x] != Constants::NoCard) {

      drawSmallCard(xLeft + (x * 10), 13, this->playedCards[x], leftAlign);

    }

  }

}

void PlayGameState::drawPlayer_Upper(uint8_t oldPosition, uint8_t newPosition, bool flash) {

  uint8_t oldX = pgm_read_byte(&Board_Positions_Player_1[oldPosition * 2]);
  uint8_t oldY = pgm_read_byte(&Board_Positions_Player_1[oldPosition * 2] + 1);

  uint8_t newX = pgm_read_byte(&Board_Positions_Player_1[newPosition * 2]);
  uint8_t newY = pgm_read_byte(&Board_Positions_Player_1[newPosition * 2] + 1);

  uint8_t oldFrame = 0;
  uint8_t newFrame = 0;

  if (oldY >= 128) {
    oldFrame = 1;
    oldY = oldY & 0x7F;
  }

  if (newY >= 128) {
    newFrame = 1;
    newY = newY & 0x7F;
  }

  SpritesB::drawExternalMask(oldX, oldY, Images::Peg, Images::Peg_Mask, 0, oldFrame);
  if (flash) SpritesB::drawExternalMask(newX, newY, Images::Peg, Images::Peg_Mask, 0, newFrame);

}

void PlayGameState::drawPlayer_Lower(uint8_t oldPosition, uint8_t newPosition, bool flash) {

  uint8_t oldX = pgm_read_byte(&Board_Positions_Player_2[oldPosition * 2]);
  uint8_t oldY = pgm_read_byte(&Board_Positions_Player_2[oldPosition * 2] + 1);

  uint8_t newX = pgm_read_byte(&Board_Positions_Player_2[newPosition * 2]);
  uint8_t newY = pgm_read_byte(&Board_Positions_Player_2[newPosition * 2] + 1);

  uint8_t oldFrame = 0;
  uint8_t newFrame = 0;

  if (oldY >= 128) {
    oldFrame = 1;
    oldY = oldY & 0x7F;
  }

  if (newY >= 128) {
    newFrame = 1;
    newY = newY & 0x7F;
  }

  SpritesB::drawExternalMask(oldX, oldY, Images::Peg, Images::Peg_Mask, 1, oldFrame);
  if (flash) SpritesB::drawExternalMask(newX, newY, Images::Peg, Images::Peg_Mask, 1, newFrame);

}
