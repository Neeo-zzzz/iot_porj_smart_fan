//#define _DEBUG_
#include "MQTT.h"
#include "header.h"
#include "light.h"
#include "light_temp_detecter.h"
#include "fan.h"
#include "infrare_detector.h"


MQTT* mqtt;
light* rgb_light;
LT_detector* lt_sensor;
fan* f;
infrare_detector* if_d;
int temp;

void InitDevice()
{
    PinInit();
    mqtt = new MQTT();
    rgb_light = new light();
    lt_sensor = new LT_detector();
    f = new fan();
    if_d = new infrare_detector();
}

void SetComponentValue()
{
    f->SetSpeed(Rotate_Speed);
    rgb_light->Update();
}

void GetComponentValue()
{
    Temperature = lt_sensor->GetTemperature();
    Light_Intensive = lt_sensor->GetLightIntensive();
    Is_People = if_d->GetState();
    Rotate_Speed = f->now_rotate_speed;
}

void setup()
{
    InitDevice();
}

void loop()
{
    //receive the info
    mqtt->ReceiveInfo();

    //update the param
    if(Is_Param_Change)
    {
        SetComponentValue();
        Is_Param_Change = false;
        
    }
    GetComponentValue();
    mqtt->UpdateDate();
    delay(1000);
}