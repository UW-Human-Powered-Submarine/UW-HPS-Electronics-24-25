#include "main.h"
#if defined(Charlieplex4Pin_TEST) 
//  +-------------------------+
//  |  Charlieplex4Pin_TEST   |
//  +-------------------------+

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

#elif defined(Charlieplex4PinBlink_TEST)
//  +-------------------------+
//  |Charlieplex4PinBlink_TEST|
//  +-------------------------+

void setup() {
    Serial.begin(9600);
    hud.set_fast_blink_period_ms(100);
    hud.set_slow_blink_period_ms(200);
}

void loop() {
    hud.update();
    main_loop_update();
}

void main_loop_update() {
    SETUP_FSM(main_loop)

    STATE(0) {
        light_counter = 0;
        TO_NEXT;
    }

    STATE(1) {
        hud.set_yellow_led(light_counter % 5,           CharlieplexBlinkStates::CBS_OFF);
        hud.set_yellow_led((light_counter + 1) % 5,     CharlieplexBlinkStates::CBS_ON);
        hud.set_yellow_led((light_counter + 1)  % 5,    CharlieplexBlinkStates::CBS_BLINK_SLOW);
        hud.set_yellow_led((light_counter + 1)  % 5,    CharlieplexBlinkStates::CBS_BLINK_FAST);

        hud.set_red_led(light_counter % 5,          CharlieplexBlinkStates::CBS_OFF);
        hud.set_red_led((light_counter + 1) % 5,    CharlieplexBlinkStates::CBS_ON);
        hud.set_red_led((light_counter + 1)  % 5,   CharlieplexBlinkStates::CBS_BLINK_SLOW);
        hud.set_red_led((light_counter + 1)  % 5,   CharlieplexBlinkStates::CBS_BLINK_FAST);

        hud.set_green_led(0, static_cast<CharlieplexBlinkStates>(light_counter % 4));
        hud.set_green_led(1, static_cast<CharlieplexBlinkStates>((light_counter + 1) % 4));

        light_counter = (light_counter + 1) % 10;
        SLEEP(2000);
    }
}

 
#elif defined(BLINKING_TEST)
//  +-------------------------+
//  |      BLINKING_TEST      |
//  +-------------------------+

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
