#include "MQTT.h"

MQTT::MQTT()
{
    start_time = end_time = 0;
    Serial3.begin(115200);
    Serial.begin(9600);

    BEEP(1);
    while(1)
    {
        if(!WiFiInit()) continue;
        BEEP(2);
        if(!AliConnect()) continue;
        break;
    }
    BEEP(3);
    
    //connect and sub finished
}

int MQTT::Parse(String data)
{

}



bool MQTT::check_send_cmd(const char* cmd,const char* resp,unsigned int timeout)
{
    int i = 0;
    unsigned long timeStart;
    timeStart = millis();
    cleanBuffer(ATbuffer,BUF_LEN);
    Serial3.print(cmd);
    Serial3.flush();
    while(1)
    {
    while(Serial3.available())
    {
        ATbuffer[i++] = Serial3.read();
        if(i >= BUF_LEN)i = 0;
    }
    if(NULL != strstr(ATbuffer,resp))break;
    if((unsigned long)(millis() - timeStart > timeout * 1000)) break;
    }

    if(NULL != strstr(ATbuffer,resp))return true;
    return false;
}

void MQTT::cleanBuffer(char *buf,int len)
{
    for(int i = 0;i < len;i++)
    {
        buf[i] = '\0';
    } 
}
void MQTT::BEEP(int b_time)
{
    for(int i = 1;i <= b_time;i++)
    { 
        digitalWrite(BuzzerPin,HIGH);
        delay(100);
        digitalWrite(BuzzerPin,LOW);
        delay(100);
    }
}

bool MQTT::WiFiInit()
{
    bool flag;

    flag = check_send_cmd(AT,AT_OK,DEFAULT_TIMEOUT);
    if(!flag)return false;

    flag = check_send_cmd(AT_REBOOT,AT_OK,20);
    if(!flag)return false;
    delay(5000);

    flag = check_send_cmd(AT_ECHO_OFF,AT_OK,DEFAULT_TIMEOUT);
    if(!flag)return false;

    flag = check_send_cmd(AT_MSG_ON,AT_OK,DEFAULT_TIMEOUT);
    if(!flag)return false;

    cleanBuffer(ATcmd,BUF_LEN);
    snprintf(ATcmd,BUF_LEN,AT_WIFI_START,wifi_ssid,wifi_psw);
    flag = check_send_cmd(ATcmd,AT_WIFI_START_SUCC,20);
    return flag;
}

bool MQTT::AliConnect()
{
    bool flag;
    bool flag1;

    cleanBuffer(ATcmd,BUF_LEN);
    snprintf(ATcmd,BUF_LEN,AT_MQTT_AUTH,DeviceName,ProductKey,password);
    flag = check_send_cmd(ATcmd,AT_OK,DEFAULT_TIMEOUT);
    if(!flag)return false;

    cleanBuffer(ATcmd,BUF_LEN);
    snprintf(ATcmd,BUF_LEN,AT_MQTT_CID,clientIDstr,timestamp);
    flag = check_send_cmd(ATcmd,AT_OK,DEFAULT_TIMEOUT);
    if(!flag)return false;

    cleanBuffer(ATcmd,BUF_LEN);
    snprintf(ATcmd,BUF_LEN,AT_MQTT_SOCK,ProductKey);
    flag = check_send_cmd(ATcmd,AT_OK,DEFAULT_TIMEOUT);
    if(!flag)return false;

    flag = check_send_cmd(AT_MQTT_AUTOSTART_OFF,AT_OK,DEFAULT_TIMEOUT);
    if(!flag)return false;

    flag = check_send_cmd(AT_MQTT_ALIVE,AT_OK,DEFAULT_TIMEOUT);
    if(!flag)return false;

    flag = check_send_cmd(AT_MQTT_START,AT_MQTT_START_SUCC,20);
    if(!flag)return false;

    cleanBuffer(ATcmd,BUF_LEN);
    snprintf(ATcmd,BUF_LEN,AT_MQTT_PUB_SET,ProductKey,DeviceName);
    flag = check_send_cmd(ATcmd,AT_OK,DEFAULT_TIMEOUT);
    if(!flag)return false;

    cleanBuffer(ATcmd,BUF_LEN);
    snprintf(ATcmd,BUF_LEN,AT_MQTT_SUB,ProductKey,DeviceName);
    flag = check_send_cmd(ATcmd,AT_MQTT_SUB_SUCC,DEFAULT_TIMEOUT);
    if(!flag){ BEEP(4);flag1 = check_send_cmd(AT_MQTT_CLOSE,AT_OK,DEFAULT_TIMEOUT);}
    return flag;
}
void MQTT::SendInfo(char* data,int len)
{
    //pubset
    bool flag;
    int len;
    cleanBuffer(ATcmd,BUF_LEN);
    snprintf(ATcmd,BUF_LEN,AT_MQTT_PUB_SET,ProductKey,DeviceName);
    flag = check_send_cmd(ATcmd,AT_OK,DEFAULT_TIMEOUT);

    //生成要发送的json数据
    cleanBuffer(ATdata,BUF_LEN_DATA);
    len = snprintf(ATdata,BUF_LEN_DATA,JSON_DATA_RGB,r,g,b);


    //给出mqttsend指令
    cleanBuffer(ATcmd,BUF_LEN);
    snprintf(ATcmd,BUF_LEN,AT_MQTT_PUB_DATA,len-1);
    flag = check_send_cmd(ATcmd,">",DEFAULT_TIMEOUT);
    if(flag) 
    {
        Serial.println("mqtt send success!");
        flag = check_send_cmd(ATdata,AT_MQTT_PUB_DATA_SUCC,DEFAULT_TIMEOUT);
        Serial.println(flag);
        Serial.println("final return: ");
        Serial.println(ATcmd);
        Serial.println(ATdata);
    }
    else{
        Serial.println("mqtt send fault!");
    }
    while(Serial3.available()>0){
        //delay(100);
        String s = Serial3.readString();
    }
    return;

}

void MQTT::ReceiveInfo()
{
    String inString;
    delay(10);
    if (Serial3.available()>0){
        delay(100);
        inString=Serial3.readString();
        if (inString!=""){
            Parse(inString);
    }
  }
}