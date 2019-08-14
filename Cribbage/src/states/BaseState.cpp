#include "BaseState.h"

#include "../images/Images.h"
#include "../utils/Utils.h"
#include "../utils/Enums.h"
#include "../fonts/Font3x5.h"


void BaseState::drawMessageBox(StateMachine &machine, String message, uint8_t lines, BubbleAlignment alignment) {

	drawMessageBox(machine, message, lines, 72, alignment);

}

void BaseState::drawMessageBox(StateMachine &machine, String message, uint8_t lines, uint8_t width, BubbleAlignment alignment) {

	auto & arduboy = machine.getContext().arduboy;

//	const uint8_t yPos[] = { 12, 8, 4 };
	uint8_t x = (alignment == BubbleAlignment::Player ? 0 : 128 - width);
//	uint8_t y = yPos[lines - 1];
	int8_t y = -1;
	uint8_t yBottom = y + (lines * 8) + 4;

	arduboy.fillRect(x, y + 4, width, yBottom - y, BLACK);	
	arduboy.fillRect(x + 4, y, width - 8, yBottom - y + 8, BLACK);	
	arduboy.fillRect(x + 1, y + 4, width - 2, yBottom - y + 1, WHITE);	
	arduboy.fillRect(x + 4, y + 1, width - 8, yBottom - y + 6, WHITE);	
	arduboy.drawRect(x + 2, y + 2, width - 4, yBottom - y + 4, BLACK);	


	SpritesB::drawExternalMask(x, y, Images::Talk_Top_Left, Images::Talk_Top_Left_Mask, 0, 0);
	SpritesB::drawExternalMask(x + width - 8, y, Images::Talk_Top_Right, Images::Talk_Top_Right_Mask, 0, 0);

	if (alignment == BubbleAlignment::Player) {

		SpritesB::drawExternalMask(x, yBottom - 1, Images::Talk_Bubble_Left, Images::Talk_Bubble_Left_Mask, 0, 0);
		SpritesB::drawExternalMask(x + width - 8, yBottom, Images::Talk_Bottom_Right, Images::Talk_Bottom_Right_Mask, 0, 0);

	}
	else {

		SpritesB::drawExternalMask(x, yBottom, Images::Talk_Bottom_Left, Images::Talk_Bottom_Left_Mask, 0, 0);
		SpritesB::drawExternalMask(x + width - 26, yBottom - 1, Images::Talk_Bubble_Right, Images::Talk_Bubble_Right_Mask, 0, 0);

	}

  font3x5.setCursor(x + 6, y + 6);
  font3x5.setTextColor(BLACK);
  font3x5.print(message);
  font3x5.setTextColor(WHITE);

}


void BaseState::drawScore(StateMachine & machine, uint8_t x, int8_t y, uint8_t score) {

	auto & arduboy = machine.getContext().arduboy;

  uint8_t xPos = x + 6; 
  if (score >= 10)  xPos = x + 4; 
  if (score >= 100) xPos = x + 2; 

  font3x5.setTextColor(BLACK);
  arduboy.fillRect(x, y, 14, 8, BLACK);
  arduboy.fillRect(x + 1, y + 1, 13, 7);
  font3x5.setCursor(xPos, y + 1);
  font3x5.print(score);
  font3x5.setTextColor(WHITE);

}