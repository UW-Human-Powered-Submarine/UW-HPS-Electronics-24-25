#include "MS5837_FSM.h"

const uint8_t MS5837_ADDR = 0x76;
const uint8_t MS5837_RESET = 0x1E;
const uint8_t MS5837_ADC_READ = 0x00;
const uint8_t MS5837_PROM_READ = 0xA0;
const uint8_t MS5837_CONVERT_D1_8192 = 0x4A;
const uint8_t MS5837_CONVERT_D2_8192 = 0x5A;

MS5837_FSM::MS5837_FSM()
    : MS5837(), SchedulerFSM(100000) {
}

MS5837_FSM::~MS5837_FSM() {
}

void MS5837_FSM::event() {
    SETUP_FSM_FUNCTION(MS5837_FSM) 

    STATE(0) {
        //Check that _i2cPort is not NULL (i.e. has the user forgoten to call .init or .begin?)
        if (_i2cPort == NULL) {
            this->fsm_finished();   //  end the sequence
            TO(0);  //  reset the fsm
        }

        // Request D1 conversion
        _i2cPort->beginTransmission(MS5837_ADDR);
        _i2cPort->write(MS5837_CONVERT_D1_8192);
        _i2cPort->endTransmission();

        SLEEP_TO_NEXT(20); // Max conversion time per datasheet
    }

    STATE(1) {
        _i2cPort->beginTransmission(MS5837_ADDR);
        _i2cPort->write(MS5837_ADC_READ);
        _i2cPort->endTransmission();

        _i2cPort->requestFrom(MS5837_ADDR, (uint8_t)3);
        D1_pres = 0;
        D1_pres = _i2cPort->read();
        D1_pres = (D1_pres << 8) | _i2cPort->read();
        D1_pres = (D1_pres << 8) | _i2cPort->read();

        // Request D2 conversion
        _i2cPort->beginTransmission(MS5837_ADDR);
        _i2cPort->write(MS5837_CONVERT_D2_8192);
        _i2cPort->endTransmission();

        SLEEP_TO_NEXT(20); // Max conversion time per datasheet
    }

	STATE(2) {
        _i2cPort->beginTransmission(MS5837_ADDR);
        _i2cPort->write(MS5837_ADC_READ);
        _i2cPort->endTransmission();

        _i2cPort->requestFrom(MS5837_ADDR,(uint8_t)3);
        D2_temp = 0;
        D2_temp = _i2cPort->read();
        D2_temp = (D2_temp << 8) | _i2cPort->read();
        D2_temp = (D2_temp << 8) | _i2cPort->read();

        calculate();

        this->fsm_finished();   //  end the sequence
        TO(0);  //  reset the fsm
    }
}
