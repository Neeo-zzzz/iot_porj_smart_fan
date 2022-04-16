#include "MQTT.h"
#include "header.h"
#include "light.h"

#define _DEBUG_

MQTT* mqtt;
light* rgb_light;
void setup()
{
    PinInit();
    mqtt = new MQTT();
    rgb_light = new light();
    Serial.println("===================connect success===================");
}

void loop()
{
    //receive the info
    mqtt->ReceiveInfo();

    //update the param
    if(Is_Param_Change)
    {
        rgb_light->Update();
        Is_Param_Change = false;
    }

    //read the sensor

    //send the current param 
    mqtt->UpdateDate();
    delay(1000);
}