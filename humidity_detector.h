#ifndef _HUMD_
#define _HUMD_
#include "header.h"
#include "dht11.h"

#define PIN_HUMD_VCC 48
#define PIN_HUMD_GND 50
#define PIN_HUMD_DATA 51

class hd
{
    public:
    dht11* dht;
    hd()
    {
        pinMode(PIN_HUMD_VCC,OUTPUT);
        pinMode(PIN_HUMD_GND,OUTPUT);
        pinMode(PIN_HUMD_DATA,INPUT);
        digitalWrite(PIN_HUMD_VCC,HIGH);
        digitalWrite(PIN_HUMD_GND,LOW);
        dht = new dht11();
    }

    int upload()
    {
        int temp = dht->read(PIN_HUMD_DATA);
        
        Humidity = dht->humidity;
        return temp;
    }
};

#endif 