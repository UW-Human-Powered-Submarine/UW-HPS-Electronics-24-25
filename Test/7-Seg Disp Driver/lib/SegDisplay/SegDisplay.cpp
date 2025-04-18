#include "SegDisplay.h"

SegDisplay::SegDisplay(int pin_clk, int pin_dio) 
    : Scheduler(1000), display(pin_clk, pin_dio)
    , should_refresh(false)
    , fast_flash_state(true), fast_flash_rate_ms(200), fast_flash_next_update_time_ms(0)
    , slow_flash_state(true), slow_flash_rate_ms(500), slow_flash_next_update_time_ms(0)
    , is_display_static_text(false)
    , static_text(), display_static_text(true), static_text_display_mode(SDBS_ON)
    , depth(0), display_depth(true), depth_display_mode(SDBS_ON)
    , pitch(0), display_pitch(true), pitch_display_mode(SDBS_ON) {
}

void SegDisplay::begin() {
    display.begin();
}

void SegDisplay::set_static_text(const char *static_text) {
    if (strlen(static_text) <= 7) {
        strcpy(this->static_text, static_text);

        this->is_display_static_text = true;
        this->should_refresh = true;
    }
}

void SegDisplay::set_static_text_blink_mode(SegDisplayBlinkState mode) {
    this->static_text_display_mode = mode;
    if (mode == SDBS_ON) {
        this->display_static_text = true;
    }
    this->should_refresh = true;
}

void SegDisplay::set_depth(float depth)
{
    if (depth < 0) {
        this->set_depth(0);
    } else if (depth > 9.9) {
        this->set_depth(9.9);
    } else if (abs(depth - this->depth) > 1e-8) {
        this->depth = depth;
    }

    this->is_display_static_text = false;
    this->should_refresh = true;
}

void SegDisplay::set_depth_blink_mode(SegDisplayBlinkState mode) {
    this->depth_display_mode = mode;
    if (mode == SDBS_ON) {
        this->display_depth = true;
    }
    this->should_refresh = true;
}

void SegDisplay::set_pitch(float pitch) {   
    if (pitch < -99) {
        this->set_pitch(-99);
    } else if (pitch > 99) {
        this->set_pitch(99);
    } else if (abs(pitch - this->pitch) > 1e-8) {
        this->pitch = pitch;
    }

    this->is_display_static_text = false;
    this->should_refresh = true;
}

void SegDisplay::set_pitch_blink_mode(SegDisplayBlinkState mode) {
    this->pitch_display_mode = mode;
    if (mode == SDBS_ON) {
        this->display_pitch = true;
    }
    this->should_refresh = true;
}

void SegDisplay::set_to_static_text_mode() {
    this->is_display_static_text = true;
    this->should_refresh = true;
}

void SegDisplay::set_to_sensor_info_mode() {
    this->is_display_static_text = false;
    this->should_refresh = true;
}

TM1637TinyDisplay6 *SegDisplay::get_display() {
    return &(this->display);
}

void SegDisplay::event() {
    unsigned long curr_time = millis();

    if (curr_time > this->fast_flash_next_update_time_ms) {
        this->fast_flash_state = !this->fast_flash_state;

        if (this->is_display_static_text) {
            //  text mode
            if (this->static_text_display_mode == SDBS_BLINK_FAST) {
                this->display_static_text = this->fast_flash_state;
                this->should_refresh = true;
            }
        } else {
            //  info mode
            if (this->pitch_display_mode == SDBS_BLINK_FAST) {
                this->display_pitch = this->fast_flash_state;
                this->should_refresh = true;
            }
            if (this->depth_display_mode == SDBS_BLINK_FAST) {
                this->display_depth = this->fast_flash_state;
                this->should_refresh = true;
            }
        }

        this->fast_flash_next_update_time_ms += this->fast_flash_rate_ms;
    }

    if (curr_time > this->slow_flash_next_update_time_ms) {
        this->slow_flash_state = !this->slow_flash_state;

        if (this->is_display_static_text) {
            //  text mode
            if (this->static_text_display_mode == SDBS_BLINK_SLOW) {
                this->display_static_text = this->slow_flash_state;
                this->should_refresh = true;
            }
        } else {
            //  info mode
            if (this->pitch_display_mode == SDBS_BLINK_SLOW) {
                this->display_pitch = this->slow_flash_state;
                this->should_refresh = true;
            }
            if (this->depth_display_mode == SDBS_BLINK_SLOW) {
                this->display_depth = this->slow_flash_state;
                this->should_refresh = true;
            }
        }

        this->slow_flash_next_update_time_ms += this->slow_flash_rate_ms;
    }

    this->refresh();
}

void SegDisplay::refresh() {
    if (this->should_refresh) {
            
        if (this->is_display_static_text)  {
            //  display static text
            if (display_static_text) {
                display.showString(this->static_text, 6U, 0U, 0b00000000);
            } else {
                display.showString("      ", 6U, 0U, 0b00000000);
            }

        } else {
            //  display pitch then depth
            char display_string[7];
            unsigned char dot_placement = 0;

            if (this->display_pitch) {
                display_string[0] = (this->pitch < 0)? '-' : ' ';
                display_string[1] = (int)(abs(this->pitch) / 10) % 10 + 48;
                display_string[2] = (int)abs(this->pitch) % 10 + 48;
                dot_placement |= 0b00100000;
            } else {
                display_string[0] = ' ';
                display_string[1] = ' ';
                display_string[2] = ' ';
            }
            
            if (this->display_depth) {
                display_string[3] = ' ';
                display_string[4] = (int)this->depth % 10 + 48;
                display_string[5] = (int)(this->depth * 10) % 10 + 48;
                dot_placement |= 0b00001000;
            } else {
                display_string[3] = ' ';
                display_string[4] = ' ';
                display_string[5] = ' ';
            }
            
            display_string[6] = '\0';

            //  0b00101000 is the bit mask for the dot placement
            display.showString(display_string, 6U, 0U, dot_placement);
        }
        
        this->should_refresh = false;
    }
}
