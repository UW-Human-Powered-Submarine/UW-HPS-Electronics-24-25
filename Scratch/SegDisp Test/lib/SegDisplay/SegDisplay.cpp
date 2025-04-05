#include "SegDisplay.h"

SegDisplay::SegDisplay(int pin_clk, int pin_dio) 
    :Scheduler(100000), display(pin_clk, pin_dio), depth(0), pitch(0), require_refresh(false) {
}

void SegDisplay::set_depth(float depth) {
    if (depth < 0) {
        this->set_depth(0);
    } else if (depth > 9.9) {
        this->set_depth(9.9);
    } else if (abs(depth - this->depth) > 1e-8) {
        //  digits are different than before
        this->depth = depth;
        this->require_refresh = true;
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
        this->require_refresh = true;
    }
}

void SegDisplay::event() {
    if (this->require_refresh) {
        //  display pitch then depth

        char display_string[6];

        display_string[0] = (int)(this->depth * 10) % 10 + 48;
        display_string[1] = (int)this->depth % 10 + 48;
        display_string[2] = ' ';

        display_string[3] = (int)abs(this->pitch) % 10;
        display_string[4] = (int)(abs(this->pitch) / 10) % 10;
        display_string[5] = (this->pitch < 0)? '-' : ' ';

        for (int i = 0; i < 5; i++) {
            Serial.print(display_string[i]);
        }

        display.showString(display_string);

        this->require_refresh = false;
    }
}
