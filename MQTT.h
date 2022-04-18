#ifndef _MQTT_
#define _MQTT_

#define _DEBUG_
#include <Arduino.h>
#include <ArduinoJson.h>
#include "header.h"

//User Modified Part
#define wifi_ssid     "neeoneeo"    
#define wifi_psw      "zjp011018"     
// #define clientIDstr   "gw7ywqzvFMl.test_1"
// #define TIME_STAMP     "999"
// #define ProductKey    "gw7ywqzvFMl"
// #define DeviceName    "test_1"
// #define DeviceSecret  "7a7536faa693e159fbc0fe68b7b77ca5"
// #define password      "83D1F779E3568BA9BE753D2034AE812D194FDA02"

#define clientIDstr   "gw7y8M2T5CN.fan_1"
#define TIME_STAMP     "999"
#define ProductKey    "gw7y8M2T5CN"
#define DeviceName    "fan_1"
#define DeviceSecret  "9af9697aa13f373c15093172f2eeb589"
#define password      "DCEFFD695FE2CB5BC8465C08E54462F0B23EB80D"

//ATcmd Format
#define AT                    "AT\r"
#define AT_OK                 "OK"
#define AT_REBOOT             "AT+REBOOT\r"
#define AT_ECHO_OFF           "AT+UARTE=OFF\r"
#define AT_MSG_ON             "AT+WEVENT=ON\r"

#define AT_WIFI_START         "AT+WJAP=%s,%s\r"
#define AT_WIFI_START_SUCC    "+WEVENT:STATION_UP"

#define AT_MQTT_AUTH          "AT+MQTTAUTH=%s&%s,%s\r"
#define AT_MQTT_CID           "AT+MQTTCID=%s|securemode=3\\,signmethod=hmacsha1\\,timestamp=%s|\r"
#define AT_MQTT_SOCK          "AT+MQTTSOCK=%s.iot-as-mqtt.cn-shanghai.aliyuncs.com,1883\r"

#define AT_MQTT_AUTOSTART_OFF "AT+MQTTAUTOSTART=OFF\r"
#define AT_MQTT_ALIVE         "AT+MQTTKEEPALIVE=500\r"
#define AT_MQTT_START         "AT+MQTTSTART\r"
#define AT_MQTT_START_SUCC    "+MQTTEVENT:CONNECT,SUCCESS"
#define AT_MQTT_PUB_SET       "AT+MQTTPUB=/sys/%s/%s/thing/event/property/post,1\r"
#define AT_MQTT_PUB_ALARM_SET "AT+MQTTPUB=/sys/%s/%s/thing/event/GasAlarm/post,1\r"
#define AT_MQTT_PUB_DATA      "AT+MQTTSEND=%d\r"
#define JSON_DATA_PACK        "{\"id\":\"100\",\"version\":\"1.0\",\"method\":\"thing.event.property.post\",\"params\":{\"RoomTemp\":%d.%02d,\"AC\":%d,\"Fan\":%d,\"Buzzer\":%d,\"GasDetector\":%d}}\r"
#define JSON_DATA_PACK_2      "{\"id\":\"110\",\"version\":\"1.0\",\"method\":\"thing.event.property.post\",\"params\":{\"LightDetector\":%d,\"Curtain\":%d,\"Light\":%d,\"SoilHumi\":%d,\"Pump\":%d,\"eCO2\":%d,\"TVOC\":%d}}\r"
#define JSON_DATA_PACK_ALARM  "{\"id\":\"110\",\"version\":\"1.0\",\"method\":\"thing.event.GasAlarm.post\",\"params\":{\"GasDetector\":%d}}\r"
#define JSON_DATA_LIGHT  "{\"id\":\"120\",\"version\":\"1.0\",\"method\":\"thing.event.property.post\",\"params\":{\"PhotoResistors\":%d}}\r"
#define JSON_DATA_RGB  "{\"id\":\"130\",\"version\":\"1.0\",\"method\":\"thing.event.property.post\",\"params\":{\"ColorRed\":%d,\"ColorGreen\":%d,\"ColorBlue\":%d}}\r"
#define AT_MQTT_PUB_DATA_SUCC "+MQTTEVENT:PUBLISH,SUCCESS"
#define AT_MQTT_UNSUB         "AT+MQTTUNSUB=2\r"
#define AT_MQTT_SUB           "AT+MQTTSUB=2,/sys/%s/%s/thing/service/property/set,1\r"
#define AT_MQTT_SUB_SUCC      "+MQTTEVENT:2,SUBSCRIBE,SUCCESS"
#define AT_MQTT_CLOSE          "AT+MQTTCLOSE\r"
#define AT_MQTT_KEEPALIVE       "AT+MQTTKEEPALIVE=%d\r"

