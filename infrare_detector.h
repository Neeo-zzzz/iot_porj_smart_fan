
#define PIN_INFRARE_5V 32 
#define PIN_INFRARE_OUT 53

class infrare_detector
{
    public:
    infrare_detector()
    {
        pinMode(PIN_INFRARE_5V,OUTPUT);
        pinMode(PIN_INFRARE_OUT,INPUT);
        digitalWrite(PIN_INFRARE_5V,HIGH);
    }

    bool GetState()
    {
        if(digitalRead(PIN_INFRARE_OUT)==HIGH)
        {
            return false;
        }else
        {
            return true;
        }
    }
};