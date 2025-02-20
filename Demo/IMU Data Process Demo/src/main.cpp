#include "main.h"
#ifndef BLINKING_TEST

void setup() {
    
}

void loop() {

}

#else
//  BLINKING TEST CODE
#define DELAY_MS 500
#define LED_PIN 13
void setup() {
    pinMode(LED_PIN, OUTPUT);
}
void loop() {
    digitalWrite(LED_PIN, HIGH);
    delay(DELAY_MS);
    digitalWrite(LED_PIN, LOW);
    delay(DELAY_MS);
}
#endif  //  BLINKING_TEST