#include "DisplayScoreState.h"
#include "../images/Images.h"
#include "../utils/Constants.h"


const uint8_t PROGMEM Board_Positions_Player_1[] = {
  12, 2,
  19, 2,
  22, 2,
  25, 2,
  28, 2,
  31, 2, // 5

  37, 2,
  40, 2,
  43, 2,
  46, 2,
  49, 2, // 10

  55, 2,
  58, 2,
  61, 2,
  64, 2,
  67, 2, // 15

  73, 2,
  76, 2,
  79, 2,
  82, 2,
  85, 2, // 20

  91, 2,
  94, 2,
  97, 2,
  100, 2,
  103, 2, // 25

  109, 2,
  112, 2,
  115, 2,
  118, 2,
  121, 2, // 30

  127, 2,
  130, 2,
  133, 2,
  136, 2,
  139, 2, // 35

  147, 2,
  154, 4,
  159, 8,
  163, 13,
  165, 19, // 40

  165, 26,
  163, 32,
  159, 37,
  154, 41,
  147, 43, // 45

  139, 43,
  136, 43,
  133, 43,
  130, 43,
  127, 43, // 50

  121, 43,
  118, 43,
  115, 43,
  112, 43,
  109, 43, // 55

  103, 43,
  100, 43,
  97, 43,
  94, 43,
  91, 43, // 60

  85, 43,
  82, 43,
  79, 43,
  76, 43,
  73, 43, // 65

  67, 43,
  64, 43,
  61, 43,
  58, 43,
  55, 43, // 70

  49, 43,
  46, 43,
  43, 43,
  40, 43,
  37, 43, // 75

  31, 43,
  28, 43,
  25, 43,
  22, 43,
  19, 43, // 80

  13, 43,
  5, 40,
  1, 31,
  5, 23,
  13, 20, // 85

  19, 20,
  22, 20,
  25, 20,
  28, 20,
  31, 20, // 90

  37, 20,
  40, 20,
  43, 20,
  46, 20,
  49, 20, // 95

  55, 20,
  58, 20,
  61, 20,
  64, 20,
  67, 20, // 100

  73, 20,
  76, 20,
  79, 20,
  82, 20,
  85, 20, // 105

  91, 20,
  94, 20,
  97, 20,
  100, 20,
  103, 20, // 110

  109, 20,
  112, 20,
  115, 20,
  118, 20,
  121, 20, // 115

  127, 20,
  130, 20,
  133, 20,
  136, 20,
  139, 20, // 120

  145, 20, // 121

};


const uint8_t PROGMEM Board_Positions_Player_2[] = {
  12, 6,
  19, 6,
  22, 6,
  25, 6,
  28, 6,
  31, 6, // 5

  37, 6,
  40, 6,
  43, 6,
  46, 6,
  49, 6, // 10

  55, 6,
  58, 6,
  61, 6,
  64, 6,
  67, 6, // 15

  73, 6,
  76, 6,
  79, 6,
  82, 6,
  85, 6, // 20

  91, 6,
  94, 6,
  97, 6,
  100, 6,
  103, 6, // 25

  109, 6,
  112, 6,
  115, 6,
  118, 6,
  121, 6, // 30

  127, 6,
  130, 6,
  133, 6,
  136, 6,
  139, 6, // 35

  147, 6,
  152, 8,
  156, 11,
  159, 15,
  160, 19, // 40

  160, 26,
  159, 30,
  156, 34,
  152, 37,
  147, 39, // 45

  139, 39,
  136, 39,
  133, 39,
  130, 39,
  127, 39, // 50

  121, 39,
  118, 39,
  115, 39,
  112, 39,
  109, 39, // 55

  103, 39,
  100, 39,
  97, 39,
  94, 39,
  91, 39, // 60

  85, 39,
  82, 39,
  79, 39,
  76, 39,
  73, 39, // 65

  67, 39,
  64, 39,
  61, 39,
  58, 39,
  55, 39, // 70

  49, 39,
  46, 39,
  43, 39,
  40, 39,
  37, 39, // 75

  31, 39,
  28, 39,
  25, 39,
  22, 39,
  19, 39, // 80

  13, 39,
  8, 37,
  6, 31,
  8, 26,
  13, 24, // 85

  19, 24,
  22, 24,
  25, 24,
  28, 24,
  31, 24, // 90

  37, 24,
  40, 24,
  43, 24,
  46, 24,
  49, 24, // 95

  55, 24,
  58, 24,
  61, 24,
  64, 24,
  67, 24, // 100

  73, 24,
  76, 24,
  79, 24,
  82, 24,
  85, 24, // 105

  91, 24,
  94, 24,
  97, 24,
  100, 24,
  103, 24, // 110

  109, 24,
  112, 24,
  115, 24,
  118, 24,
  121, 24, // 115

  127, 24,
  130, 24,
  133, 24,
  136, 24,
  139, 24, // 120

  145, 24, // 121

};



