#include "Charlieplex4Pin.h"

//  +-------------------------------------------------------------------------+
//  |                      Charliplex 4 Pin Driver Class                      |
//  +-------------------------------------------------------------------------+

Charlieplex4Pin::Charlieplex4Pin(int pin0, int pin1, int pin2, int pin3)
    : Charlieplex4Pin(pin0, pin1, pin2, pin3, 2000) {
}

Charlieplex4Pin::Charlieplex4Pin(
    int pin0, int pin1, int pin2, int pin3, int refresh_period_microsecond
)   : Scheduler(refresh_period_microsecond)
    , led_on_delay_us(1000) {

    this->pin_array[0] = pin0;
    this->pin_array[1] = pin1;
    this->pin_array[2] = pin2;
    this->pin_array[3] = pin3;

    for (int i1 = 0; i1 < 12; i1++) {
        this->led_states[i1] = false;
    }
}

Charlieplex4Pin::~Charlieplex4Pin() {
}

void Charlieplex4Pin::set_led_on_delay(unsigned long microsecond) {
    this->led_on_delay_us = microsecond;
}

unsigned long Charlieplex4Pin::get_led_on_delay_us() const {
    return this->led_on_delay_us;
}

void Charlieplex4Pin::set_led_state(int index_permutation, bool state) {
    this->led_states[index_permutation] = state;
}

bool Charlieplex4Pin::get_led_state(int index_permutation) const {
    return this->led_states[index_permutation];
}

void Charlieplex4Pin::set_green_led(int index, bool state) {
    this->led_states[CHARLIEPLEX_LED_GREEN[index]] = state;
}

bool Charlieplex4Pin::get_green_led(int index) const {
    return this->led_states[CHARLIEPLEX_LED_GREEN[index]];
}

void Charlieplex4Pin::set_red_led(int index, bool state) {
    this->led_states[CHARLIEPLEX_LED_RED[index]] = state;
}

bool Charlieplex4Pin::get_red_led(int index) const {
    return this->led_states[CHARLIEPLEX_LED_RED[index]];
}

void Charlieplex4Pin::set_yellow_led(int index, bool state) {
    this->led_states[CHARLIEPLEX_LED_YELLOW[index]] = state;
}

bool Charlieplex4Pin::get_yellow_led(int index) const {
    return this->led_states[CHARLIEPLEX_LED_YELLOW[index]];
}

void Charlieplex4Pin::clear_states() {
    for (int i = 0; i < 12; i++) {
        this->led_states[i] = false;
    }
}

void Charlieplex4Pin::led_on(
    int pin_high, int pin_low, int pin_disconnect_1, int pin_disconnect_2
)const {
    pinMode(pin_array[pin_high], OUTPUT);
    digitalWrite(pin_array[pin_high], HIGH);

    pinMode(pin_array[pin_low], OUTPUT);
    digitalWrite(pin_array[pin_low], LOW);

    pinMode(pin_array[pin_disconnect_1], INPUT);
    pinMode(pin_array[pin_disconnect_2], INPUT);
}

void Charlieplex4Pin::led_on_permutation(int i) const{
    led_on(
        CHARLIEPLEX_CIRCUIT_PERMUTATION[i][0],
        CHARLIEPLEX_CIRCUIT_PERMUTATION[i][1],
        CHARLIEPLEX_CIRCUIT_PERMUTATION[i][2],
        CHARLIEPLEX_CIRCUIT_PERMUTATION[i][3]
    );
}

void Charlieplex4Pin::event() {
    for (int i = 0; i < 12; i++) {
        if (led_states[i]) {
            this->led_on_permutation(i);
            delayMicroseconds(this->led_on_delay_us);
        }
    }

    //  turn off all pins, prevent from inconsistent brightness
    for (int i = 0; i < 4; i++) {
        digitalWrite(pin_array[i], LOW);
    }
}

//  +-------------------------------------------------------------------------+
//  |            Charliplex 4 Pin Driver with Blink Function Class            |
//  +-------------------------------------------------------------------------+

Charlieplex4PinBlink::Charlieplex4PinBlink(int pin0, int pin1, int pin2, int pin3)
    : Charlieplex4PinBlink(pin0, pin1, pin2, pin3, 250, 500) {
}

