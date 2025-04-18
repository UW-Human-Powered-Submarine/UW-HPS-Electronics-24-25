#include "SegDisplay.h"

SegDisplay::SegDisplay(int pin_clk, int pin_dio) 
    : Scheduler(10000), display(pin_clk, pin_dio)
    , should_refresh(false), depth(0), pitch(0) {
}

void SegDisplay::begin() {
    display.begin();
}

void SegDisplay::set_depth(float depth) {
    if (depth < 0) {
        this->set_depth(0);
    } else if (depth > 9.9) {
        this->set_depth(9.9);
    } else if (abs(depth - this->depth) > 1e-8) {
        //  digits are different than before
        this->depth = depth;
        should_refresh = true;
    }
}

void SegDisplay::set_pitch(float pitch) {
    if (pitch < -99) {
        this->set_pitch(-99);
    } else if (pitch > 99) {
        this->set_pitch(99);
    } else if (abs(pitch - this->pitch) > 1e-8) {
        //  digits are different than before
        this->pitch = pitch;
        should_refresh = true;
    }
}

TM1637TinyDisplay6 *SegDisplay::get_display() {
    return &(this->display);
}

void SegDisplay::event() {
    this->refresh();
}

void SegDisplay::refresh() {
    if (should_refresh) {
            //  display pitch then depth

        char display_string[7];

        display_string[0] = (this->pitch < 0)? '-' : ' ';
        display_string[1] = (int)(abs(this->pitch) / 10) % 10 + 48;
        display_string[2] = (int)abs(this->pitch) % 10 + 48;
        
        display_string[3] = ' ';
        display_string[4] = (int)this->depth % 10 + 48;
        display_string[5] = (int)(this->depth * 10) % 10 + 48;

        display_string[6] = '\0';

        //  0b00101000 is the bit mask for the dot placement
        display.showString(display_string, 6U, 0U, 0b00101000);
        
        should_refresh = false;
    }
}
