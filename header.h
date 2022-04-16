#ifndef _HEADER_
#define _HEADER_
#include <Arduino.h>
//this scope is used to declare the global variant
/////////////////////////////////////macro/////////////////////////////////////
//pin macro

////////////////////////////////////variable//////////////////////////////////

double Temperature;
int Rotate_Speed;
double Humidity;
bool Is_People;
bool Is_Pump;
double Light_Intensive;
//RGB light
int Light_Red;
int Light_Green;
int Light_Blue;

////////////////////////////////////function///////////////////////////////////

/**
 * @brief init all the pin mode, should be called once the device is started
 * 
 */
void PinInit();


#endif