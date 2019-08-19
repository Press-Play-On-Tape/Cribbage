#include "GameOverState.h"
#include "../images/Images.h"


// ----------------------------------------------------------------------------
//  Initialise state ..
//
void GameOverState::activate(StateMachine & machine) {
	
}


// ----------------------------------------------------------------------------
//  Handle state updates .. 
//
void GameOverState::update(StateMachine & machine) {

	auto & arduboy = machine.getContext().arduboy;
  auto justPressed = arduboy.justPressedButtons();
  auto pressed = arduboy.pressedButtons();


	// Update ticker ..

	if (arduboy.everyXFrames(8)) {
		this->counter++;
		if (this->counter == 28) this->counter = 0;
	}

	
	// Handle other input ..

	if ((justPressed & LEFT_BUTTON) && this->index == 1) this->index = 0;
	if ((justPressed & RIGHT_BUTTON) && this->index == 0) this->index = 1;

	if (justPressed & A_BUTTON) {
		if (this->index == 0) {
			machine.changeState(GameStateType::PlayGame); 
		}
		else {
			machine.changeState(GameStateType::TitleScreen); 
		}
	}

}


// ----------------------------------------------------------------------------
//  Render the state .. 
//
void GameOverState::render(StateMachine & machine) {

	auto & arduboy = machine.getContext().arduboy;

	SpritesB::drawOverwrite(0, 0, Images::GameOver, 0);


	for (int16_t x = -20; x < 154; x = x + 28) {
		SpritesB::drawSelfMasked(x - this->counter, 45, Images::TitleScreen_Cards, 0);
	}

	Message message;
	this->drawDealer(machine, 87, 18, DealerFace::Normal, message);
	arduboy.fillRect(0, 60, 128, 63, BLACK);
	arduboy.drawFastHLine(0, 61, 128);
	arduboy.drawHorizontalDottedLine(0, 127, 63);

	SpritesB::drawExternalMask((this->index == 0 ? 10 : 58), 34, Images::Hand, Images::Hand_Mask, 0, 0);

}

