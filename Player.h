#pragma once

class Player { 
    
    public:

        Player();

        uint8_t calculateHandScore(uint8_t turnUp);
        uint8_t calculateCribScore(uint8_t turnUp);

    public: 

        void addToHand(unt8_t card);
        void addToCrib(unt8_t card);

        uin8_t removeFromHand(unt8_t index);
        void discardToCrib(uint8_t *card1, uint8_t *card2);
        
        void resethand();

    private:

        uint8_t handIdx = 0;
        uint8_t cribIdx = 0;
        uint8_t scoreIdx = 0;
        uint8_t hand[7];
        uint8_t crib[5];
        uint8_t calcScore[5];
        Score scores[10];

};
