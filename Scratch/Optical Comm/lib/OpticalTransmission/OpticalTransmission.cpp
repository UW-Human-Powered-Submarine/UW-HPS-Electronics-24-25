#include "OpticalTransmission.h"

//  +-------------------------------------------------------------------------+
//  |                       Communication -- Transmitter                      |
//  |                                   OptTX                                 |
//  +-------------------------------------------------------------------------+

OptTX::OptTX(uint8_t pin_tx_data, uint8_t pin_clk, unsigned long clk_period_ms, int buffer_size, int stopping_bit_count)
    : Scheduler(clk_period_ms * 500)
    , pin_tx_data(pin_tx_data), pin_clk(pin_clk), buffer(buffer_size)
    , stopping_bit_count(stopping_bit_count),  bit_counter(0), stopping_bit_counter(0) {
}

OptTX::~OptTX() {
}

void OptTX::sent_data(byte b) {
    this->buffer.push_back(b);
}

void OptTX::event() {
    SETUP_FSM_FUNCTION_NO_SLEEP_FUNC(OPT_TX);

    STATE(OTS_INIT) {
        pinMode(this->pin_tx_data, OUTPUT);
        pinMode(this->pin_clk, OUTPUT);

        digitalWrite(this->pin_tx_data, HIGH);
        digitalWrite(this->pin_clk, LOW);

        TO(OTS_IDLE_OFF);
    }

    STATE(OTS_IDLE_ON) {
        digitalWrite(this->pin_clk, LOW);

        TO(OTS_IDLE_OFF);
    }

    STATE(OTS_IDLE_OFF) {
        digitalWrite(this->pin_clk, HIGH);

        if (buffer.is_empty()) {
            TO(OTS_IDLE_ON);
        } else {
            digitalWrite(this->pin_tx_data, LOW);
            this->bit_counter = 7;

            TO(OTS_TX_ON);
        }
    }

    STATE(OTS_TX_ON) {
        digitalWrite(this->pin_clk, LOW);

        TO(OTS_TX_OFF);
    }


    STATE(OTS_TX_OFF) {
        digitalWrite(this->pin_clk, HIGH);
        digitalWrite(
            this->pin_tx_data, 
            ((this->buffer.peek_front() >> this->bit_counter) & 1) == 1
        );

        if (this->bit_counter == 0) {
            this->buffer.pop_front();
            this->stopping_bit_counter = 2;
            TO(OTS_STOP_ON);
        } else {
            this->bit_counter--;
            TO(OTS_TX_ON);
        }
    }

    STATE(OTS_STOP_ON) {
        digitalWrite(this->pin_clk, LOW);

        TO(OTS_STOP_OFF);
    }

    STATE(OTS_STOP_OFF) {
        digitalWrite(this->pin_clk, HIGH);
        digitalWrite(this->pin_tx_data, HIGH);

        if (this->stopping_bit_counter == 0) {
            TO(OTS_IDLE_ON);
        } else {
            stopping_bit_counter--;
            TO(OTS_STOP_ON);
        }
    }
}

//  +-------------------------------------------------------------------------+
//  |                        Communication -- Receiver                        |
//  |                                   OptRX                                 |
//  +-------------------------------------------------------------------------+

OptRX::OptRX(uint8_t pin_rx_data_analog, uint8_t pin_clk_analog, unsigned long clk_period_ms, int buffer_size)
    : Scheduler(clk_period_ms * 1000 / OPT_RX_SAMPLES_PER_PERIOD)
    , pin_rx_data(pin_rx_data_analog), pin_clk(pin_clk_analog)
    , buffer(buffer_size), clk_samples(OPT_RX_DATA_SAMPLING_SIZE), data_samples(OPT_RX_DATA_SAMPLING_SIZE)
    , bit_counter(0), temp_bit(0) {
}

OptRX::~OptRX() {
}

byte OptRX::read_data() {
    return buffer.pop_front();
}

