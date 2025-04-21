#include "main.h"

#if defined(COMM_RX)
//  +-------------------------------------------------------------------------+
//  |                        Communication -- Receiver                        |
//  +-------------------------------------------------------------------------+

void setup() {
    Serial.begin(9600);
}

void loop() {
    opt_rx.update();
    if (opt_rx.is_avaliable()) {
        Serial.print("Data Received: ");
        Serial.println(opt_rx.read_data());
    }
}



#elif defined(COMM_TX)
//  +-------------------------------------------------------------------------+
//  |                       Communication -- Transmitter                      |
//  +-------------------------------------------------------------------------+

void setup() {
    Serial.begin(9600);
    counter = 0;
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
        opt_tx.sent_data(counter++);
        opt_tx.sent_data(counter++);
        opt_tx.sent_data(counter++);
        SLEEP(5000);
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


