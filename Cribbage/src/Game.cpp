/*
	Copyright (C) 2018 Pharap (@Pharap)

	Licensed under the Apache License, Version 2.0 (the "License");
	you may not use this file except in compliance with the License.
	You may obtain a copy of the License at

	http://www.apache.org/licenses/LICENSE-2.0

	Unless required by applicable law or agreed to in writing, software
	distributed under the License is distributed on an "AS IS" BASIS,
	WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
	See the License for the specific language governing permissions and
	limitations under the License.

	Modified by Filmote and Vampirics
*/

#include "Game.h"

#include "states/States.h"
#include "utils/Utils.h"
#include "utils/EEPROM_Utils.h"

void Game::setup(void) {

	auto & arduboy = this->context.arduboy;

	arduboy.boot();
	arduboy.flashlight();
	arduboy.systemButtons();
	arduboy.audio.begin();
	arduboy.setFrameRate(60);

//#ifdef DEBUG  
//    randomSeed(3);          // Seed with a constant to enable debugging with a repeatable bone set. 
//#else
    arduboy.initRandomSeed();
//#endif

  EEPROM_Utils::initEEPROM(false);

  // EEPROM_Utils::getName(gameStats.player1.name, EEPROM_PLAYER1);
  // EEPROM_Utils::getName(gameStats.player2.name, EEPROM_PLAYER2);
  // EEPROM_Utils::getName(gameStats.player3.name, EEPROM_PLAYER3);
  // EEPROM_Utils::getName(gameStats.player4.name, EEPROM_PLAYER4);

	this->currentState = GameStateType::SplashScreen; 
	this->splashScreenState.activate(*this);
	
}

void Game::loop(void) {

	auto &arduboy = this->context.arduboy;
	if (!arduboy.nextFrame()) return;

	arduboy.pollButtons();
	arduboy.clear();

	switch (currentState) {

		case GameStateType::SplashScreen:

			if (currentState != savedCurrentState) {
				this->context.gameState = this->currentState;
				this->splashScreenState.activate(*this);
				this->savedCurrentState = this->currentState;
			}
			this->splashScreenState.update(*this);
			this->splashScreenState.render(*this);
			break;

		case GameStateType::TitleScreen: 

			if (currentState != savedCurrentState) {
				this->context.gameState = this->currentState;
				this->titleScreenState.activate(*this);
				this->savedCurrentState = this->currentState;
			}
			this->titleScreenState.update(*this);
			this->titleScreenState.render(*this);
			break;

		case GameStateType::PlayGame: 

			if (currentState != savedCurrentState) {
				this->context.gameState = this->currentState;
				this->playGameState.activate(*this);
				this->savedCurrentState = this->currentState;
			}
			this->playGameState.update(*this);
			this->playGameState.render(*this);
			break;

		case GameStateType::GameOver: 

			if (currentState != savedCurrentState) {
				this->context.gameState = this->currentState;
				this->gameOverState.activate(*this);
				this->savedCurrentState = this->currentState;
			}
			this->gameOverState.update(*this);
			this->gameOverState.render(*this);
			break;

		default: break;	

	}

	arduboy.display();
	
}
