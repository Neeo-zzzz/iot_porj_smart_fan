//#define _DEBUG_
#include "MQTT.h"
#include "header.h"
#include "light.h"
#include "light_temp_detecter.h"
#include "fan.h"
#include "infrare_detector.h"
#include "pump.h"
#include "humidity_detector.h"


MQTT* mqtt; //wifi mqtt sender
light* rgb_light; //rgb light
LT_detector* lt_sensor; //light sensor
fan* f; //fans
infrare_detector* if_d; //infrare sensor
int temp;
hd* humidity_detector; //humidity detector
pump* pump_controler; //atomize slice

void InitDevice()
{
    PinInit(); //init the global varient
    mqtt = new MQTT();
    rgb_light = new light();
    lt_sensor = new LT_detector();
    f = new fan();
    if_d = new infrare_detector();
    humidity_detector = new hd();
    pump_controler = new pump();
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
    pump_controler->update();
    humidity_detector->upload();
}

void setup()
{
    InitDevice();
}

void loop()
{

    //receive the info
    mqtt->ReceiveInfo();
    //update the component configurence
    SetComponentValue();
    //read the new data
    GetComponentValue();
    //update the enviernment params and send to the back end
    mqtt->UpdateDate();
    delay(1000);
}
