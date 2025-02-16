#ifndef ___CHARLIEPLEX_4PIN_H___
#define ___CHARLIEPLEX_4PIN_H___

#include "Arduino.h"

#define USE_HANDSOLDERED_BOARD_CONFIG

//  loockup table for charlieplex circuit. 
//  each entry means {high, low, disconnect, disconnect}
//  two disconnect pins are interchangable
const int CHARLIEPLEX_CIRCUIT_PERMUTATION[12][4] = {
    {0, 1, 2, 3}, {0, 2, 1, 3}, {0, 3, 1, 2},
    {1, 0, 2, 3}, {1, 2, 0, 3}, {1, 3, 0, 2},
    {2, 0, 1, 3}, {2, 1, 0, 3}, {2, 3, 0, 1},
    {3, 0, 1, 2}, {3, 1, 0, 2}, {3, 2, 0, 1}
};

#ifdef USE_HANDSOLDERED_BOARD_CONFIG
//  Map from individual LED to permutation entry in CHARLIEPLEX_CIRCUIT_PERMUTATION
const int CHARLIEPLEX_LED_GREEN[2] = {3, 0};
const int CHARLIEPLEX_LED_YELLOW[5] = {1, 4, 8, 5, 2};
const int CHARLIEPLEX_LED_RED[5] = {6, 7, 11, 10, 9};
#endif  //  USE_HANDSOLDERED_BOARD_CONFIG

class Charlieplex4Pin {
private:
    int pin0;
    int pin1;
    int pin2;
    int pin3;

    unsigned int refresh_period_us;

    unsigned int led_on_delay_us = 1000;
    unsigned long prev_timestamp_us = 0;

    bool led_states[12];

    void led_on(int pin_high, int pin_low, int pin_disconnect_1, int pin_disconnect_2) const;
    void led_on_permutation(int permutation_index) const;
    

public:
    Charlieplex4Pin(int pin0, int pin1, int pin2, int pin3);
    Charlieplex4Pin(int pin0, int pin1, int pin2, int pin3, int refresh_period_microsecond);
    ~Charlieplex4Pin();

    //  Refresh the Charliplex matrix. 
    //  This should be called at highest frequency possible. 
    //  The actual frequency is regulated internally 
    void update();

    //  Forced refresh. 
    //  Loop() should call Charlieplex.update() instead of this function
    void refresh() const;

    void set_led_on_delay(int microsecond);
    void set_refresh_period(int microsecond);

    //  Set led state
    //  index from permutation
    void set_led_state(int index_permutation, bool state);

    void set_green_led(int index, bool state);
    void set_red_led(int index, bool state);
    void set_yellow_led(int index, bool state);
};



#endif  //  ___CHARLIEPLEX_H___
