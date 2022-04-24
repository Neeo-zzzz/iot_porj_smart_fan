#include "header.h"
double Temperature;
int Rotate_Speed;
double Humidity;
bool Is_People;
bool Is_Pump;
int Light_Intensive;
//RGB light
int Light_Red;
int Light_Green;
int Light_Blue;
bool Is_Param_Change; //denote if the params are changed, true after change the params

bool Is_Fan_Humidifier_Smart;
bool Is_Fan_Safe;
bool Is_Light_Smart;
double Temp_Threshold;
double Humidity_Threshold;
int Light_Threshold;

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

    Is_Fan_Humidifier_Smart = false;
    Is_Fan_Safe = true;
    Is_Light_Smart = false;
    Temp_Threshold = 20.0;
    Humidity_Threshold = 70.0;
    Light_Threshold = 300;
}