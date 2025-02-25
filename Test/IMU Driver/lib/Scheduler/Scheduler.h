#ifndef ___SCHEDULER_H___
#define ___SCHEDULER_H___

#include "Arduino.h"

//  +-------------------------------------------------------------------------+
//  |   
//  |                                                                         |
//  |                                                                         |
//  |   Author     :    Zihui(Andy) Liu <liuzihui@uw.edu>                     |
//  |   Last Update:    February 25, 2025                                     |
//  +-------------------------------------------------------------------------+

class Scheduler {
public:
    Scheduler(unsigned long refresh_period_us);
    ~Scheduler();

    //  Call event() on given schedule/interval
    //  This should be called at highest frequency possible. 
    //  The actual frequency is regulated internally 
    void update();

    //  Forced refresh. Call event() imediately
    //  Loop() should call update() instead of this function
    void refresh();

    void set_refresh_period(unsigned long microsecond);
    void set_refresh_period_ms(unsigned long millisecond);

private:
    unsigned long refresh_period_us = 0;
    unsigned long prev_timestamp_us = 0;

    virtual void event() = 0;
};

#endif // SCHEDULER_H
