#include "Scheduler.h"

Scheduler::Scheduler(unsigned long refresh_period_us)
    : refresh_period_us(refresh_period_us), prev_timestamp_us(0) {
}

Scheduler::~Scheduler() {
}

void Scheduler::update() {
    unsigned long curr_time = micros();
    if (
        (this->prev_timestamp_us + this->refresh_period_us <= curr_time) 
        || (curr_time < this->prev_timestamp_us)   //  detect micros() overflow
    ) {
        // Serial.println(curr_time);
        this->prev_timestamp_us = curr_time;    //  record the new timestamp

        this->event();
    }
}

void Scheduler::refresh() {
    this->event();
}

void Scheduler::set_refresh_period(unsigned long microsecond){
    this->prev_timestamp_us = microsecond;
}

void Scheduler::set_refresh_period_ms(unsigned long millisecond) {
    this->prev_timestamp_us = millisecond * 1000;
}