Charlieplex4PinBlink::Charlieplex4PinBlink(
    int pin0, int pin1, int pin2, int pin3, 
    unsigned long slow_blink_period_ms, unsigned long fast_blink_period_ms
)
    : Charlieplex4Pin(pin0, pin1, pin2, pin3)
    , slow_blink_period_ms(slow_blink_period_ms), fast_blink_period_ms(fast_blink_period_ms)
    , slow_blink_resume_time_ms(0), slow_blink_current_status(false)
    , fast_blink_resume_time_ms(0), fast_blink_current_status(false) {

    for (int i = 0; i < 12; i++) {
        this->led_blink_states[i] = CharlieplexBlinkStates::CBS_OFF;
    }
}

Charlieplex4PinBlink::Charlieplex4PinBlink(
    int pin0, int pin1, int pin2, int pin3, 
    int refresh_period_microsecond, 
    unsigned long slow_blink_period_ms, unsigned long fast_blink_period_ms
)
    : Charlieplex4Pin(pin0, pin1, pin2, pin3, refresh_period_microsecond)
    , slow_blink_period_ms(slow_blink_period_ms), fast_blink_period_ms(fast_blink_period_ms) {

    for (int i = 0; i < 12; i++) {
            this->led_blink_states[i] = CharlieplexBlinkStates::CBS_OFF;
        }
}

Charlieplex4PinBlink::~Charlieplex4PinBlink() {
}

void Charlieplex4PinBlink::set_green_led(int index, CharlieplexBlinkStates state_blink) {
    this->led_blink_states[CHARLIEPLEX_LED_GREEN[index]] = state_blink;
}

CharlieplexBlinkStates Charlieplex4PinBlink::get_green_led(int index) const {
    return this->led_blink_states[CHARLIEPLEX_LED_GREEN[index]];
}

void Charlieplex4PinBlink::set_red_led(int index, CharlieplexBlinkStates state_blink) {
    this->led_blink_states[CHARLIEPLEX_LED_RED[index]] = state_blink;
}

CharlieplexBlinkStates Charlieplex4PinBlink::get_red_led(int index) const {
    return this->led_blink_states[CHARLIEPLEX_LED_RED[index]];
}

void Charlieplex4PinBlink::set_yellow_led(int index, CharlieplexBlinkStates state_blink) {
    this->led_blink_states[CHARLIEPLEX_LED_YELLOW[index]] = state_blink;
}

CharlieplexBlinkStates Charlieplex4PinBlink::get_yellow_led(int index) const {
    return this->led_blink_states[CHARLIEPLEX_LED_YELLOW[index]];
}

void Charlieplex4PinBlink::set_slow_blink_period_ms(unsigned long milliseconds) {
    this->slow_blink_period_ms = milliseconds;
}

unsigned long Charlieplex4PinBlink::get_slow_blink_period_ms() const {
    return this->slow_blink_period_ms;
}

void Charlieplex4PinBlink::set_fast_blink_period_ms(unsigned long milliseconds) {
    this->fast_blink_period_ms = milliseconds;
}

unsigned long Charlieplex4PinBlink::get_fast_blink_period_ms() const {
    return this->fast_blink_period_ms;
}

void Charlieplex4PinBlink::event() {
    unsigned long current_time = millis();
    if (this->slow_blink_resume_time_ms <= current_time) {
        slow_blink_update();
        this->slow_blink_resume_time_ms = current_time + this->slow_blink_period_ms;
    }

    if (this->fast_blink_resume_time_ms <= current_time) {
        fast_blink_update();
        this->fast_blink_resume_time_ms = current_time = this->fast_blink_period_ms;
    }

    //  Calling parent's method to update the lower level charlieplex function
    Charlieplex4Pin::event();
}

void Charlieplex4PinBlink::slow_blink_update() {
    this->slow_blink_current_status = !this->slow_blink_current_status;
    for (int i = 0; i < 12; i++) {
        if (this->led_blink_states[i] == CharlieplexBlinkStates::CBS_BLINK_SLOW) {
            Charlieplex4Pin::set_led_state(i, this->slow_blink_current_status);
        }
    }
}

void Charlieplex4PinBlink::fast_blink_update() {
    this->fast_blink_current_status = !this->fast_blink_current_status;
    for (int i = 0; i < 12; i++) {
        if (this->led_blink_states[i] == CharlieplexBlinkStates::CBS_BLINK_FAST) {
            Charlieplex4Pin::set_led_state(i, this->fast_blink_current_status);
        }
    }
}
