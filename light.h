#ifndef _LIGHT_
#define _LIGHT_
#include <Arduino.h>
#include "header.h"

#define PIN_LIGHT_RED 7
#define PIN_LIGHT_GREEN 8
#define PIN_LIGHT_BLUE 9

class light
{
    public:
    light()
    {
        analogWrite(PIN_LIGHT_RED,Light_Red);
        analogWrite(PIN_LIGHT_GREEN,Light_Green);
        analogWrite(PIN_LIGHT_BLUE,Light_Blue);
    }

    void Update()
    {
        if(Is_Light_Smart)
        {
            int light = 0;
            if(Light_Intensive<Light_Threshold)
            {
                light = (Light_Threshold-Light_Intensive)*255/1000 + 30;
            }else
            {
                light = 0;
            }
            analogWrite(PIN_LIGHT_RED,light);
            analogWrite(PIN_LIGHT_BLUE,light);
            analogWrite(PIN_LIGHT_GREEN,light);
        }else
        {
            analogWrite(PIN_LIGHT_RED,Light_Red);
            analogWrite(PIN_LIGHT_GREEN,Light_Green);
            analogWrite(PIN_LIGHT_BLUE,Light_Blue);
        }
        
    }

    void SetLight(int r,int g,int b)
    {
        Light_Red = r;
        Light_Green = g;
        Light_Blue = b;
        this->Update();
    }
};

#endif