#include "Score.h"

Score::Score() {}

uint8_t Score::getHand(uint8_t index) {

    return this->hand[index];

}

uint8_t Score::getScore() {

    return this->score;

}

void Score::setHand(uint8_t index, uint8_t card) {

    this->hand[index] = card;

}

void Score::setScore(uint8_t score) {

    this->score = score;

}


void Score::reset() {

    memset(this->hand, 0, (sizeof(this->hand) / sizeof(this->hand[0])));
    this->score = score;

}