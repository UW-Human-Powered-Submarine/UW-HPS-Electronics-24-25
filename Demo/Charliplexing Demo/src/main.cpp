#include <Arduino.h>

#define PIN_ONBOARD_LED 13

#define CHARLIEPLEX_PIN(cp_pin) (cp_pin + 2)

const int permutations[12][4] = {
    {0, 1, 2, 3}, {0, 2, 1, 3}, {0, 3, 1, 2},
    {1, 0, 2, 3}, {1, 2, 0, 3}, {1, 3, 0, 2},
    {2, 0, 1, 3}, {2, 1, 0, 3}, {2, 3, 0, 1},
    {3, 0, 1, 2}, {3, 1, 0, 2}, {3, 2, 0, 1}
};

const int green[2] = {3, 0};
const int yellow[5] = {1, 4, 8, 5, 2};
const int red[5] = {6, 7, 11, 10, 9};

void led_on(int pin_high, int pin_low, int pin_disconnect_1, int pin_disconnect_2) {
    pinMode(CHARLIEPLEX_PIN(pin_high), OUTPUT);
    digitalWrite(CHARLIEPLEX_PIN(pin_high), HIGH);

    pinMode(CHARLIEPLEX_PIN(pin_low), OUTPUT);
    digitalWrite(CHARLIEPLEX_PIN(pin_low), LOW);

    pinMode(CHARLIEPLEX_PIN(pin_disconnect_1), INPUT);
    pinMode(CHARLIEPLEX_PIN(pin_disconnect_2), INPUT);
}

void led_on_perm(int i) {
    led_on(
        permutations[i][0],
        permutations[i][1],
        permutations[i][2],
        permutations[i][3]
    );
}

void setup() {
    
}

void loop() {
    // for (int i = 0; i < 2; i++) {
    //     led_on_perm(green[i]);
    //     delay(500);
    // }

    // for (int i = 0; i < 5; i++) {
    //     led_on_perm(red[i]);
    //     delay(500);
    // }

    // for (int i = 0; i < 5; i++) {
    //     led_on_perm(yellow[i]);
    //     delay(500);
    // }

    led_on_perm(red[2]);
    // delay(1);
    led_on_perm(yellow[2]);
    // delay(1);
}

