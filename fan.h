#ifndef _FAN_
#define _FAN_
#include "header.h"
#define PIN_FAN_INA 33
#define PIN_FAN_INB 31
#define PIN_FAN_5V 30
//#define _DEBUG_

class fan
{
    public:
    int now_rotate_speed;
    fan()
    {
        pinMode(PIN_FAN_INA,OUTPUT);
        pinMode(PIN_FAN_INB,OUTPUT);
        pinMode(PIN_FAN_5V,OUTPUT);
        now_rotate_speed = 0;
        digitalWrite(PIN_FAN_5V, HIGH);
        digitalWrite(PIN_FAN_INB,LOW);
        analogWrite(PIN_FAN_INA,now_rotate_speed);
    }

    void SetSpeed(int n)
    {
//        Serial.print("input speed: ");
//        Serial.println(n);
        if(n<=0) this->now_rotate_speed = 0;
        else if(n>=255) this->now_rotate_speed = 255;
        else
        {
            this->now_rotate_speed = n;
        }

        if(Is_Fan_Safe)
        {
            //safe mode
            if(Is_People) this->now_rotate_speed = 0;
            else
            {
                if(Is_Fan_Humidifier_Smart) this->now_rotate_speed = GetAutoSpeed();
            }
        }else
        {
            if(Is_Fan_Humidifier_Smart) this->now_rotate_speed = GetAutoSpeed();
        }

        analogWrite(PIN_FAN_INA,this->now_rotate_speed);
        #ifdef _DEBUG_
        //Serial.print("now speed: ");
        //Serial.println(this->now_rotate_speed);
        #endif
    }

    int GetAutoSpeed()
    {
        if(Temperature>Temp_Threshold)
        {
            return 130 + (int)(Temperature-Temp_Threshold)*50;
        }else
        {
            return 0;
        }
    }
};

#endif
