//  +-------------------------------------------------------------------------+
//  | This is the library that helps the development of the finite state      |
//  | machine. It handles many details of the FSM, state variable and         |
//  | transition, so the user can focus on its content.                       |
//  |                                                                         |
//  | Author:       Zihui(Andy) Liu                                           |
//  | Email:        liuzihui@uw.edu, salzhx@gmail.com                         |
//  | Last Update:  Jan 3, 2022                                               |
//  |                                                                         |
//  |                                    Check README.md for more information |
//  +-------------------------------------------------------------------------+

#ifndef __FINITE_STATE_MACHINE_MACRO_H__
#define __FINITE_STATE_MACHINE_MACRO_H__

//  Interfaces
//  
//  FSM_CREATE(fsmName, initialState)
//  FSM_CREATE_MINIMAL(fsmName, initialState)
//
//  FSM_TICK(fsmName) 
//  FSM_RESET(fsmName)
//  FSM_REBOOT(fsmName)
//
//  FSM_TICK_FUNCTION(fsmName, relations)
//  FSM_TRANSITION(originState, targetState, transitionExpr)
//  FSM_MOORE_EVENT(state, event)
//  FSM_MEALY_EVENT(originState, transitionExpr, event)

//  ---------- Helper Macros ---------- //
#define FSM_STATE_VARIABLE_TYPE unsigned char

#define __CREATE_STATE_VARIABLE(fsmName, initialState) \
FSM_STATE_VARIABLE_TYPE FSM_ ## fsmName ## _state = initialState;

#define __CREATE_RESET_FUNCTION(fsmName, initialState) \
FSM_STATE_VARIABLE_TYPE FSM_ ## fsmName ## _initialState = initialState; \
void FSM_ ## fsmName ## _reset() { FSM_ ## fsmName ## _state = FSM_ ## fsmName ## _initialState; } 

#define __CREATE_SCHEDULER(fsmName) \
unsigned long FSM_ ## fsmName ## _resumeTime = 0;

#define __TICK_FUNCTION_DECLARATION(fsmName) \
void FSM_ ## fsmName ## _tick();


//  ---------- FSM Initialization ---------- //
#define FSM_CREATE(fsmName, initialState) \
__CREATE_STATE_VARIABLE(fsmName, initialState) \
__CREATE_RESET_FUNCTION(fsmName, initialState) \
__TICK_FUNCTION_DECLARATION(fsmName)

#define FSM_CREATE_MINIMAL(fsmName, initialState) \
__CREATE_STATE_VARIABLE(fsmName, initialState) \
__TICK_FUNCTION_DECLARATION(fsmName)

#define FSM_CREATE_W_SCHEDULER(fsmName, initialState) \
__CREATE_STATE_VARIABLE(fsmName, initialState) \
__CREATE_RESET_FUNCTION(fsmName, initialState) \
__CREATE_SCHEDULER(fsmName) \
__TICK_FUNCTION_DECLARATION(fsmName)

//  ---------- FSM Control ---------- //
#define FSM_TICK(fsmName) FSM_ ## fsmName ## _tick();

#define FSM_RESET(fsmName) FSM_ ## fsmName ## _reset();

#define FSM_REBOOT(fsmName) { FSM_RESET(fsmName); return; }


//  ---------- FSM Definitaion ---------- //
#define FSM_TICK_FUNCTION(fsmName, relations) \
void FSM_ ## fsmName ## _tick() { \
  FSM_STATE_VARIABLE_TYPE __ps = FSM_ ## fsmName ## _state; \
  FSM_STATE_VARIABLE_TYPE __ns = FSM_ ## fsmName ## _state; \
  relations; \
  FSM_ ## fsmName ## _state = __ns; \
}

#define FSM_TICK_FUNCTION_W_SCHEDULER(fsmName, relations) \
void FSM_ ## fsmName ## _tick() { \
  if (millis() < FSM_ ## fsmName ## _resumeTime) { return; } \
  FSM_STATE_VARIABLE_TYPE __ps = FSM_ ## fsmName ## _state; \
  FSM_STATE_VARIABLE_TYPE __ns = FSM_ ## fsmName ## _state; \
  unsigned long __resumeTime = 0; \
  relations; \
  FSM_ ## fsmName ## _state = __ns; \
  FSM_ ## fsmName ## _resumeTime = __resumeTime; \
}

#define FSM_TRANSITION(originState, targetState, transitionExpr) \
  if ((__ps == originState) && (transitionExpr)) { __ns = targetState; }

#define FSM_MOORE_EVENT(state, event) \
  if (__ps == state) { event; }

#define FSM_MEALY_EVENT(originState, transitionExpr, event) \
  if ((__ps == originState) && (transitionExpr)) { event; } 

//  ---------- FSM Scheduler Function ---------- // 
#define FSM_SCHDR_SLEEP(delayTime_ms) \
__resumeTime = millis() + delayTime_ms;

#define FSM_SCHDR_SLEEP_UNTIL(delayTime_ms) \
__resumeTime = delayTime_ms;

#define FSM_SCHDR_SLEEP_TRNS(delayTime_ms, targetState) \
__resumeTime = millis() + delayTime_ms; \
__ns = targetState;

#define FSM_SCHDR_SLEEP_UNTIL_TRNS(delayTime_ms, targetState) \
__resumeTime = delayTime_ms; \
__ns = targetState;

#define FSM_SCHDR_SLEEP_TRNS_NEXT(delayTime_ms) \
__resumeTime = millis() + delayTime_ms; \
++__ns;

#define FSM_SCHDR_SLEEP_UNTIL_TRNS_NEXT(delayTime_ms) \
__resumeTime = delayTime_ms; \
++__ns;

#endif // __FINITE_STATE_MACHINE_MACRO_H__