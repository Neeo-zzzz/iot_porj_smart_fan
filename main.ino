#include "MQTT.h"

MQTT* mqtt;
void setup()
{
    mqtt = new MQTT();
}

void loop()
{
    mqtt->BEEP(1);
    delay(10000);
    Serial.println("connect success");
}