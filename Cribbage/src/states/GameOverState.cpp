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

	if (arduboy.everyXFrames(16)) {
		this->counter++;
		if (this->counter == 28) this->counter = 0;
	}

	
	// Handle other input ..

	if (justPressed & A_BUTTON) {
		arduboy.setRGBled(0, 0, 0);
		machine.changeState(GameStateType::TitleScreen); 
	}

}


// ----------------------------------------------------------------------------
//  Render the state .. 
//
void GameOverState::render(StateMachine & machine) {

	SpritesB::drawOverwrite(0, 20, Images::GameOver, 0);

	for (int16_t x = 154; x > -50; x = x - 28) {
		SpritesB::drawSelfMasked(x + this->counter, -1, Images::TitleScreen_Cards_Top, 0);
	}


	for (int16_t x = -20; x < 154; x = x + 28) {
		SpritesB::drawSelfMasked(x - this->counter, 45, Images::TitleScreen_Cards, 0);
	}

}

