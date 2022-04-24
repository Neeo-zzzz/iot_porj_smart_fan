#ifndef _PUMP_
#define _PUMP_
#include "header.h"

#define PIN_PUMP_VCC 24
#define PIN_PUMP_GND 25

class pump
{
    public:
    pump()
    {
        pinMode(PIN_PUMP_VCC,OUTPUT);
        pinMode(PIN_PUMP_GND,OUTPUT);
        digitalWrite(PIN_PUMP_GND,LOW);
    }

    void update()
    {
        if(!Is_Fan_Humidifier_Smart)
        {
            if(Is_Pump) digitalWrite(PIN_PUMP_VCC,HIGH);
            else digitalWrite(PIN_PUMP_VCC,LOW);
        }else
        {
            if(Humidity<Humidity_Threshold) digitalWrite(PIN_PUMP_VCC,HIGH);
            else digitalWrite(PIN_PUMP_VCC,LOW);
        }
    }
};


#endif