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
