#include "Scheduler.h"

Scheduler::Scheduler(unsigned long refresh_period_us)
    : refresh_period_us(refresh_period_us), prev_timestamp_us(0)
    , accurate_mode(false) {
}

Scheduler::~Scheduler() {
}

void Scheduler::update() {
    unsigned long curr_time = micros();
    if (
        (this->prev_timestamp_us + this->refresh_period_us <= curr_time) 
        || (curr_time < this->prev_timestamp_us)   //  detect micros() overflow
    ) {
        if (this->accurate_mode) {
            //  increment based on the previous time stamp
            this->prev_timestamp_us += this->refresh_period_us;
        } else {
            //  increment based on the current time stamp
            this->prev_timestamp_us = curr_time;    
        }
        

        this->event();
    }
}

void Scheduler::refresh() {
    this->prev_timestamp_us = micros();    //  record the new timestamp
    this->event();
}

void Scheduler::refresh_no_timer_reset() {
    this->event();
}

void Scheduler::set_refresh_period(unsigned long microsecond)
{
    this->refresh_period_us = microsecond;
}

void Scheduler::set_refresh_period_ms(unsigned long millisecond) {
    this->refresh_period_us = millisecond * 1000;
}

void Scheduler::enable_accurate_mode() {
    this->accurate_mode = true;
}

void Scheduler::disable_accurate_mode() {
    this->accurate_mode = false;
}
