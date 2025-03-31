#include "OpticalTransmission.h"

OptTX::OptTX(uint8_t pin_tx_data, uint8_t pin_clk, unsigned long clk_period_ms, int buffer_size, int stopping_bit_count)
    : Scheduler(clk_period_ms * 500)
    , pin_tx_data(pin_tx_data), pin_clk(pin_clk), buffer(buffer_size)
    , stopping_bit_count(stopping_bit_count),  bit_counter(0), stopping_bit_counter(0) {
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
