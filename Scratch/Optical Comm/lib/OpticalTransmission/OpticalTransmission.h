#ifndef ___OPTICAL_TRANSMISSION_H___
#define ___OPTICAL_TRANSMISSION_H___

#include "Arduino.h"
#include "Scheduler.h"
#include "FSMMacroV2.h"
#include "lzhStackQueue.h"

class OptTX: public Scheduler {
public:
    OptTX() = delete;
    OptTX(uint8_t pin_tx_data, uint8_t pin_clk, unsigned long clk_period_ms, int buffer_size, int stopping_bit_count);

    void sent_data(byte b);
private:
    enum OPT_TX_STATE {
        OTS_INIT, 
        OTS_IDLE_ON, OTS_IDLE_OFF, 
        OTS_TX_ON, OTS_TX_OFF,
        OTS_STOP_ON, OTS_STOP_OFF
    };

    const uint8_t pin_tx_data;
    const uint8_t pin_clk;

    LZHStackQueue<byte> buffer;
    int stopping_bit_count;

    int bit_counter; 
    int stopping_bit_counter;

    CREATE_FSM_NO_SLEEP_FUNC(OPT_TX, OTS_INIT);

    void event() override;
};

class OptRX: public SchedulerFSM {
public:
    OptRX() = delete;
    OptRX(uint8_t pin_rx_data, uint8_t pin_clk, unsigned long sampling_period_ms, int buffer_size);

    byte read_data();
private:
    const uint8_t pin_rx_data;
    const uint8_t pin_clk;

    LZHStackQueue<byte> buffer;
    LZHStackQueue<uint8_t> clk_samples;
    LZHStackQueue<uint8_t> data_samples;
};


#endif  //  ___OPTICAL_TRANSMISSION_H___