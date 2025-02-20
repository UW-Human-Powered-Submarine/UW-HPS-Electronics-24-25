#include "main.h"
#ifndef BLINKING_TEST

void setup() {
    Serial.begin(9600);
}

void loop() {
    hud.update();
    led_update();
}

void led_update() {
    unsigned long curr_time_ms = millis();
    if (last_entry_ms + LED_UPDATE_DELAY_MS <= curr_time_ms) {
        last_entry_ms = curr_time_ms;

        if (led_counter == 12) {
            //  clear states;
            hud.clear_states();
        } else {
            //  update LEDs
            int color = led_counter / 5;
            int led_index = led_counter % 5;
            
            if (color == 0) {
                hud.set_yellow_led(led_index, true);
            } else if (color == 1) {
                hud.set_red_led(led_index, true);
            } else {
                hud.set_green_led(led_index, true);
            }
        }

        //  update states
        led_counter = (led_counter + 1) % 13;
    }   
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
