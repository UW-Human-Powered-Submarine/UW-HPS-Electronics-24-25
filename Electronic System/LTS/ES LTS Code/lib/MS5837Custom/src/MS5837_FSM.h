#ifndef ___MS5837_FSM_H___
#define ___MS5837_FSM_H___

#include "Arduino.h"
#include <Wire.h>
#include "Scheduler.h"
#include "MS5837.h"
#include "FSMMacroV2.h"

//  implement MS5837::read() method in fsm

class MS5837_FSM: public MS5837, public SchedulerFSM {
public:
    MS5837_FSM();
    ~MS5837_FSM();

private:
    CREATE_FSM(MS5837_FSM, 0);

    void event() override;
};


#endif  //  ___MS5837_FSM_H___