#include "main.h"

#if defined(COMM_RX)
//  +-------------------------------------------------------------------------+
//  |                        Communication -- Receiver                        |
//  +-------------------------------------------------------------------------+

void setup() {
    Serial.begin(9600);
}

void loop() {
    Serial.print("CLK:");
    Serial.print(analogRead(PIN_CLK));
    Serial.print(", D:");
    Serial.print(analogRead(PIN_RX));
    Serial.println();
    delay(200);
}



#elif defined(COMM_TX)
//  +-------------------------------------------------------------------------+
//  |                       Communication -- Transmitter                      |
//  +-------------------------------------------------------------------------+

void setup() {
    Serial.begin(9600);
}

void loop() {
    opt_tx.update();
    data_feeder_update();
}

void data_feeder_update() {
    SETUP_FSM_FUNCTION(DataFeeder)

    STATE(0) {
        SLEEP_TO_NEXT(2000);
    }

    STATE(1) {
        opt_tx.sent_data(11);
        SLEEP(15000);
    }
}

#elif defined(QUEUE_TEST)
//  +-------------------------------------------------------------------------+
//  |                                Queue Test                               |
//  +-------------------------------------------------------------------------+

void setup() {
    Serial.begin(9600);
}

void loop() {
    LZHStackQueue<int> stqu(5);

    for(int i = 0; i < 10; i++) {
        stqu.push_back(i);
        Serial.print(stqu.size());
        Serial.println(stqu.peek_back());
        delay(500);
    }

    for(int i = 0; i < 10; i++) {
        Serial.println(stqu.pop_front());
        delay(500);
    }

    for(int i = 0; i < 10; i++) {
        stqu.push_front(i);
        Serial.println(stqu.peek_front());
        delay(500);
    }

    for(int i = 0; i < 10; i++) {
        Serial.println(stqu.pop_back());
        delay(500);
    }
}


#endif