// ----------------------------------------------------------------------------
//  Initialise state ..
//
void DisplayScoreState::activate(StateMachine & machine) {

	auto & arduboy = machine.getContext().arduboy;
	auto & gameStats = machine.getContext().gameStats;

	this->player1StartPos = 70;
  this->player1EndPos = 90;
  this->player1Counter = this->player1StartPos;

  this->xOffset = 0;
  this->yOffset = 7;


}


// ----------------------------------------------------------------------------
//  Handle state updates .. 
//
void DisplayScoreState::update(StateMachine & machine) {

	auto & arduboy = machine.getContext().arduboy;
	auto & gameStats = machine.getContext().gameStats;
  auto justPressed = arduboy.justPressedButtons();
  auto pressed = arduboy.pressedButtons();


  if (arduboy.everyXFrames(16)) {

    if (this->player1Counter < this->player1EndPos) this->player1Counter++;

  }


  if (arduboy.everyXFrames(2)) {

    if (arduboy.pressed(LEFT_BUTTON) && this->xOffset > -45) this->xOffset--; 
    if (arduboy.pressed(RIGHT_BUTTON) && this->xOffset <30) this->xOffset++; 

  }

	if (justPressed & A_BUTTON) {
		machine.changeState(GameStateType::PlayGame); 
	}

}


// ----------------------------------------------------------------------------
//  Render the state .. 
//
void DisplayScoreState::render(StateMachine & machine) {
Serial.println("sdfsdf");
	auto & arduboy = machine.getContext().arduboy;
	auto & gameStats = machine.getContext().gameStats;
	

  this->drawBoard(this->xOffset, this->yOffset);

  this->drawPlayer_1(this->xOffset, this->yOffset, this->player1StartPos, PlayerTile::Original);
  this->drawPlayer_1(this->xOffset, this->yOffset, this->player1Counter, PlayerTile::Player1);

}


void DisplayScoreState::drawBoard(int8_t xOffset, int8_t yOffset) {

  Sprites::drawSelfMasked(xOffset, yOffset + Constants::Board_Row_1_Y, Images::Board_Start, 0);
  Sprites::drawSelfMasked(xOffset, yOffset + Constants::Board_Row_2_Y, Images::Board_SmallRadius, 0);

  for (uint8_t x = 0; x < 7; x++) {
    Sprites::drawSelfMasked(xOffset + Constants::Board_Start_Width + (x * Constants::Board_Tile_Width), yOffset + Constants::Board_Row_1_Y, Images::Board_Tile, 0);
    Sprites::drawSelfMasked(xOffset + Constants::Board_Start_Width + (x * Constants::Board_Tile_Width), yOffset + Constants::Board_Row_2_Y, Images::Board_Tile, 0);
    Sprites::drawSelfMasked(xOffset + Constants::Board_Start_Width + (x * Constants::Board_Tile_Width), yOffset + Constants::Board_Row_3_Y, Images::Board_Tile, 0);
  }

  Sprites::drawSelfMasked(xOffset + Constants::Board_Start_Width + (7 * Constants::Board_Tile_Width), yOffset + Constants::Board_Row_1_Y, Images::Board_LargeRadius, 0);

}


void DisplayScoreState::drawPlayer_1(int8_t xOffset, int8_t yOffset, uint8_t position, PlayerTile playerTile) {

  Sprites::drawExternalMask(xOffset + pgm_read_byte(&Board_Positions_Player_2[position * 2]), 
                            yOffset + pgm_read_byte(&Board_Positions_Player_2[(position * 2) + 1]),
                            Images::Player_Tiles, Images::Player_Mask, static_cast<uint8_t>(playerTile), 0);

}
