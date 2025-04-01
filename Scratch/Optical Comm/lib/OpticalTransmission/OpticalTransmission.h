#ifndef ___OPTICAL_TRANSMISSION_H___
#define ___OPTICAL_TRANSMISSION_H___

#include "Arduino.h"
#include "Scheduler.h"
#include "FSMMacroV2.h"
#include "lzhStackQueue.h"
#include "lzhByteSQ.h"

//  +-------------------------------------------------------------------------+
//  |                       Communication -- Transmitter                      |
//  |                                   OptTX                                 |
//  +-------------------------------------------------------------------------+

class OptTX: public Scheduler {
public:
    OptTX() = delete;
    OptTX(uint8_t pin_tx_data, uint8_t pin_clk, unsigned long clk_period_ms, int buffer_size, int stopping_bit_count);
    ~OptTX();

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

//  +-------------------------------------------------------------------------+
//  |                        Communication -- Receiver                        |
//  |                                   OptRX                                 |
//  +-------------------------------------------------------------------------+

#define OPT_RX_SAMPLES_PER_PERIOD 5
#define OPT_RX_DATA_SAMPLING_SIZE (OPT_RX_SAMPLES_PER_PERIOD * 8)

#define OPT_RX_REVERSE_CLK_LOGIC  true
#define OPT_RX_CLK_DISCONNECTION_THRESHOLD  50

#define OPT_RX_REVERSE_DATA_LOGIC true
#define OPT_RX_DATA_DISCONNECTION_THRESHOLD 50

class OptRX: public Scheduler {
public:
    OptRX() = delete;
    OptRX(uint8_t pin_rx_data_analog, uint8_t pin_clk_analog, unsigned long sampling_period_ms, int buffer_size);
    ~OptRX();

    byte read_data();

    bool is_avaliable() const;
private:
    enum OPT_RX_STATE {
        ORX_IDLE_ON, ORX_IDLE_OFF,
        ORX_READING_ON, ORX_READING_OFF,
        ORX_FINISHED, 
        ORX_DISCONNECT
    };
    const uint8_t pin_rx_data;
    const uint8_t pin_clk;

    LZHStackQueue<byte> buffer;
    LZHZByteSQ clk_samples;
    LZHZByteSQ data_samples;

    int bit_counter; 
    uint8_t temp_bit;   //  place to store current reading bit

    CREATE_FSM_NO_SLEEP_FUNC(OPT_RX, ORX_IDLE_ON);

    void event() override;
    void update_clk_data_sample();

    bool is_clk_high() const;
    bool is_clk_disconnected() const;
    bool is_data_high() const;
};


#endif  //  ___OPTICAL_TRANSMISSION_H___