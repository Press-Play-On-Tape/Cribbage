#include "BaseState.h"

#include "../images/Images.h"
#include "../utils/Utils.h"
#include "../utils/Enums.h"
#include "../fonts/Font3x5.h"



void BaseState::renderScore(StateMachine & machine, int16_t score, uint8_t x, uint8_t y) {

	auto & arduboy = machine.getContext().arduboy;

	// if (score < 0) { arduboy.drawFastHLine(x - 3, y + 3, 2); }

	// if (score > 10000) {

	// 	for (uint8_t j = 5, x2 = x - 4; j > 0; --j, x2 += 4) {
			
	// 		uint8_t digits[5] = {};
	// 		extractDigits(digits, static_cast<uint16_t>(absT(score)));

	// 		font3x5.setCursor(x2, y);
	// 		font3x5.print(digits[j - 1]);

	// 	}

	// }
	// else {

	// 	for (uint8_t j = 4, x2 = x; j > 0; --j, x2 += 4) {
			
	// 		uint8_t digits[4] = {};
	// 		extractDigits(digits, static_cast<uint16_t>(absT(score)));

	// 		font3x5.setCursor(x2, y);
	// 		font3x5.print(digits[j - 1]);
	// 	}

	// }

}


void BaseState::drawMessageBox(StateMachine &machine, String message, uint8_t lines, BubbleAlignment alignment) {

	drawMessageBox(machine, message, lines, 72, alignment);

}

void BaseState::drawMessageBox(StateMachine &machine, String message, uint8_t lines, uint8_t width, BubbleAlignment alignment) {

	auto & arduboy = machine.getContext().arduboy;

//	const uint8_t yPos[] = { 12, 8, 4 };
	uint8_t x = (alignment == BubbleAlignment::Player ? 0 : 128 - width);
//	uint8_t y = yPos[lines - 1];
	uint8_t y = 0;
	uint8_t yBottom = y + (lines * 8) + 4;

	arduboy.fillRect(x, y + 2, width, yBottom - y + 4, BLACK);	
	arduboy.fillRect(x + 2, y, width - 4, yBottom - y + 8, BLACK);	
	arduboy.fillRect(x + 1, y + 4, width - 2, yBottom - y + 1, WHITE);	
	arduboy.fillRect(x + 4, y + 1, width - 8, yBottom - y + 6, WHITE);	
	arduboy.drawRect(x + 2, y + 2, width - 4, yBottom - y + 4, BLACK);	


	Sprites::drawExternalMask(x, y, Images::Talk_Top_Left, Images::Talk_Bottom_Left_Mask, 0, 0);
	Sprites::drawExternalMask(x + width - 8, y, Images::Talk_Top_Right, Images::Talk_Bottom_Right_Mask, 0, 0);

	if (alignment == BubbleAlignment::Player) {

		Sprites::drawExternalMask(x, yBottom - 1, Images::Talk_Bubble_Left, Images::Talk_Bubble_Left_Mask, 0, 0);
		Sprites::drawExternalMask(x + width - 8, yBottom, Images::Talk_Bottom_Right, Images::Talk_Bottom_Right_Mask, 0, 0);

	}
	else {

		Sprites::drawExternalMask(x, yBottom, Images::Talk_Bottom_Left, Images::Talk_Bottom_Left_Mask, 0, 0);
		Sprites::drawExternalMask(x + width - 26, yBottom - 1, Images::Talk_Bubble_Right, Images::Talk_Bubble_Right_Mask, 0, 0);

	}

  font3x5.setCursor(x + 6, y + 6);
  font3x5.setTextColor(BLACK);
  font3x5.print(message);
  font3x5.setTextColor(WHITE);

}

