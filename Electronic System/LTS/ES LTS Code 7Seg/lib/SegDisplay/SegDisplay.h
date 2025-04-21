#ifndef ___SEG_DISPLAY_H___
#define ___SEG_DISPLAY_H___

#include "Arduino.h"
#include "Scheduler.h"
#include "TM1637TinyDisplay6.h"

enum SegDisplayBlinkState {
    SDBS_ON, SDBS_BLINK_SLOW, SDBS_BLINK_FAST
};

class SegDisplay: public Scheduler {
public:
    SegDisplay(int pin_clk, int pin_dio);

    void begin();

    void set_static_text(const char * static_text);
    void set_static_text_blink_mode(SegDisplayBlinkState mode);

    void set_depth(float depth);
    void set_depth_blink_mode(SegDisplayBlinkState mode);

    void set_pitch(float pitch);
    void set_pitch_blink_mode(SegDisplayBlinkState mode);


    void set_to_static_text_mode();
    void set_to_sensor_info_mode();

    TM1637TinyDisplay6 *get_display();

private:
    TM1637TinyDisplay6 display;

    bool should_refresh; 

    bool fast_flash_state;
    unsigned long fast_flash_rate_ms;
    unsigned long fast_flash_next_update_time_ms;

    bool slow_flash_state;
    unsigned long slow_flash_rate_ms;
    unsigned long slow_flash_next_update_time_ms;

    bool is_display_static_text;

    char static_text[7];
    bool display_static_text;
    SegDisplayBlinkState static_text_display_mode;

    float depth; 
    bool display_depth;
    SegDisplayBlinkState depth_display_mode;
    
    float pitch;
    bool display_pitch; 
    SegDisplayBlinkState pitch_display_mode;

    void event();
    void refresh();
};

#endif  //  ___SEG_DISPLAY_H___