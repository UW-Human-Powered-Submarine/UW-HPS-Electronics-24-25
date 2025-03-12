//  +---------------------------------------------------------------------------------------------+
//  |   This is the library that helps the development of the finite state machine. It handles    |
//  |       many details of the FSM, state variable and transition, so the user can focus on its  |
//  |       content.                                                                              |
//  |   This is a simplification from FiniteStateMachineMacro.h based on my (Andy's) experiences  |
//  |       of using the macro class. The resulting code is more natural to C++ code, but it      |
//  |       doesn't enforce the rigorous FSM structure.                                           |
//  |                                                                                             |
//  |   Author:       Zihui(Andy) Liu                                                             |
//  |   Email:        liuzihui@uw.edu, salzhx@gmail.com                                           |
//  |   Last Update:  March 9, 2025                                                               |
//  |                                                                                             |
//  |                                                        Check README.md for more information |
//  +---------------------------------------------------------------------------------------------+

#ifndef __FINITE_STATE_MACHINE_MACRO_V2_H__
#define __FINITE_STATE_MACHINE_MACRO_V2_H__

#define FSM_STATE_VARIABLE_TYPE unsigned int

#define CREATE_FSM(fsm_name, init_state) \
FSM_STATE_VARIABLE_TYPE FSM_ ## fsm_name ## _state = init_state; \
const FSM_STATE_VARIABLE_TYPE FSM_ ## fsm_name ## _init_state = init_state;  \
unsigned long FSM_ ## fsm_name ## _resume_time = 0;

#define RESET_FSM(fsm_name) { FSM_ ## fsm_name ## _state = FSM_ ## fsm_name ## _init_state; }

#define SETUP_FSM(fsm_name) \
if (millis() < FSM_ ## fsm_name ## _resume_time) { return; } \
FSM_STATE_VARIABLE_TYPE __current_state = FSM_ ## fsm_name ## _state; \
FSM_STATE_VARIABLE_TYPE *__global_state = &FSM_ ## fsm_name ## _state;  \
unsigned long *__resume_time = &FSM_ ## fsm_name ## _resume_time;

#define STATE(state) if (__current_state == state) 

#define WHEN(bool_expr) if (bool_expr)

#define FROM(state) if (__current_state == state) 

#define TO(state) { *__global_state = state; return; }
#define TO_NEXT { ++(*__global_state); return; }

#define SLEEP(delay_time_ms) { *__resume_time = millis() + delay_time_ms; return; }

#define SLEEP_TO(delay_time_ms, state) \
{ *__resume_time = millis() + delay_time_ms; *__global_state = state; return; }

#define SLEEP_TO_NEXT(delay_time_ms) \
{ *__resume_time = millis() + delay_time_ms; ++(*__global_state); return; }

#define GET_STATE(fsm_name) FSM_ ## fsm_name ## _state


#endif // __FINITE_STATE_MACHINE_MACRO_V2_H__