#ifndef _HEADER_
#define _HEADER_
#include <Arduino.h>
//this scope is used to declare the global variant
/////////////////////////////////////macro/////////////////////////////////////
//pin macro

////////////////////////////////////variable//////////////////////////////////

extern double Temperature;
extern int Rotate_Speed;
extern double Humidity;
extern bool Is_People;
extern bool Is_Pump;
extern int Light_Intensive;
//RGB light
extern int Light_Red;
extern int Light_Green;
extern int Light_Blue;

//the param next is only used in local
extern bool Is_Param_Change; //denote if the params are changed, true after change the params

////////////////////////////////////function///////////////////////////////////

/**
 * @brief init all the pin mode, should be called once the device is started
 * 
 */
void PinInit();


#endif