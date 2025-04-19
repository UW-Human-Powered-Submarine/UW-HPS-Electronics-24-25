#include <Arduino.h>
#include "SegDisplay.h"

// Define Digital Pins
#define CLK 9
#define DIO 8

unsigned long update_time = 0;

SegDisplay seg(CLK, DIO);

float pitch = -10;
float depth = 0;

void setup() {
    Serial.begin(9600);
    seg.begin();
    // seg.set_static_text_blink_mode(SDBS_BLINK_SLOW);
}

void loop() {
    if (update_time < millis()) {
        // seg.set_depth(depth);
        // seg.set_pitch(pitch);
        // pitch += 1;
        // depth += 0.2;

        // update_time += 1000;

        seg.set_static_text("NEUTRL");
    }
    seg.update();
    delay(1);
}