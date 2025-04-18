#ifndef ___SEG_DISPLAY_H___
#define ___SEG_DISPLAY_H___

#include "Scheduler.h"
#include "TM1637TinyDisplay6.h"

class SegDisplay: public Scheduler {
public:
    SegDisplay(int pin_clk, int pin_dio);

    void begin();

    void set_depth(float depth);
    void set_pitch(float pitch);

    TM1637TinyDisplay6 *get_display();

private:
    TM1637TinyDisplay6 display;

    bool should_refresh; 

    float depth; 
    float pitch;

    void event();
    void refresh();
};

#endif  //  ___SEG_DISPLAY_H___