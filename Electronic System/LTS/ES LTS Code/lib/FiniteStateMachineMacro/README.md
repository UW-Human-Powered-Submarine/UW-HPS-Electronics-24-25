# Finite State Machine Macro
Author: Zihui(Andy) Liu <liuzihui@uw.edu, salzhx@gmail.com>  
Last Update: Jan 3, 2021
***

## Overview
Macro version of the FSM lib.

This is the library that helps the development of Finite State Machine (FSM). It handles many details of FSM, state variable, and transition, so the user can focus on its content. 

***
## Interfaces
### FSM Initialization
- `FSM_CREATE(fsmName, initialState)`
  - Initialize the FSM.
  - Create necessary variables:
    - `FSM_fsmName_state` is created with initial value `initialState`. This variable stores the current state of the FSM.
    - `FSM_fsmName_initialState` is created to store the starting state of the FSM. This part is vital when using the reset function. 
  - Declare relevant functions:
    - `FSM_fsmName_tick()` is created. 
    - `FSM_fsmName_reset()` is created. 
- `FSM_CREATE_MINIMAL(fsmName, initialState)`
  - Initialize the FSM with minimal resources. 
  - Reset function is omitted
    - Omit `FSM_fsmName_initialState` variable and `fsmName_reset()` function. 

### FSM Control
- `FSM_TICK(fsmName)`
  - Advance the given FSM by one step. 
  - Invoke this tick function periodically
    - Inside `while(true){}` loop for general code.
    - Inside `void loop()` if it is in the Arduino environment. 
- `FSM_RESET(fsmName)`
  - Invoke `FSM_fsmName_reset()`, which reset the FSM state. 
  - Warning: This function does not work when it resets itself inside the tick function (`FSM_TICK_FUNCTION`). Use `FSM_REBOOT(fsmName)` to achieve the behavior above.
- `FSM_REBOOT(fsmName)`
  - Reset the FSM state. This function can only be used inside `FSM_TICK_FUNCTION`, and its purpose is to RESET ITSELF. 
  - Warning: Using `FSM_REBOOT(fsmName)` outside tick function will result in unexpected behavior. 


### FSM Definitaiton
- `FSM_TICK_FUNCTION(fsmName, relations)`
  - Define the basic structure for the FSM tick function. 
  - `relations` should contain all FSM operations:
    - `FSM_TRANSITION`, `FSM_MOORE_EVENT`, and `FSM_MEALY_EVENT`
- `FSM_TRANSITION(originState, targetState, transitionExpr)`
  - Define a transition in FSM. 
  - The FSM state transfer from `originState` to `targetState` when `transitionExpr` is true. 
  - `transitionExpr` is equivlant to the input of the FSM. 
- `FSM_MOORE_EVENT(state, event)`
  - Define a Moore-type event.
  - The output of FSM depends on the current state. a.k.a. the event will be executed when the current state is the same as the given state.  
  - `event` should contains regular C/C++ code. 
- `FSM_MEALY_EVENT(originState, transitionExpr, event)`
  - Define a Mealy-type event
  - The output of FSM depends on the current state and input. a.k.a. the event will be executed when the current state is the same as the given state and `transitionExpr` is true.  
  - `event` should contains regular C/C++ code. 
## Scheduler
The scheduler has several limitations and cannot replace time-sensitive functions. 
### FSM Scheduler Declaration
- `FSM_CREATE_W_SCHEDULER(fsmName, initialState)`
  - Initialize the FSM with the scheduler.
  - Same as `FSM_CREATE(fsmName, initialState)`
    - Create necessary variables:
      - `FSM_fsmName_state` is created with initial value `initialState`. This variable stores the current state of the FSM.
      - `FSM_fsmName_initialState` is created to store the starting state of the FSM. This part is vital when using the reset function. 
    - Declare relevant functions:
      - `FSM_fsmName_tick()` is created. 
      - `FSM_fsmName_reset()` is created. 
  - Additional Variable Created
    - `FSM_fsmName_resumeTime` stores the return time of the FSM
- `FSM_TICK_FUNCTION_W_SCHEDULER(fsmName, relations)`
  - Define the basic structure for the FSM tick function with the scheduler.
  - Same as `FSM_TICK_FUNCTION(fsmName, relations)`
    - `relations` should contain all FSM operations:
      - `FSM_TRANSITION`, `FSM_MOORE_EVENT`, and `FSM_MEALY_EVENT`

### FSM Scheduler Transitions
All functions must be used inside Moore event `FSM_MOORE_EVENT(state, event)`.
- `FSM_SCHDR_SLEEP(delayTime_ms)`
  - FSM will sleep for given time `delayTime_ms`, and resume back to same state.
