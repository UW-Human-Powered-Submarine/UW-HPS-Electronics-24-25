#include <Arduino.h>
#include "SegDisplay.h"

// Define Digital Pins
#define CLK 3
#define DIO 2

SegDisplay seg(CLK, DIO);


void setup() {
    Serial.begin(9600);
}

void loop() {

}