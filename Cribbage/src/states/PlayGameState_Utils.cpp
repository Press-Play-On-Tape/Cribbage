#include "PlayGameState.h"
#include "../images/Images.h"
#include "../utils/CardUtils.h"
#include "../utils/Utils.h"

void PlayGameState::resetPlay(StateMachine & machine) {

	auto & gameStats = machine.getContext().gameStats;
	auto & player1 = gameStats.player1;
	auto & player2 = gameStats.player2;
	auto & deck = gameStats.deck;

	for (uint8_t x = 0; x < 8; x++) {

		this->playedCards[x] = Constants::NoCard;

	}

	this->playIdx = 0;

	player1.setGo(false);
	player2.setGo(false);

}

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

void PlayGameState::saveMessage(String message, uint8_t lines, BubbleAlignment alignment) {

	saveMessage(message, lines, 72, alignment);

}

void PlayGameState::saveMessage(String message, uint8_t lines, uint8_t width, BubbleAlignment alignment) {

	this->message.message = message;
	this->message.lines= lines;
	this->message.width= width;
	this->message.alignment = alignment;
	this->message.renderRequired = true;

}

uint8_t PlayGameState::getBoardValue() {

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

	uint8_t playValue = getBoardValue();

	if (playValue == 15 || playValue == 31) {

		score += 2;

	}


	// Check for run of three ..

	if (playIdx >= 3) {

		uint8_t card1 = CardUtils::getCardValue(playedCards[playIdx - 3], false);
		uint8_t card2 = CardUtils::getCardValue(playedCards[playIdx - 2], false);
		uint8_t card3 = CardUtils::getCardValue(playedCards[playIdx - 1], false);

		if ((card1 != card2 && card1 != card3 && card2 != card3) &&
			diffT(card1, card2) <= 2 && diffT(card1, card3) <=2) {

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
			diffT(card1, card2) <= 3 && diffT(card1, card3) <=3 && diffT(card1, card4) <=3 && diffT(card2, card3) <=3 && diffT(card2, card4) <=3 && diffT(card3, card4) <=3) {

			score += 1;

		}

	}
	
	return score;

}


void PlayGameState::saveMessageWithScore(uint8_t playedValue, uint8_t points, BubbleAlignment alignment) {

  uint8_t messageIdx = 0;

  if (points > 0) {

    const char forText[] = " for ";
    const char pointsText[] = " points.";
    char messageText[] = "                ";

    if (playedValue >= 10) 	{ messageText[messageIdx] = (playedValue / 10) + 48; messageIdx++; }
    messageText[messageIdx] = (playedValue % 10) + 48; messageIdx++; 

    memcpy(&messageText[messageIdx], forText, 5);
    messageIdx += 5;

    if (points >= 10) 	{ messageText[messageIdx] = (points / 10) + 48; messageIdx++; }
    if (points < 10) 		{ messageText[messageIdx] = (points % 10) + 48; messageIdx++; }

    memcpy(&messageText[messageIdx], pointsText, 8);
    saveMessage(messageText, 1, alignment);

  }
  else {
    
    char messageText[] = "      ";
		messageIdx = 3;

    if (playedValue >= 10) 	{ messageText[messageIdx] = (playedValue / 10) + 48; messageIdx++; }
    messageText[messageIdx] = (playedValue % 10) + 48; messageIdx++; 
    messageText[messageIdx] = '.'; 
    saveMessage(messageText, 1, 34, alignment);

  }

}


bool PlayGameState::isEndOfHand(StateMachine & machine) {

	auto & gameStats = machine.getContext().gameStats;
	auto & player1 = gameStats.player1;
	auto & player2 = gameStats.player2;
	
	return (!(player1.canPlay(this->playedCards) || player2.canPlay(this->playedCards)));

}