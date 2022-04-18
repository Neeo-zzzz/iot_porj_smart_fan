#include "MQTT.h"

MQTT::MQTT()
{
    //init variable
    start_time = end_time = 0;

    Serial3.begin(115200);
    Serial.begin(9600);

    //BEEP(1);
    Serial.println("setup success!");
    while(1)
    {
        if(!WiFiInit()) continue;
        //BEEP(2);
        Serial.println("wifi connect success!");
        if(!AliConnect()){Serial.println("ali connect failed!"); continue; }
        break;
    }

    Serial.println("Aliyun connect success!");
    //BEEP(3);
    
    //connect and sub finished
}

int MQTT::Parse(String data)
{
    #ifndef _DEBUG_
    Serial.println("receive raw data:");
    Serial.println(data);
    #endif

    DynamicJsonDocument json_data(1024);
    json_data.clear();
    int command_position = data.indexOf('{');
    data = data.substring(command_position,data.length());
    char* temp_data = (char*) data.c_str();
    
    deserializeJson(json_data,data);
    strcpy(method,json_data["method"]);
    strcpy(id,json_data["id"]);

    //process the data in params and updata the global variable
    //if(strstr(temp_data,"Temperature")!=NULL) Temperature = json_data["params"]["Temperature"];
    if(strstr(temp_data,"Rotate_Speed")!=NULL) Rotate_Speed = json_data["params"]["Rotate_Speed"];
    //if(strstr(temp_data,"Humidity")!=NULL) Humidity = json_data["params"]["Humidity"];
    //if(strstr(temp_data,"Is_People")!=NULL) Is_People = json_data["params"]["Is_People"];
    if(strstr(temp_data,"Is_Pump")!=NULL) Is_Pump = json_data["params"]["Is_Pump"];
    //if(strstr(temp_data,"Light_Intensive")!=NULL) Light_Intensive = json_data["params"]["Light_Intensive"];
    if(strstr(temp_data,"Light_Red")!=NULL) Light_Red = json_data["params"]["Light_Red"];
    if(strstr(temp_data,"Light_Green")!=NULL) Light_Green = json_data["params"]["Light_Green"];
    if(strstr(temp_data,"Light_Blue")!=NULL) Light_Blue = json_data["params"]["Light_Blue"];

    if(strstr(temp_data,"Is_Fan_Humidifier_Smart")!=NULL) Is_Fan_Humidifier_Smart = json_data["params"]["Is_Fan_Humidifier_Smart"];
    if(strstr(temp_data,"Is_Fan_Safe")!=NULL) Is_Fan_Safe = json_data["params"]["Is_Fan_Safe"];
    if(strstr(temp_data,"Is_Light_Smart")!=NULL) Is_Light_Smart = json_data["params"]["Is_Light_Smart"];
    if(strstr(temp_data,"Temp_Threshold")!=NULL) Temp_Threshold = json_data["params"]["Temp_Threshold"];
    if(strstr(temp_data,"Humidity_Threshold")!=NULL) Humidity_Threshold = json_data["params"]["Humidity_Threshold"];
    if(strstr(temp_data,"Light_Threshold")!=NULL) Light_Threshold = json_data["params"]["Light_Threshold"];


    return json_data.size();
}



bool MQTT::check_send_cmd(const char* cmd,const char* resp,unsigned int timeout)
{
    int i = 0;
    unsigned long timeStart;
    timeStart = millis();
    cleanBuffer(ATbuffer,BUF_LEN);
    Serial3.print(cmd);
    Serial3.flush();

    #ifdef _DEBUG_
    Serial.println("send the messave:");
    Serial.println(cmd);
    #endif
    
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
    snprintf(ATcmd,BUF_LEN,AT_MQTT_CID,clientIDstr,TIME_STAMP);
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
    cleanBuffer(ATcmd,BUF_LEN);
    snprintf(ATcmd,BUF_LEN,AT_MQTT_PUB_SET,ProductKey,DeviceName);
    flag = check_send_cmd(ATcmd,AT_OK,DEFAULT_TIMEOUT);

    // //生成要发送的json数据
    // cleanBuffer(ATdata,BUF_LEN_DATA);
    // len = snprintf(ATdata,BUF_LEN_DATA,JSON_DATA_RGB);

    //给出mqttsend指令
    cleanBuffer(ATcmd,BUF_LEN);
    snprintf(ATcmd,BUF_LEN,AT_MQTT_PUB_DATA,len-1);
    flag = check_send_cmd(ATcmd,">",DEFAULT_TIMEOUT);
    if(flag) 
    {
        flag = check_send_cmd(data,AT_MQTT_PUB_DATA_SUCC,DEFAULT_TIMEOUT);
        #ifdef _DEBUG_
        Serial.println("mqtt send success!");
        //Serial.println(flag);
        Serial.println("final return: ");
        Serial.println(ATcmd);
        Serial.println(data);
        #endif
        if(!flag)
        {
            Serial.println("send commend failed!");
        }
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
            Is_Param_Change = true;
    }
  }
}

void MQTT::UpdateDate()
{
    char buffer[400];
    snprintf(buffer,400,MQTT_ALL_PARAMS_JSON_TEMPLATE,Temperature,Rotate_Speed,Humidity,Is_People,Is_Pump,Light_Intensive,Light_Red,Light_Green,Light_Blue,Is_Fan_Humidifier_Smart,Is_Fan_Safe,Is_Light_Smart,Temp_Threshold,Humidity_Threshold,Light_Threshold);
    char cmd[500];
    snprintf(cmd,500,MQTT_JSON_PUB_TEMPLATE,buffer);
    int i = 0;
    for(i = 0;i<500;i++)
    {
        if(cmd[i]==0) break;
    }
    SendInfo(cmd,i);
}