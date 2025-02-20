#ifndef ___MAIN_H___
#define ___MAIN_H___

#include "Arduino.h"
#include "Charlieplex4Pin.h"

// #define BLINKING_TEST

Charlieplex4Pin hud(2, 3, 4, 5);

void led_update();

//  0-4 yellow, 5-9 red, 10-11 green, 13 clear all
int led_counter = 0;
unsigned long last_entry_ms = 0;
#define LED_UPDATE_DELAY_MS 500

#endif  //  ___MAIN_H___