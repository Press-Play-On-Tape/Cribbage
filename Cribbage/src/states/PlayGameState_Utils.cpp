#include "PlayGameState.h"
#include "../images/Images.h"
#include "../utils/CardUtils.h"


void PlayGameState::resetHand(StateMachine & machine) {

	auto & gameStats = machine.getContext().gameStats;
	auto & player1 = gameStats.player1;
	auto & player2 = gameStats.player2;
	auto & deck = gameStats.deck;

	player1.resetHand();
	player2.resetHand();
	deck.shuffle();

	for (uint8_t x = 0; x < 8; x++) {

		this->play[x] = Constants::NoCard;

	}

	this->turnUp = Constants::NoCard;

}

void PlayGameState::saveMessage(String message, uint8_t lines, Alignment alignment) {

	saveMessage(message, lines, 72, alignment);

}

void PlayGameState::saveMessage(String message, uint8_t lines, uint8_t width, Alignment alignment) {

	this->message.message = message;
	this->message.lines= lines;
	this->message.width= width;
	this->message.alignment = alignment;
	this->message.renderRequired = true;

}

uint8_t PlayGameState::playValue() {

	uint8_t value = 0;

	for (uint8_t x = 0; x < 8; x++) {

		if (this->play[x] != Constants::NoCard) {

			value = value + CardUtils::getCardValue(this->play[x], true);

		}

	}

	return value;

}