bool OptRX::is_avaliable() const {
    return !buffer.is_empty();
}

void OptRX::event() {
    SETUP_FSM_FUNCTION_NO_SLEEP_FUNC(OPT_RX);

    this->update_clk_data_sample();
    // Serial.println(GET_STATE(OPT_RX));

    // // Serial.println(clk_samples.size());

    // // Serial.print(clk_samples.max_val());
    // // Serial.print(", ");
    // // Serial.println(clk_samples.min_val());

    STATE(ORX_IDLE_ON) {
        if (this->is_clk_disconnected()) {
            TO(ORX_DISCONNECT);
        }

        if (!this->is_clk_high()) {
            if (this->is_data_high()) {
                TO(ORX_IDLE_OFF);
            } else {
                bit_counter = 0;
                temp_bit = 0;
                TO(ORX_READING_OFF);
            }
        }
    }

    STATE(ORX_IDLE_OFF) {
        if (this->is_clk_disconnected()) {
            TO(ORX_DISCONNECT);
        }

        if (this->is_clk_high()) {
            TO(ORX_IDLE_ON)
        }
    }

    STATE(ORX_READING_ON) {
        if (this->is_clk_disconnected()) {
            TO(ORX_DISCONNECT);
        }

        if (!this->is_clk_high()) {
            if (bit_counter < 8) {
                temp_bit = (temp_bit << 1)+ (uint8_t) this->is_data_high();
                bit_counter++;
                TO(ORX_READING_OFF);
    
            } else {
                TO(ORX_FINISHED);
            }
        }
    }

    STATE(ORX_READING_OFF) {
        if (this->is_clk_disconnected()) {
            TO(ORX_DISCONNECT);
        }

        if (this->is_clk_high()) {
            TO(ORX_READING_ON);
        }
    }

    STATE(ORX_FINISHED) {
        this->buffer.push_back(temp_bit);

        if (this->is_clk_disconnected()) {
            TO(ORX_DISCONNECT);
        }
        if (this->is_clk_high()) {
            TO(ORX_IDLE_ON);
        } else {
            TO(ORX_IDLE_OFF);
        }
    }

    STATE(ORX_DISCONNECT) {
        if (!this->is_clk_disconnected()) {
            if (this->is_clk_high()) {
                TO(ORX_IDLE_ON);
            } else {
                TO(ORX_IDLE_OFF);
            }
        }
    }
}

void OptRX::update_clk_data_sample() {
    if (this->clk_samples.is_full()) {
        this->clk_samples.pop_front();
    }
    this->clk_samples.push_back(analogRead(this->pin_clk));

    if (this->data_samples.is_full()) {
        this->data_samples.pop_front();
    }
    this->data_samples.push_back(analogRead(this->pin_rx_data));
}

bool OptRX::is_clk_high() const {
    unsigned char min_val = this->clk_samples.min_val();
    unsigned char max_val = this->clk_samples.max_val();

    if (max_val - min_val <= OPT_RX_CLK_DISCONNECTION_THRESHOLD) {
        return false;
    } else {
        unsigned char threshold = (min_val + max_val) / 2;
        return (this->clk_samples.peek_back() < threshold) == OPT_RX_REVERSE_CLK_LOGIC;
    }
}

bool OptRX::is_clk_disconnected() const {
    return this->clk_samples.max_val() - this->clk_samples.min_val() 
        <= OPT_RX_CLK_DISCONNECTION_THRESHOLD;
}

bool OptRX::is_data_high() const {
    unsigned char min_val = this->data_samples.min_val();
    unsigned char max_val = this->data_samples.max_val();

    if (max_val - min_val <= OPT_RX_DATA_DISCONNECTION_THRESHOLD) {
        return true;
    } else {
        unsigned char threshold = (min_val + max_val) / 2;
        return (this->data_samples.peek_back() < threshold) == OPT_RX_REVERSE_DATA_LOGIC;
    }
}
