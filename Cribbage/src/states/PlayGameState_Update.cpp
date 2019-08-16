#include "PlayGameState.h"
#include "../images/Images.h"
#include "../utils/CardUtils.h"


// ----------------------------------------------------------------------------
//  Handle state updates .. 
//
void PlayGameState::update(StateMachine & machine) {

	auto & arduboy = machine.getContext().arduboy;
	auto & gameStats = machine.getContext().gameStats;
	auto & player1 = gameStats.player1;
	auto & player2 = gameStats.player2;
	auto & deck = gameStats.deck;

  auto justPressed = arduboy.justPressedButtons();
  auto pressed = arduboy.pressedButtons();

	switch (this->viewState) {

		case ViewState::DealCards:

			this->cribState = CribState::Empty;

			if (player1.getHandCardCount() < 6) {

				if (arduboy.everyXFrames(16)) {

					player1.addToHand(deck.getCard());
					player2.addToHand(deck.getCard());
					player1.printHand(1);
					player2.printHand(2);

				}

			}
			else {

				this->viewState = ViewState::DiscardCribPlayer;

			}

			break;

		case ViewState::DiscardCribPlayer:

			if (player1.getHandCardCount() > 4) {

				if ((justPressed & LEFT_BUTTON) && (this->highlightCard > 0))															this->highlightCard--;
				if ((justPressed & RIGHT_BUTTON) && this->highlightCard < player1.getHandCardCount() - 1)	this->highlightCard++;

				if (justPressed & A_BUTTON) {

					this->cribState = CribState::Hidden;
					uint8_t card = player1.removeFromHand(highlightCard);
	
					if (gameStats.playersTurn == 0) {

						player1.addToCrib(card);

					}
					else {

						player2.addToCrib(card);

					}

					if (this->highlightCard == player1.getHandCardCount()) this->highlightCard--;

				}

			}
			else {

				player2.discardToCrib(this->computerDiscard1, this->computerDiscard2);
				this->viewState = ViewState::DiscardCribComputer;
				this->counter = 0;

			}

			break;

		case ViewState::DiscardCribComputer:

			this->counter++;

			switch (this->counter) {

				case 0 ... 45:
					if (justPressed & A_BUTTON) this->counter = 45;
					saveMessage(F("I will throw\nthese two cards."), 2, BubbleAlignment::Computer);
					break;

				case 46:
					{
						uint8_t index = player2.getHandCardIndex(this->computerDiscard1);
						player2.removeFromHand(index);

						if (gameStats.playersTurn == 0) {

							player1.addToCrib(this->computerDiscard1);

						}
						else {

							player2.addToCrib(this->computerDiscard1);

						}

					}
					break;

				case 47 ... 60:
					break;

				case 61:
					{
						uint8_t index = player2.getHandCardIndex(this->computerDiscard2);
						player2.removeFromHand(index);

						if (gameStats.playersTurn == 0) {

							player1.addToCrib(this->computerDiscard2);

						}
						else {

							player2.addToCrib(this->computerDiscard2);

						}

						this->viewState = ViewState::TurnUp;
						this->counter = 0;
					}
					break;

			}

			break;

		case ViewState::TurnUp:

			this->counter++;

			switch (this->counter) {

				case 0 ... 14:
					break;

				case 15:
					this->turnUp = deck.getCard();
					break;

				case 16 ... 25:
					break;

				case 26 ... 70:
					if (justPressed & A_BUTTON) this->counter = 70;
					saveMessage(F("Your turn to\nstart."), 2, BubbleAlignment::Computer);
					break;

				case 71:				
					this->counter = 0;
					this->viewState = ViewState::PlayersTurn;

					player1.printHand(1);
					player2.printHand(2);

					break;

			}

			break;

		case ViewState::PlayersTurn:

			if (!player1.canPlay(this->playedCards)) {
				player1.setGo(true);
				this->viewState = ViewState::PlayersTurn_Go;
			}
			else {
				this->viewState = ViewState::PlayersTurn_Normal;
			}
			break;

		case ViewState::PlayersTurn_Go:

			this->counter++;

			switch (this->counter) {

				case 0 ... 5:
					break;

				case 6:
					if (player2.getGo()) {
						player1.addScore(1);
						saveMessage(F(" Go for 1. "), 1, 45, BubbleAlignment::Player);
					}
					else {
						saveMessage(F("  Go!  "), 1, 34, BubbleAlignment::Player);
					}
					break;

				case 7 ... 52:
					this->message.renderRequired = true;
					break;

				case 53:
					this->counter = 0;
					if (player2.getGo()) {
						resetPlay(machine);
						this->viewState = ViewState::ComputersTurn;
					}
					else {
						this->viewState = ViewState::PlayersTurn;
						if (!player2.getGo()) {
							this->viewState = ViewState::ComputersTurn;
						}
					}

					break;

			}
			break;
			
		case ViewState::PlayersTurn_Normal:
				
			switch (this->counter) {

				case 0:
					if ((justPressed & LEFT_BUTTON) && (this->highlightCard > 0))															this->highlightCard--;
					if ((justPressed & RIGHT_BUTTON) && this->highlightCard < player1.getHandCardCount() - 1)	this->highlightCard++;

					if (justPressed & A_BUTTON) {

						uint8_t board = getBoardValue();
						uint8_t card = CardUtils::getCardValue(player1.getHandCard(this->highlightCard), true);

						if (board + card > 31) {

							//RGB LED

						}
						else {

							uint8_t card = player1.removeFromHand(highlightCard);
							this->playedCards[this->playIdx] = card;
							this->playIdx++;
							if (this->highlightCard == player1.getHandCardCount()) this->highlightCard--;

							uint8_t playedValue = getBoardValue();
							uint8_t score = getScore(machine, player1, player2.getGo());
							saveMessageWithScore(playedValue, score, BubbleAlignment::Player);
							this->counter++;

							player1.addScore(score);
							Serial.print("playIdx: ");
							Serial.println(playIdx);							

							Serial.print("score: ");
							Serial.println(score);							

						}

					}
					break;

				case 1 ... 45:
					this->counter++;
					this->message.renderRequired = true;
					break;

				case 46:
					{
						uint8_t board = getBoardValue();
						this->counter = 0;

						if (board == 31) {

							if (player2.getHandCardCount() != 0) {
								this->viewState = ViewState::ComputersTurn;
								resetPlay(machine);
							}
							else if (player1.getHandCardCount() != 0) {
								resetPlay(machine);
							}
							else {
								this->viewState = ViewState::DisplayScore_Board;
							}

						}
						else {

							this->viewState = ViewState::ComputersTurn;
							if (!player1.getGo() && player2.getGo()) {
								
								if (!player1.canPlay(this->playedCards)) {
								 	resetPlay(machine);
									if (player2.getHandCardCount() > 0) {
										this->viewState = ViewState::ComputersTurn;
									}
									else {
										this->viewState = ViewState::PlayersTurn_Normal;
									}
								}
								this->viewState = ViewState::PlayersTurn_Normal;
							}
						}

					}
					break;

			}

			break;

		case ViewState::ComputersTurn:

			this->counter++;

			switch (this->counter) {

				case 0 ... 15:
					break;

				case 16:
					{
						uint8_t card = Constants::NoCard;
						uint8_t points = 0;
						player2.playCard(this->playedCards, (player1.getHandCardCount() > 1 && !player1.getGo()), card, points);
						Serial.print("Computer played: ");
						Serial.print(card);
						Serial.print(" ");
						Serial.println(points);

						if (card != Constants::NoCard) {

							this->playedCards[this->playIdx] = card;
							this->playIdx++;
							uint8_t playedValue = getBoardValue();


							// Add a point if the dealer has played the last card ..

							if (playedValue != 31) {
								if ((player1.getHandCardCount() == 0 || player1.getGo()) && (player2.getHandCardCount() == 0 || !player2.canPlay(this->playedCards))) {
									points++;
								}
							}

							player2.addScore(points);
							saveMessageWithScore(playedValue, points, BubbleAlignment::Computer);
						
						}
						else {

							if (player1.getGo()) {
								player2.addScore(1);
								saveMessage(F(" Go for 1. "), 1, 45, BubbleAlignment::Computer);
							}
							else {
								saveMessage(F("  Go!  "), 1, 34, BubbleAlignment::Computer);
							}

						}

					}

					break;

				case 17 ... 60:
					if (justPressed & A_BUTTON) this->counter = 60;
					this->message.renderRequired = true;
					break;

				case 61:				
					{
						this->counter = 0;
						uint8_t board = getBoardValue();

						if (board == 31) {
							if (player1.getHandCardCount() != 0) {
								this->viewState = ViewState::PlayersTurn;
								resetPlay(machine);
							}
							else if (player2.getHandCardCount() != 0) {
								this->viewState = ViewState::ComputersTurn;
								resetPlay(machine);
							}
							else {
								this->viewState = ViewState::DisplayScore_Board;
							}
						}
						else {

							if (player1.getHandCardCount() == 0 && player2.getHandCardCount() == 0) {
								this->viewState = ViewState::DisplayScore_Board;
							}
							else if (player1.getHandCardCount() != 0) {
								this->viewState = ViewState::PlayersTurn;
								if (player1.getGo()) {
									resetPlay(machine);
								}
							}
							else {
								this->viewState = ViewState::PlayersTurn;
								if ((player1.getGo() || player1.getHandCardCount() == 0) && !player2.getGo()) {
									if (!player2.canPlay(this->playedCards)) {
										if (player1.getHandCardCount() > 0) {
											this->viewState = ViewState::PlayersTurn;
										}
										else {
											this->viewState = ViewState::ComputersTurn;
										}
										resetPlay(machine);
									}
									else {
										this->viewState = ViewState::ComputersTurn;
									}

								}
								else if (player1.getGo() && player2.getGo()) {
										resetPlay(machine);
								}
							}
						}
					}

					player1.printHand(1);
					player2.printHand(2);
					break;

			}

			break;

		case ViewState::DisplayScore_Board:

			// if (justPressed & A_BUTTON) {
			// 	player1.setPrevScore(player1.getScore());
			// 	player2.setPrevScore(player2.getScore());
			// 	viewState = prevViewState; 
			// }

			if (arduboy.everyXFrames(16)) {

				switch (this->counter) {

					case 0:
Serial.print(this->player1Counter);
Serial.print("<");
Serial.print(player1.getPrevScore());
Serial.print(",");
Serial.print(this->player2Counter);
Serial.print("<");
Serial.print(player2.getPrevScore());
Serial.println("");

						if (this->player1Counter < player1.getPrevScore()) this->player1Counter = player1.getPrevScore();
						if (this->player2Counter < player2.getPrevScore()) this->player2Counter = player2.getPrevScore();

						if (this->player1Counter < player1.getScore() || this->player2Counter < player2.getScore()) {
							if (this->player1Counter < player1.getScore()) {
								this->player1Counter++;
							}
							if (this->player2Counter < player2.getScore()) {
								this->player2Counter++;
							}
						}
						else {
							this->counter++;
						}
						break;

					case 1:
					case 3:
					case 5:
					case 7:
						this->highlight = false;
						this->counter++;
						break;

					case 2:
					case 4:
					case 6:
					case 8:
						this->highlight = true;
						this->counter++;
						break;

					default: break;

				}

			}

			if (justPressed & A_BUTTON) {
				viewState = ViewState::DisplayScore_Other;
				this->counter = 0;
				this->highlight = true;
			}

			break;

		case ViewState::DisplayScore_Other:
		case ViewState::DisplayScore_Dealer:
		case ViewState::DisplayScore_Crib:
			{
				switch (this->counter) {

					case 0 ... 5:
						this->counter++;
						break;

					case 6 ... 48:
						this->counter++;
						switch (this->viewState) {

							case ViewState::DisplayScore_Other:
								if (gameStats.playerDealer == 0) {
									saveMessage(F("My hand .."), 1, 45, BubbleAlignment::Player);
								}
								else {
									saveMessage(F("Your hand .."), 1, 45, BubbleAlignment::Player);
								}
								break;
								
							case ViewState::DisplayScore_Dealer:
								if (gameStats.playerDealer == 1) {
									saveMessage(F("My hand .."), 1, 45, BubbleAlignment::Player);
								}
								else {
									saveMessage(F("Your hand .."), 1, 45, BubbleAlignment::Player);
								}
								break;

							case ViewState::DisplayScore_Crib:
								if (gameStats.playerDealer == 1) {
									saveMessage(F("My crib .."), 1, 45, BubbleAlignment::Player);
								}
								else {
									saveMessage(F("Your crib .."), 1, 45, BubbleAlignment::Player);
								}
								break;

						}
						break;

					case 49:
						this->counter++;
						switch (this->viewState) {

							case ViewState::DisplayScore_Other:
								if (gameStats.playerDealer == 0) {
									player2.calculateHandScore(gameStats.scores, this->turnUp);
								}
								else {
									player1.calculateHandScore(gameStats.scores, this->turnUp);
								}
								break;

							case ViewState::DisplayScore_Dealer:
								if (gameStats.playerDealer == 0) {
									player1.calculateHandScore(gameStats.scores, this->turnUp);
								}
								else {
									player2.calculateHandScore(gameStats.scores, this->turnUp);
								}
								break;

							case ViewState::DisplayScore_Crib:
								if (gameStats.playerDealer == 0) {
									player2.calculateCribScore(gameStats.scores, this->turnUp);
								}
								else {
									player1.calculateCribScore(gameStats.scores, this->turnUp);
								}
								break;

						}
						
for (uint8_t x=0; x < 10; x++) {
Score score = gameStats.scores[x];
for (uint8_t y=0; y < 5; y++) {
CardUtils::printCard(score.getHand(y));
Serial.print(" ");
}
Serial.print(" (S");
Serial.print(score.getScore());
Serial.print(" ,T");
Serial.print((uint8_t)score.getType());
Serial.println(")");
}
						break;

					case 50 ... 64:

						if (this->player1Counter < player1.getPrevScore()) this->player1Counter = player1.getPrevScore();
						if (this->player2Counter < player2.getPrevScore()) this->player2Counter = player2.getPrevScore();

						if (this->player1Counter < player1.getScore() || this->player2Counter < player2.getScore()) {
							if (this->player1Counter < player1.getScore()) {
								this->player1Counter++;
							}
							if (this->player2Counter < player2.getScore()) {
								this->player2Counter++;
							}
						}
						else {
							this->counter++;
						}
						break;


					case 80 ... 94:
					case 110 ... 124:
					case 140 ... 154:
					case 170 ... 184:
						this->counter++;
						this->highlight = false;
						break;

					case 65 ... 79:
					case 95 ... 109:
					case 125 ... 139:
					case 155 ... 169:
						this->counter++;
						this->highlight = true;
						break;

					default: break;

				}

				if (justPressed & A_BUTTON) {

					switch (this->viewState){

						case ViewState::DisplayScore_Other:
							this->viewState = ViewState::DisplayScore_Dealer;
							break;

						case ViewState::DisplayScore_Dealer:
							this->viewState = ViewState::DisplayScore_Crib;
							break;

						case ViewState::DisplayScore_Crib:
							this->viewState = ViewState::DealCards;
							break;

					}

					this->counter = 0;

				}

			}

		break;

	}

	if (justPressed & B_BUTTON) {
//			machine.changeState(GameStateType_Board); 

		this->player1Counter = 0;
		this->player2Counter = 0;

		this->counter = 0;
		this->highlight = true;
		prevViewState = viewState;
		viewState = ViewState::DisplayScore_Board;

	}

}