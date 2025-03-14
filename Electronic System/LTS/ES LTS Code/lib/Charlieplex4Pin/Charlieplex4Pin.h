#ifndef ___CHARLIEPLEX_4PIN_H___
#define ___CHARLIEPLEX_4PIN_H___

//  +-------------------------------------------------------------------------+
//  |   Charlieplex4Pin handles the multiplex function for the heads-up       |
//  |       display. Charlieplex4Pin.update() should be called at highest     |
//  |       frequency (<2ms), and the internal timer regulates the detailed   |
//  |       refresh schedule.                                                 |
//  |                                                                         |
//  |   Call Charlieplex4Pin.update() at highest frequency possible.          |
//  |       less than about 2 milliseconds,                                   |
//  |       otherwise the display will be too dim                             |
//  |                                                                         |
//  |   Uncomment "#define USE_HANDSOLDERED_BOARD_CONFIG" for handsoldered    |
//  |       board                                                             |
//  |                                                                         |
//  |   Author     :    Zihui(Andy) Liu <liuzihui@uw.edu>                     |
//  |   Last Update:    February 26, 2025                                     |
//  +-------------------------------------------------------------------------+

#include "Arduino.h"
#include "Scheduler.h"

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
const int CHARLIEPLEX_LED_GREEN[2] = {8, 11};
const int CHARLIEPLEX_LED_YELLOW[5] = {10, 7, 3, 6, 9};
const int CHARLIEPLEX_LED_RED[5] = {5, 4, 0, 1, 2};
#endif  //  USE_HANDSOLDERED_BOARD_CONFIG

//  +-------------------------------------------------------------------------+
//  |                      Charliplex 4 Pin Driver Class                      |
//  +-------------------------------------------------------------------------+

class Charlieplex4Pin: public Scheduler {
public:
    Charlieplex4Pin(int pin0, int pin1, int pin2, int pin3);
    Charlieplex4Pin(int pin0, int pin1, int pin2, int pin3, unsigned long refresh_period_microsecond);
    ~Charlieplex4Pin();

    void set_led_on_delay(unsigned long microsecond);
    unsigned long get_led_on_delay_us() const;

    //  Set led state
    //  index from permutation
    void set_led_state(int index_permutation, bool state);
    bool get_led_state(int index_permutation) const;

    void set_green_led(int index, bool state);
    bool get_green_led(int index) const;
    void set_red_led(int index, bool state);
    bool get_red_led(int index) const;
    void set_yellow_led(int index, bool state);
    bool get_yellow_led(int index) const;

    //  Set all states to 0
    void clear_states();

protected:
    unsigned long led_on_delay_us = 1000;
    
    int pin_array[4];   //  lookup table for pins
    bool led_states[12];

    void led_on(int pin_high, int pin_low, int pin_disconnect_1, int pin_disconnect_2) const;
    void led_on_permutation(int permutation_index) const;
    void event() override;
};

//  +-------------------------------------------------------------------------+
//  |            Charliplex 4 Pin Driver with Blink Function Class            |
//  +-------------------------------------------------------------------------+

enum CharlieplexBlinkStates {
    CBS_OFF, CBS_ON, CBS_BLINK_SLOW, CBS_BLINK_FAST
};

class Charlieplex4PinBlink: public Charlieplex4Pin {
public:

    Charlieplex4PinBlink(int pin0, int pin1, int pin2, int pin3);
    Charlieplex4PinBlink(
        int pin0, int pin1, int pin2, int pin3, 
        unsigned long slow_blink_period_ms, unsigned long fast_blink_period_ms
    );
    Charlieplex4PinBlink(
        int pin0, int pin1, int pin2, int pin3, 
        int refresh_period_microsecond, 
        unsigned long slow_blink_period_ms, unsigned long fast_blink_period_ms
    );
    ~Charlieplex4PinBlink();

    //  Set led state
    //  index from permutation
    void set_led_state(int index_permutation, CharlieplexBlinkStates state_blink);
    CharlieplexBlinkStates get_led_state(int index_permutation) const;

    void set_green_led(int index, CharlieplexBlinkStates state_blink);
    CharlieplexBlinkStates get_green_led(int index) const;
    void set_red_led(int index, CharlieplexBlinkStates state_blink);
    CharlieplexBlinkStates get_red_led(int index) const;
    void set_yellow_led(int index, CharlieplexBlinkStates state_blink);
    CharlieplexBlinkStates get_yellow_led(int index) const;

    void set_slow_blink_period_ms(unsigned long milliseconds); 
    unsigned long get_slow_blink_period_ms() const;
    void set_fast_blink_period_ms(unsigned long milliseconds); 
    unsigned long get_fast_blink_period_ms() const;

    //  Set all states to CBS_OFF state
    void clear_states(); 

private:
    unsigned long slow_blink_period_ms;
    unsigned long fast_blink_period_ms;

    unsigned long slow_blink_resume_time_ms;
    bool slow_blink_current_status;

    unsigned long fast_blink_resume_time_ms;
    bool fast_blink_current_status;

    CharlieplexBlinkStates led_blink_states[12];

    void event() override;

    void slow_blink_update();
    void fast_blink_update();
};

#endif  //  ___CHARLIEPLEX_H___
