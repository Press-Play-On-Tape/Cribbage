#include "PlayGameState.h"
#include "../images/Images.h"



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

// Serial.print("Vs:");
// Serial.println((uint8_t)this->viewState);

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

      switch (this->counter) {

        case 0:
          if ((justPressed & LEFT_BUTTON) && (this->highlightCard > 0))															this->highlightCard--;
          if ((justPressed & RIGHT_BUTTON) && this->highlightCard < player1.getHandCardCount() - 1)	this->highlightCard++;

          if (justPressed & A_BUTTON) {

            uint8_t card = player1.removeFromHand(highlightCard);
            this->playedCards[this->playIdx] = card;
            this->playIdx++;
            if (this->highlightCard == player1.getHandCardCount()) this->highlightCard--;

            uint8_t playedValue = getBoardValue();
            saveMessageWithScore(playedValue, getScore(), BubbleAlignment::Player);
            this->counter = 1;

						uint8_t score = getScore();
						player1.addScore(score);
            Serial.print("playIdx: ");
            Serial.println(playIdx);							

            Serial.print("score: ");
            Serial.println(getScore());							

          }
          break;

        case 1 ... 45:
          this->counter++;
          this->message.renderRequired = true;
          break;

        case 46:
          this->viewState = ViewState::ComputersTurn;
          this->counter = 0;
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
						player2.playCard(this->playedCards, card, points);
						Serial.print("Computer played: ");
						Serial.print(card);
						Serial.print(" ");
						Serial.println(points);

						if (card != Constants::NoCard) {

							this->playedCards[this->playIdx] = card;
							this->playIdx++;
							uint8_t playedValue = getBoardValue();

							if (points != 0) {
								
								player2.addScore(points);
								saveMessageWithScore(playedValue, points, BubbleAlignment::Computer);

							}
							else {

								saveMessageWithScore(playedValue, 0, BubbleAlignment::Computer);

							}
Serial.print("playIdx: ");
Serial.println(playIdx);							

Serial.print("score: ");
Serial.println(getScore());							
						
						}
						else {

							saveMessage(F("  Go!  "), 1, 34, BubbleAlignment::Computer);

						}

					}

					break;

				case 17 ... 60:
					if (justPressed & A_BUTTON) this->counter = 60;
					this->message.renderRequired = true;
					break;

				case 61:				

					this->counter = 0;
					this->viewState = ViewState::PlayersTurn;

					player1.printHand(1);
					player2.printHand(2);
					break;

				}

			break;

		case ViewState::DisplayScore:

			if (justPressed & A_BUTTON) {
				viewState = prevViewState; 
			}

			if (arduboy.everyXFrames(16)) {

				switch (this->counter) {

					case 0:
						if (this->player1Counter < this->player1EndPos) {
							this->player1Counter++;
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

		}

		if (justPressed & B_BUTTON) {
//			machine.changeState(GameStateType::DisplayScore); 

			this->player1StartPos = 80;
			this->player1EndPos = 93;
			this->player1Counter = this->player1StartPos;

			this->counter = 0;
			this->highlight = true;
			prevViewState = viewState;
			viewState = ViewState::DisplayScore;

		}

}