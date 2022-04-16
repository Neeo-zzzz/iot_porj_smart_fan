#include "header.h"

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
bool Is_Param_Change; //denote if the params are changed, true after change the params

void PinInit()
{
    //todo 
    Is_Param_Change = false;

    Light_Red = 0;
    Light_Blue = 0;
    Light_Green = 0;

    Is_People = false;
    Is_Pump = false;
    Rotate_Speed = 0;
}