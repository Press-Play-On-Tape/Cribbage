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


	// Has the EOG flag been set?

	if (this->eog) this->viewState = ViewState::EndOfGame;

	switch (this->viewState) {

		case ViewState::DealCards:

			if (arduboy.everyXFrames(16) && player1.getHandCardCount() == 0) {
				
				resetHand(machine);
				gameStats.playerDealer = (gameStats.playerDealer == WhichPlayer::Player2 ? WhichPlayer::Player1 : WhichPlayer::Player2);
				
			}

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

			this->counter++;

			if (player1.getHandCardCount() > 4) {

				if (this->counter < 45) {
					saveMessage(F("Throw two cards\nout to the crib."), 2, 70, DealerFace::Normal, BubbleAlignment::Normal_Computer);
				}

				if ((justPressed & LEFT_BUTTON) && (this->highlightCard > 0))															this->highlightCard--;
				if ((justPressed & RIGHT_BUTTON) && this->highlightCard < player1.getHandCardCount() - 1)	this->highlightCard++;

				if (justPressed & A_BUTTON) {

					this->cribState = CribState::Hidden;
					uint8_t card = player1.removeFromHand(highlightCard);
	
					if (gameStats.playerDealer == WhichPlayer::Player1) {

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
					skipSequence(machine, 45);
					saveMessage(F("   I will throw\nthese two cards."), 2, DealerFace::Normal, BubbleAlignment::Normal_Computer);
					break;

				case 46:
					{
						uint8_t index = player2.getHandCardIndex(this->computerDiscard1);
						player2.removeFromHand(index);

						if (gameStats.playerDealer == WhichPlayer::Player1) {

							player1.addToCrib(this->computerDiscard1);

						}
						else {

							player2.addToCrib(this->computerDiscard1);

						}

					}
					break;

				case 47 ... 60:
					skipSequence(machine, 60);
					break;

				case 61:
					{
						uint8_t index = player2.getHandCardIndex(this->computerDiscard2);
						player2.removeFromHand(index);

						if (gameStats.playerDealer == WhichPlayer::Player1) {

							player1.addToCrib(this->computerDiscard2);

						}
						else {

							player2.addToCrib(this->computerDiscard2);

						}

					}
					break;

				case 62 ... 105:
					skipSequence(machine, 105);
					break;

				case 106:
					this->viewState = ViewState::TurnUp;
					this->counter = 0;
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
					//this->turnUp = 10; // Uncomment for 'Two for his nob' ..
					player1.printHand(1);
					player2.printHand(2);
					if (CardUtils::getCardValue(this->turnUp, false) != 11) {
						this->counter = 77;
					}
					break;

				case 26:
					if (gameStats.playerDealer == WhichPlayer::Player1) {
						player1.addScore(2);
						saveMessage(F("Two for his heels!"), 1, 78, DealerFace::Sad, BubbleAlignment::Normal_Player);
					}
					else {
						player2.addScore(2);
						saveMessage(F("Two for his heels!"), 1, 78, DealerFace::Happy, BubbleAlignment::Normal_Computer);
					}
					break;

				case 27 ... 72:
					skipSequence(machine, 125);
					this->message.renderRequired = true;
					break;

				case 73 ... 77:
					skipSequence(machine, 125);
					break;

				case 78 ... 125:
					moveToEOG(machine);
					skipSequence(machine, 125);
					if (gameStats.playerDealer == WhichPlayer::Player2) {
						saveMessage(F("Your turn to start@"), 1, 81, DealerFace::Normal, BubbleAlignment::Normal_Computer);
					}
					else {
						saveMessage(F("My turn to start@"), 1, 71, DealerFace::Normal, BubbleAlignment::Normal_Computer);
					}
					break;

				case 126:				
					this->counter = 0;
					if (gameStats.playerDealer == WhichPlayer::Player2) {
						this->viewState = ViewState::PlayersTurn;
					}
					else {
						this->viewState = ViewState::ComputersTurn;
					}
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
					if (player2.getGo() || player2.getHandCardCount() == 0) {
						player1.addScore(1);
						saveMessage(F(" Go for 1. "), 1, 45, DealerFace::Sad, BubbleAlignment::Normal_Player);
					}
					else {
						saveMessage(F("  Go!  "), 1, 34, DealerFace::Normal, BubbleAlignment::Normal_Player);
					}
					break;

				case 7 ... 52:
					skipSequence(machine, 52);
					this->message.renderRequired = true;
					break;

				case 53:
					moveToEOG(machine);
					this->counter = 0;
					if (player2.getGo()) {
						resetPlay(machine);

						if (player2.getHandCardCount() > 0) {
							this->viewState = ViewState::PlayersTurn;
						}
						else {
							this->viewState = ViewState::DisplayScore_Board;
						}
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

					// if (justPressed & B_BUTTON) {
					// 	player1.addScore(118);
					// }

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
							uint8_t score = getScore(player1, (player2.getGo() || player2.getHandCardCount() == 0));
							player1.addScore(score);
							saveMessageWithScore(playedValue, score,  DealerFace::Sad, BubbleAlignment::Normal_Player);
							this->counter++;
				
						}

					}
					break;

				case 1 ... 45:
					skipSequence(machine, 45);
					this->counter++;
					this->message.renderRequired = true;
					break;

				case 46:
					{
						moveToEOG(machine);
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
							if (!player1.getGo() && (player2.getGo() || player2.getHandCardCount() == 0)) {

								if (!player1.canPlay(this->playedCards)) {

								 	resetPlay(machine);
									if (player2.getHandCardCount() > 0) {
										this->viewState = ViewState::ComputersTurn;
									}
									else {

										if (player1.getHandCardCount() > 0) {
											this->viewState = ViewState::PlayersTurn_Normal;
										}
										else {
											this->viewState = ViewState::DisplayScore_Board;
										}

									}

								}
								else {
									this->viewState = ViewState::PlayersTurn_Normal;
								}

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
							saveMessageWithScore(playedValue, points,  DealerFace::Happy, BubbleAlignment::Normal_Computer);
						
						}
						else {

							if (player1.getGo()) {
								player2.addScore(1);
								saveMessage(F(" Go for 1. "), 1, 45, DealerFace::Happy, BubbleAlignment::Normal_Computer);
							}
							else {
								saveMessage(F("  Go!  "), 1, 34, DealerFace::Normal, BubbleAlignment::Normal_Computer);
							}

						}

					}

					break;

				case 17 ... 60:
					skipSequence(machine, 60);
					this->message.renderRequired = true;
					break;

				case 61:				
					{
						moveToEOG(machine);
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

			if (arduboy.everyXFrames(16)) {

				switch (this->counter) {

					case 0:

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
				this->scoreUpperRow = 0;
				player1.setPrevScore(player1.getScore());
				player2.setPrevScore(player2.getScore());
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
								if (gameStats.playerDealer == WhichPlayer::Player1) {
									saveMessage(F(" My hand@"), 1, 52, DealerFace::Happy, BubbleAlignment::Score_Computer);
								}
								else {
									saveMessage(F(" Your hand@"), 1, 52, DealerFace::Sad, BubbleAlignment::Score_Computer);
								}
								break;
								
							case ViewState::DisplayScore_Dealer:
								if (gameStats.playerDealer == WhichPlayer::Player2) {
									saveMessage(F("  My hand@"), 1, 50, DealerFace::Happy, BubbleAlignment::Score_Computer);
								}
								else {
									saveMessage(F(" Your hand@"), 1, 52, DealerFace::Sad, BubbleAlignment::Score_Computer);
								}
								break;

							case ViewState::DisplayScore_Crib:
								if (gameStats.playerDealer == WhichPlayer::Player2) {
									saveMessage(F("  My crib@"), 1, 50, DealerFace::Happy, BubbleAlignment::Score_Computer);
								}
								else {
									saveMessage(F(" Your crib@"), 1, 52, DealerFace::Sad, BubbleAlignment::Score_Computer);
								}
								break;

								default: break;

						}
						break;

					case 49:
						this->counter++;
						this->scoresTotal = addHandScoreToPlayerTotal(machine);
						break;

					case 50:
						{
							uint8_t numberOfScores = gameStats.getNumberOfScores();

							if (justPressed & A_BUTTON) {
								if (this->scoresTotal == 0) {
									this->counter = 0; 	// Will fall through this case.  I have not cleared the justPressed so will exit.
								}
								else { 
									justPressed = 0;		// Cleared the justPressed to prevent bottom justPressed test from handling.
									this->counter++;
								}
							}

							if (justPressed & UP_BUTTON && this->scoreUpperRow > 0) { this->scoreUpperRow--; }
							if (justPressed & DOWN_BUTTON && this->scoreUpperRow + 5 < numberOfScores) { this->scoreUpperRow++; }

						}
						break;

					case 51:

						if (this->player1Counter < player1.getPrevScore()) this->player1Counter = player1.getPrevScore();
						if (this->player2Counter < player2.getPrevScore()) this->player2Counter = player2.getPrevScore();

						if (arduboy.everyXFrames(16)) {

							if (this->player1Counter < player1.getScore() || this->player2Counter < player2.getScore()) {
								if (this->player1Counter < player1.getScore()) {
									this->player1Counter++;
								}
								if (this->player2Counter < player2.getScore()) {
									this->player2Counter++;
								}
							}
							else {
								this->counter = 65;
							}

						}
						break;


					case 80 ... 94:
					case 110 ... 124:
					case 140 ... 154:
					case 170 ... 184:
						this->counter++;
						this->highlight = true;
						break;

					case 65 ... 79:
					case 95 ... 109:
					case 125 ... 139:
					case 155 ... 169:
						this->counter++;
						this->highlight = false;
						break;

					default: 
						moveToEOG(machine);
						break;

				}

				if (justPressed & A_BUTTON) {

					if (this->counter <= 49) {
						this->scoresTotal = addHandScoreToPlayerTotal(machine);
					}

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

						default: break;

					}

					player1.setPrevScore(player1.getScore());
					player2.setPrevScore(player2.getScore());
					this->counter = 0;

				}

			}

			break;

		case ViewState::EndOfGame:

			if (this->counter <= 65) {

				if (player1.getScore() >= 121) {
					saveMessage(F("Congratulations!\n          You Won!"), 2, 76, 48, DealerFace::Sad, BubbleAlignment::Score_Computer);
				}
				else {
					saveMessage(F("I am sorry@ you\n lost the game.\n       I won!"), 3, 67, 48, DealerFace::Happy, BubbleAlignment::Score_Computer);
				}
				this->counter++;

			}
			else {

				if (arduboy.everyXFrames(16)) {

					switch (this->counter) {

						case 66:

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

						case 67:
						case 69:
						case 71:
						case 73:
							this->highlight = false;
							this->counter++;
							break;

						case 68:
						case 70:
						case 72:
						case 74:
							this->highlight = true;
							this->counter++;
							break;

						default: break;

					}

				}

			}

			if (justPressed & A_BUTTON) {
				machine.changeState(GameStateType::GameOver); 
			}

			break;

	}

// 	if (justPressed & A_BUTTON) {
// //			machine.changeState(GameStateType_Board); 

// 		this->player1Counter = 0;
// 		this->player2Counter = 0;

// 		this->counter = 0;
// 		this->highlight = true;
// 		prevViewState = viewState;
// 		viewState = ViewState::DisplayScore_Board;

// 	}

}