- `FSM_SCHDR_SLEEP_UNTIL(delayTime_ms)` 
  - FSM will wait until given time `delayTime_ms`, and resume back to same state.
- `FSM_SCHDR_SLEEP_TRSN(delayTime_ms, targetState)` 
  - FSM will sleep for given time `delayTime_ms`, and resume back to `targetState`.
- `FSM_SCHDR_SLEEP_UNTIL_TRSN(delayTime_ms, targetState)` 
  - FSM will wait until given time `delayTime_ms`, and resume back to `targetState`.
- `FSM_SCHDR_SLEEP_TRSN_NEXT(delayTime_ms)` 
  - FSM will sleep for given time `delayTime_ms`, and resume back to next state, e.g. 0 -> 1, 4 -> 5.
- `FSM_SCHDR_SLEEP_UNTIL_TRSN_NEXT(delayTime_ms)`
  - FSM will wait until given time `delayTime_ms`, and resume back to next state.

***
## Example 1
The following example is an Arduino code of toggle button. 
- When the button is pressed (`digitalRead(12) == 0`), the onboard LED (pin 13) turns on, and pressing the button again turns off the onboard LED.

``` C++
#include "FiniteStateMachineMacro.h"

//  create FSM named fsm1 with initial state 0
FSM_CREATE(fsm1, 0)

void setup() {
  pinMode(13, OUTPUT);        // onboard LED
  pinMode(12, INPUT_PULLUP);  // input button
}

void loop() {
  FSM_TICK(fsm1)
  delay(100);  // "clock speed"
}

//  Input checks
//  Button pressed
#define A (digitalRead(12) == 0)

//  Button released
#define A_ (digitalRead(12) != 0)

//  State code:
//  0:  Initial State
//  1:  LED off, button released
//  10: LED off, button pressed (intermediate state)
//  2:  LED on,  button released
//  20: LED on,  button pressed (intermediate state)

//  User can use enum to improve readability

FSM_TICK_FUNCTION(fsm1, 
  //  Transitions
  FSM_TRANSITION(0, 1, true)
  FSM_TRANSITION(1, 10, A)
  FSM_TRANSITION(10, 2, A_)
  FSM_TRANSITION(2, 20, A)
  FSM_TRANSITION(20, 1, A_)

  //  INIT
  FSM_MOORE_EVENT(0, 
    //  Must include semicolon when writing events
    //  Event can be more than one line
    digitalWrite(13, LOW);  
  )

  //  LED_ON
  FSM_MEALY_EVENT(10, A_, 
    digitalWrite(13, HIGH);
  )

  //  LED_OFF
  FSM_MEALY_EVENT(20, A_, 
    digitalWrite(13, LOW);
  )
)
```

## Example 2
The following example is the default Arduino blinking example implemented using FSM Scheduler.
``` C++
#include "FiniteStateMachineMacro.h"

#define PIN_LED_BUILTIN 13

FSM_CREATE_W_SCHEDULER(blink, 0)

void setup() {
  pinMode(PIN_LED_BUILTIN, OUTPUT);
}

void loop() {
  FSM_TICK(blink)
  delay(10);
}

FSM_TICK_FUNCTION_W_SCHEDULER(blink, 
  FSM_MOORE_EVENT(0, 
    digitalWrite(PIN_LED_BUILTIN, HIGH);
    FSM_SCHDR_SLEEP_TRNS_NEXT(1000)
  )
  FSM_MOORE_EVENT(1, 
    digitalWrite(PIN_LED_BUILTIN, LOW);
    FSM_SCHDR_SLEEP_TRNS(1000, 0)
  )
)
```

***
## Note
- `FSM_STATE_VARIABLE_TYPE` defines the type of state variable (default: `unsigned char`, 1 byte). User is able to overwrite the type to increase the total number of states by following code: 
``` C++
/* All #include header files */
/* redefine the FSM state variable type right after #include */
#undef FSM_STATE_VARIABLE_TYPE 
#define FSM_STATE_VARIABLE_TYPE int
/* Other codes */
```
- Inside `FSM_TICK_FUNCTION`, the following variables are reserved. Users should NEVER create local variables with the same names. 
  - `__ps`: Present State.
  - `__ns`: Next State.
  - `__resumeTime`: The time when current FSM resume operation/

***
## Dependency
- The library is developed with the C++11 preprocessor and tested using the C++ compiler. However, it *should* be compatible with C (NOT TESTED). 

***
## Update History
- Oct 2, 2021
  - Added basic functionalities of Finite State Machine.
- Jan 3, 2022
  - Added scheduler.