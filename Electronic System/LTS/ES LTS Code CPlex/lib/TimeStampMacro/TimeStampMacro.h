//  +-------------------------------------------------------------------------+
//  | This is the library that help record time in Arduino Envrionment.       |
//  | Additionally, users can register time-sensitive event and automatically |
//  | invoke them when checking status.                                       |
//  |                                                                         |
//  | Users can set up many interesting behavior like periodic timer using    |
//  | this macro library.                                                     |
//  |                                                                         |
//  | Author:       Zihui(Andy) Liu                                           |
//  | Email:        liuzihui@uw.edu, salzhx@gmail.com                         |
//  | Last Update:  Jan 4, 2021                                               |
//  |                                                                         |
//  |                                    Check README.md for more information |
//  +-------------------------------------------------------------------------+

#ifndef __TIME_STAMP_MACRO_H__
#define __TIME_STAMP_MACRO_H__

#include "Arduino.h"

//  Interfaces
//  
//  TS_CREATE(tsName, duration)
//  TS_CREATE_W_EVENT(tsName, duration)
//
//  TS_REFRESH(tsName)
//  TS_IS_DUE_PASSED(tsName)
//  TS_SET_DURATION(tsName, duration)
//
//  TS_DUE_EVENT(tsName)
//  TS_DUE_EVENT_DECLARATION(tsName)
//  TS_DUE_EVENT_DEFINITION(tsName, preDueEvent, postDueEvent)
//  TS_NO_OP
//  TS_PRE_DUE_EVENT(tsName)
//  TS_POST_DUE_EVENT(tsName)


//  ---------- Helper Macros ---------- //
#define TS_TIME_STAMP_VARIABLE_TYPE unsigned long

#define TS_DUE_EVENT_DECLARATION(tsName) \
void TS_ ## tsName ## _preDueEvent(); \
void TS_ ## tsName ## _postDueEvent();

//  ---------- TS Initialization ---------- //
#define TS_CREATE(tsName, duration) \
TS_TIME_STAMP_VARIABLE_TYPE TS_ ## tsName ## _timeStamp = millis(); \
TS_TIME_STAMP_VARIABLE_TYPE TS_ ## tsName ## _duration = duration;

#define TS_CREATE_W_EVENT(tsName, duration) \
TS_CREATE(tsName, duration) \
TS_DUE_EVENT_DECLARATION(tsName)

//  ---------- TS Control ---------- //
#define TS_REFRESH(tsName) TS_ ## tsName ## _timeStamp = millis();

#define TS_IS_DUE_PASSED(tsName) \
(millis() - TS_ ## tsName ## _timeStamp  > TS_ ## tsName ## _duration)

#define TS_SET_DURATION(tsName, duration) TS_ ## tsName ## _duration = duration;

#define TS_TIME_ELAPSED_MS(tsName) (millis() - (TS_ ## tsName ## _timeStamp))

//  ---------- TS Event ---------- //
#define TS_DUE_EVENT_DEFINITION(tsName, preDueEvent, postDueEvent) \
void TS_ ## tsName ## _preDueEvent() { preDueEvent; } \
void TS_ ## tsName ## _postDueEvent() { postDueEvent; }

#define TS_NO_OP {}

#define TS_PRE_DUE_EVENT(tsName) TS_ ## tsName ## _preDueEvent();
#define TS_POST_DUE_EVENT(tsName) TS_ ## tsName ## _postDueEvent();

#define TS_DUE_EVENT(tsName) \
{ if (TS_IS_DUE_PASSED(tsName)) { TS_POST_DUE_EVENT(tsName) } \
else { TS_PRE_DUE_EVENT(tsName) } }

#endif  //  __TIME_STAMP_MACRO_H__