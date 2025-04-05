#ifndef ___SEG_DISPLAY_H___
#define ___SEG_DISPLAY_H___

#include "Scheduler.h"
#include "TM1637TinyDisplay6.h"

class SegDisplay: public Scheduler {
public:
    SegDisplay(int pin_clk, int pin_dio);

    void set_depth(float depth);
    void set_pitch(float pitch);

private:
    TM1637TinyDisplay6 display;

    float depth; 
    float pitch;

    bool require_refresh;

    void event();
};

#endif  //  ___SEG_DISPLAY_H___