#define MQTT_SEND_LIGHT_JSON "{\"method\":\"%s\",\"id\":\"%s\",\"params\":{\"PhotoResistors\":%d},\"version\":\"1.0\"}" //方法 id 光强度
#define MQTT_ALL_PARAMS_JSON_TEMPLATE "{\"Temperature\":%.2f,\"Rotate_Speed\":%d,\"Humidity\":%.2f,\"Is_People\":%d,\"Is_Pump\":%d,\"Light_Intensive\":%d,\"Light_Red\":%d,\"Light_Greed\":%d,\"Light_Blue\":%d}"
#define MQTT_JSON_PUB_TEMPLATE "{\"id\":\"160\",\"version\":\"1.0\",\"method\":\"thing.event.property.post\",\"params\":%s}\r"
#define AT_BUZZER_MUTE           "\"Buzzer\":2"


#define DEFAULT_TIMEOUT       10   //seconds
#define BUF_LEN               100
#define BUF_LEN_DATA          190

//Logic Preset
#define OFF           0
#define ON            1
#define MUTE          2
#define KEEP_OFF      2
#define KEEP_ON       3

#define Buzzer_ON   digitalWrite(BuzzerPin,HIGH)
#define Buzzer_OFF  digitalWrite(BuzzerPin,LOW)

#define BuzzerPin 3


class MQTT
{
    public:
    char ATcmd[BUF_LEN];
    char ATbuffer[BUF_LEN];
    char ATdata[BUF_LEN_DATA];
    int start_time, end_time;
    char method[100];
    char id[30];

    //storage last send message
    //DynamicJsonDocument json_data(1024);

    /**
     * @brief Construct a new MQTT object, init the mqtt, connect with aliyun, subscrip the topic
     * and init the Serial
     * 
     */
    MQTT();
    void BEEP(int b_time);
    bool check_send_cmd(const char* cmd,const char* resp,unsigned int timeout);
    bool WiFiInit();
    bool AliConnect();
    void cleanBuffer(char *buf,int len);

    /**
     * @brief using the data received to update the global variable
     * 
     * @param data the raw data string 
     * @return int the number of the received keys
     */
    int Parse(String data);
    void KeepAlive();

    /**
     * @brief send the input string by mqtt send
     * 
     * @param data 
     * @param len 
     */
    void SendInfo(char* data,int len=0);
    void ReceiveInfo();
    /**
     * @brief upload the device data into the backend
     * 
     */
    void UpdateDate();
};

#endif


// int MQTT::Parse(String data)
// {
//     #ifdef _DEBUG_
//     Serial.println("receive raw data:");
//     Serial.println(data);
//     #endif
//     int commaPosition;  
//     commaPosition = data.indexOf('{');
//     data= data.substring(commaPosition, data.length());
//     char* temp_data = (char* ) data.c_str();
//     //Serial.println(data);

//     //JsonObject& root = jsonBuffer.parseObject(data);
//     DynamicJsonDocument root(1024);
//     deserializeJson(root, data);

//     const char* method  = root["method"];
//     const char* id      = root["id"];
//     int         Frequency  = root["params"]["Frequency"];
//     int r,g,b;

//     //处理param的数据
//     Serial.println("params_data:");
//     Serial.println(temp_data);
//     if(strstr(temp_data,"ColorRed")!=NULL) ColorRed = root["params"]["ColorRed"];
//     if(strstr(temp_data,"ColorGreen")!=NULL) ColorGreen = root["params"]["ColorGreen"];
//     if(strstr(temp_data,"ColorBlue")!=NULL) ColorBlue = root["params"]["ColorBlue"];

//     //返回数据
//     Serial.println("return the message");
//     sendLight(ColorRed,ColorGreen,ColorBlue);
//     return Frequency;  
// }