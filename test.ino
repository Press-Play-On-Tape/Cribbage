#include <Arduboy2.h>


Arduboy2 arduboy;

uint8_t hand[6];
uint8_t deck[52];

uint8_t discards[6];

uint8_t getCardValue(uint8_t cardNumber) {
    return cardNumber % 13;
}

uint8_t getCardSuit(uint8_t cardNumber) {
    return cardNumber / 13;
}

uint16_t getCardSort(uint8_t cardNumber) {
    return ((cardNumber % 13) * 10) + (cardNumber / 13);
}

void printHand() {
    for (uint8_t x=0; x < 6; x++) {
        Serial.print(getCardValue(hand[x]));
        uint8_t suit = getCardSuit(hand[x]);
        switch (suit) {
            case 0: Serial.print("S "); break;
            case 1: Serial.print("C "); break;
            case 2: Serial.print("D "); break;
            case 3: Serial.print("H "); break;
        }
    }
    Serial.println("  ");
    for (uint8_t x=0; x < 6; x++) {
        Serial.print(discards[x]);
        Serial.print("  ");
    }
    Serial.println("  ");
}

void swap(uint8_t *xp, uint8_t *yp)  
{  
    uint8_t temp = *xp;  
    *xp = *yp;  
    *yp = temp;  
}  
  
// A function to implement bubble sort  
void bubbleSort(uint8_t arr[], uint8_t n) {  

    for (uint8_t i = 0; i < n-1; i++) {     
        
        for (uint8_t j = 0; j < n-i-1; j++) { 

            if (getCardSort(arr[j]) > getCardSort(arr[j+1]))  {
                swap(&arr[j], &arr[j+1]);  
            }

        }

    }

}  

void setup()
{
    arduboy.begin();
    arduboy.clear();

    hand[0] = 27;
    hand[1] = 34;
    hand[2] = random(0, 52);
    hand[3] = random(0, 52);
    hand[4] = random(0, 52);
    hand[5] = random(0, 52);

    bubbleSort(hand, 6);
    printHand();

}

void loop() {

    // Never discard 5's ..
    
    for (uint8_t x = 0; x < 6; x++) {
        if (getCardValue(hand[x]) == 5) discards[x] = 1; // Never
    }

    // Avoid discarding pairs ..
    
    for (uint8_t x = 0; x < 5; x++) {
        if (getCardValue(hand[x]) == getCardValue(hand[x + 1]) && discards[x] == 0 && discards[x + 1] == 0) {
            discards[x] = 2; // Avoid
            discards[x + 1] = 2; // Avoid
        }
    }

    // Avoid discarding seq ..
    
    for (uint8_t x = 0; x < 5; x++) {
        for (uint8_t y = x + 1; y < 6; y++) {
            if (getCardValue(hand[x]) + 1 == getCardValue(hand[y])) {
                Serial.print("x:");
                Serial.print(x);
                Serial.print(", y:");
                Serial.println(y);
                if (discards[x] == 0) discards[x] = 3; //avoid
                if (discards[y] == 0) discards[y] = 3; //avoid
            }
        }
    }

    printHand();




    

}