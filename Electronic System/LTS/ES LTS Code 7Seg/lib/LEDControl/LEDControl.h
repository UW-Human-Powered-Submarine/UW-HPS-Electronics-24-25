#ifndef ___LED_CONTROL_H___
#define ___LED_CONTROL_H___

#include "Arduino.h"
#include "Scheduler.h"
#include "FSMMacroV2.h"

class LEDControl: public Scheduler {
public:
    LEDControl(int pin);

    void begin();

    void on();
    void off();
    void blink(int period_ms);
private:
    int pin; 
    unsigned long half_period_ms;

    CREATE_FSM(LED, 0);
    void event();
};


#endif