#include <Arduino.h>
#include "SegDisplay.h"

// Define Digital Pins
#define CLK 3
#define DIO 2

SegDisplay seg(CLK, DIO);

float pitch = -10;
float depth = 0;

void setup() {
    Serial.begin(9600);
}

void loop() {
    seg.set_depth(depth);
    seg.set_pitch(pitch);
    pitch += 1;
    depth += 0.2;
    seg.update();
    delay(500);
}