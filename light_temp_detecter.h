#include "header.h"
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <Adafruit_BMP280.h>


//Pin Map
#define SprayPin    2
#define BuzzerPin   3
#define FanPin      5
#define SomethingOpenPin  6
#define SomethingClosePin 7

#define Spary_ON  digitalWrite(SparyPin,HIGH)
#define Spary_OFF digitalWrite(SparyPin,LOW)


#define Spary_ON_val   (28.00)
#define Spary_OFF_val  (26.00)
#define Light_ON_val    400
#define Light_OFF_val   200

#define PIN_LIGHT_DETECTOR A2

Adafruit_BMP280 bme;
class LT_detector
{
    public:
    LT_detector()
    {
        if (!bme.begin(0x76)) {
            Serial.println("Could not find a valid BME280 sensor, check wiring!");
        }
        pinMode(SprayPin,OUTPUT);
        pinMode(PIN_LIGHT_DETECTOR,INPUT);
        digitalWrite(SprayPin,LOW);
    }

    double GetTemperature()
    {
        return bme.readTemperature();
    }

    int GetLightIntensive()
    {
        return 1000 - analogRead(PIN_LIGHT_DETECTOR);
    }
};
