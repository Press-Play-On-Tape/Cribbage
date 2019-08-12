#include "CardUtils.h"

CardUtils::CardUtils() {}


uint8_t CardUtils::getCardValue(uint8_t cardNumber, bool pictureCardsAsTens) {
    uint8_t card = (cardNumber % 13) + 1;
    if (pictureCardsAsTens && card > 10) card = 10;
    return card;
}

Suit CardUtils::getCardSuit(uint8_t cardNumber) {
    return static_cast<Suit>(cardNumber / 13);
}

uint16_t CardUtils::getCardSort(uint8_t cardNumber) {
    return ((cardNumber % 13) * 10) + (cardNumber / 13);
}

void CardUtils::swap(uint8_t *xp, uint8_t *yp)  
{  
    uint8_t temp = *xp;  
    *xp = *yp;  
    *yp = temp;  
}  
  
// A function to implement bubble sort  
void CardUtils::sort(uint8_t arr[], uint8_t n) {  

    for (uint8_t i = 0; i < n-1; i++) {     
        
        for (uint8_t j = 0; j < n-i-1; j++) { 

            if (getCardSort(arr[j]) > getCardSort(arr[j+1]))  {
                swap(&arr[j], &arr[j+1]);  
            }

        }

    }

}  
