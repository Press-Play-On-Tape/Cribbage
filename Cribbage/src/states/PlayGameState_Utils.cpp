#include "PlayGameState.h"
#include "../images/Images.h"
#include "../utils/CardUtils.h"
#include "../utils/Utils.h"


void PlayGameState::resetHand(StateMachine & machine) {

	auto & gameStats = machine.getContext().gameStats;
	auto & player1 = gameStats.player1;
	auto & player2 = gameStats.player2;
	auto & deck = gameStats.deck;

	player1.resetHand();
	player2.resetHand();
	deck.shuffle();

	for (uint8_t x = 0; x < 8; x++) {

		this->playedCards[x] = Constants::NoCard;

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

uint8_t PlayGameState::getPlayValue() {

	uint8_t value = 0;

	for (uint8_t x = 0; x < 8; x++) {

		if (this->playedCards[x] != Constants::NoCard) {

			value = value + CardUtils::getCardValue(this->playedCards[x], true);

		}

	}

	return value;

}


uint8_t PlayGameState::getScore() {
	
	uint8_t score = 0;

	// A pair ?

	if (playIdx >= 2) {
	
		uint8_t card1 = CardUtils::getCardValue(playedCards[playIdx - 2], false);
		uint8_t card2 = CardUtils::getCardValue(playedCards[playIdx - 1], false);

		if (card1 == card2) {
	
			score += 2;

		}

	}
	

	// Royal pair ?

	if (playIdx >= 3) {
	
		uint8_t card1 = CardUtils::getCardValue(playedCards[playIdx - 3], false);
		uint8_t card2 = CardUtils::getCardValue(playedCards[playIdx - 2], false);
		uint8_t card3 = CardUtils::getCardValue(playedCards[playIdx - 1], false);

		if (card1 == card2 && card2== card3) {

			score += 4;

		}

	}



	// Double Royal pair ?

	if (playIdx >= 4) {
	
		uint8_t card1 = CardUtils::getCardValue(playedCards[playIdx - 4], false);
		uint8_t card2 = CardUtils::getCardValue(playedCards[playIdx - 3], false);
		uint8_t card3 = CardUtils::getCardValue(playedCards[playIdx - 2], false);
		uint8_t card4 = CardUtils::getCardValue(playedCards[playIdx - 1], false);

		if (card1 == card2 && card1 == card3 && card3 == card4) {
	
			score += 6;

		}

	}


	// Check for 15 or 31 ..

	uint8_t playValue = getPlayValue();

	if (playValue == 15 || playValue == 31) {

		score += 2;

	}


	// Check for run of three ..

	if (playIdx >= 3) {

		uint8_t card1 = CardUtils::getCardValue(playedCards[playIdx - 3], false);
		uint8_t card2 = CardUtils::getCardValue(playedCards[playIdx - 2], false);
		uint8_t card3 = CardUtils::getCardValue(playedCards[playIdx - 1], false);

		if ((card1 != card2 && card1 != card3 && card2 != card3) &&
			absT(card1 - card2) <= 2 && absT(card1 - card3) <=2) {

			score += 3;

		}

	}


	// Check for run of four ..

	if (playIdx >= 4) {

		uint8_t card1 = CardUtils::getCardValue(playedCards[playIdx - 4], false);
		uint8_t card2 = CardUtils::getCardValue(playedCards[playIdx - 3], false);
		uint8_t card3 = CardUtils::getCardValue(playedCards[playIdx - 2], false);
		uint8_t card4 = CardUtils::getCardValue(playedCards[playIdx - 1], false);

		if ((card1 != card2 && card1 != card3 && card1 != card4 && card2 != card3 && card2 != card4 && card3 != card4) &&
			absT(card1 - card2) <= 3 && absT(card1 - card3) <=3 && absT(card1 - card4) <=3 && absT(card2 - card3) <=3 && absT(card2 - card4) <=3 && absT(card3 - card4) <=3) {

			score += 1;

		}

	}
	
	return score;

}