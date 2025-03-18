#ifndef ___SCHEDULER_H___
#define ___SCHEDULER_H___

#include "Arduino.h"

//  +-------------------------------------------------------------------------+
//  |   Scheduler Base class.                                                 |
//  |                                                                         |
//  |   Calling update() at the highest frequency. Scheduler will             |
//  |       automatically run event() at approximately fixed interval.        |
//  |                                                                         |
//  |   Calling refresh() will run event() imediately and reset internal      |
//  |       timer. Call refresh_no_timer_reset() if you don't want to         |
//  |       reset timer                                                       |
//  |                                                                         |
//  |   The child class must implement event() method                         |
//  |                                                                         |
//  |   SchedulerFSM is the base class for FSM styled event                   |
//  |                                                                         |
//  |   Author     :    Zihui(Andy) Liu <liuzihui@uw.edu>                     |
//  |   Last Update:    March 15, 2025                                        |
//  +-------------------------------------------------------------------------+

//  +-------------------------------------------------------------------------+
//  |                                Scheduler                                |
//  +-------------------------------------------------------------------------+

class Scheduler {
public:
    Scheduler(unsigned long refresh_period_us);
    ~Scheduler();

    //  Call event() on given schedule/interval
    //  This should be called at highest frequency possible. 
    //  The actual frequency is regulated internally 
    void update();

    //  Forced refresh. Call event() imediately and reset timer
    //  Loop() should call update() instead of this function
    void refresh();

    //  Forced refresh. Call event() imediately
    //  Loop() should call update() instead of this function
    void refresh_no_timer_reset();

    void set_refresh_period(unsigned long microsecond);
    void set_refresh_period_ms(unsigned long millisecond);

    //  Enable accurate_mode, so the event counter is set based on the 
    //      previous time stamp, instead of the current time. This will cause 
    //      series issue if the tasks are saturated. 
    void enable_accurate_mode();

    void disable_accurate_mode();

protected:
    virtual void event() = 0;

    unsigned long refresh_period_us;
    unsigned long prev_timestamp_us;

    bool accurate_mode;
};

//  +-------------------------------------------------------------------------+
//  |                              SchedulerFSM                               |
//  +-------------------------------------------------------------------------+

//  Same as Schduler, 
//      but event will be a finite state machine style code with multiple entry. 
class SchedulerFSM: public Scheduler {
public:
    SchedulerFSM(unsigned long refresh_period_us);
    ~SchedulerFSM();

    //  Call event() on given schedule/interval
    //  This should be called at highest frequency possible. 
    //  The actual frequency is regulated internally 
    void update();

    //  Forced refresh. Call event() imediately and reset timer
    //  Loop() should call update() instead of this function
    //  NOTE: this will block the program until FSM event has ended
    void refresh();

    //  Forced refresh. Call event() imediately
    //  Loop() should call update() instead of this function
    //  NOTE: this will block the program until FSM event has ended
    void refresh_no_timer_reset();    

protected:
    //  this is the FSM style event called in set interval. 
    //  by design this has multiple entry, call fsm_finished() to end FSM
    virtual void event() override;

    bool get_is_fsm_finished() const;
    
    //  Call this method if the scheduled fsm sequence has ended. 
    void fsm_finished();
    void fsm_start();

private:
    bool is_fsm_finished;
};

#endif // SCHEDULER_H
