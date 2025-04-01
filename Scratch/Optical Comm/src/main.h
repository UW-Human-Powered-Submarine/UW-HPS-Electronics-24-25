#ifndef ___MAIN_H___
#define ___MAIN_H___

#define COMM_RX
// #define COMM_TX
// #define QUEUE_TEST

#if defined(COMM_RX)
//  +-------------------------------------------------------------------------+
//  |                        Communication -- Receiver                        |
//  +-------------------------------------------------------------------------+
#include <Arduino.h>
#include "FSMMacroV2.h"
#include "TimeStampMacro.h"

#include "OpticalTransmission.h"

#define PIN_CLK A2
#define PIN_RX  A3

OptRX opt_rx(PIN_RX, PIN_CLK, 100, 5);


#elif defined(COMM_TX)
//  +-------------------------------------------------------------------------+
//  |                       Communication -- Transmitter                      |
//  +-------------------------------------------------------------------------+
#include <Arduino.h>
#include "FSMMacroV2.h"
#include "TimeStampMacro.h"

#include "OpticalTransmission.h"

#define PIN_CLK 9
#define PIN_TX  10

OptTX opt_tx(PIN_TX, PIN_CLK, 100, 5, 4);

CREATE_FSM(DataFeeder, 0);
void data_feeder_update();

byte counter; 

#elif defined(QUEUE_TEST)
//  +-------------------------------------------------------------------------+
//  |                                Queue Test                               |
//  +-------------------------------------------------------------------------+
#include <Arduino.h>
#include "lzhStackQueue.h"

#endif


#endif  //  ___MAIN_H___