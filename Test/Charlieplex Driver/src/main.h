#ifndef ___MAIN_H___
#define ___MAIN_H___

#include "Arduino.h"
#include "Charlieplex4Pin.h"
#include "FSMMacroV2.h"

// #define Charlieplex4Pin_TEST
#define Charlieplex4PinBlink_TEST
// #define BLINKING_TEST


#if defined(Charlieplex4Pin_TEST)
//  +-------------------------+
//  |  Charlieplex4Pin_TEST   |
//  +-------------------------+
Charlieplex4Pin hud(2, 3, 4, 5);

void led_update();

//  0-4 yellow, 5-9 red, 10-11 green, 13 clear all
int led_counter = 0;
unsigned long last_entry_ms = 0;
#define LED_UPDATE_DELAY_MS 500

#elif defined(Charlieplex4PinBlink_TEST)
//  +-------------------------+
//  |Charlieplex4PinBlink_TEST|
//  +-------------------------+

Charlieplex4PinBlink hud(2, 3, 4, 5);

CREATE_FSM(main_loop, 0)
void main_loop_update();
int light_counter;

#elif defined(BLINKING_TEST)
//  +-------------------------+
//  |      BLINKING_TEST      |
//  +-------------------------+

#endif

#endif  //  ___MAIN_H___