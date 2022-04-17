//�������¶ȿ��ƣ�û�����ֱ�ӿ�����
//�����ɹ�ǿ��ֵ���ƣ���ӿ���һ������Something���ı仯



//Logic Preset
 #define OFF  0
 #define ON   1

#define Spary_ON  digitalWrite(SparyPin,HIGH)
#define Spary_OFF digitalWrite(SparyPin,LOW)


#define Spary_ON_val   (28.00)
#define Spary_OFF_val  (26.00)
#define Light_ON_val    400
#define Light_OFF_val   200


//Pin Map
#define SprayPin    2
#define BuzzerPin   3
#define FanPin      5
#define SomethingOpenPin  6
#define SomethingClosePin 7

#define AirHumi       A1


//Statues Pool//��¼״̬
float RoomTemp ;//�����¶� �������� 
int Spray = OFF;
int Buzzer = OFF;
int Something = ON;
int Light = OFF ;
int LightDetector = 0;
int SoilHumi = 0;

 
 
void setup() {
    Serial3.begin(115200);
    Serial.begin(115200);
    
    Pin_init();

}
 

#define Spray_ON_val  (28.00)
#define Spray_OFF_val  (26.50)

void loop() {
   //SensorCollect
    RoomTemp = bme.readTemperature();
     
    Serial.print("RoomTemp:");
    Serial.println(RoomTemp);
    
  //Logic Process
  //1.Spray������
  //���¶���ϵ�������¶���ֵ�������������¶���ֵ���ر������������滻ʪ��
  if((RoomTemp > Spray_ON_val)&&(SprayPin == OFF))
  {
    Spray = ON;
    Spray_ON;
    //Serial.println("����״̬-��"); 
    //delay(1000);//����ʱ�� 
  }
  if((RoomTemp < Spray_OFF_val)&&(SprayPin == ON))
  {
    Spray = OFF;
    Spray_OFF;
    //Serial.println("����״̬-�ر�"); 
    //delay(1000);//����ʱ��    
  }

  //2.����  Something���������������飬�緢��
  if((LightDetector > Light_ON_val)&&(Something == ON))
  {
    Something = OFF;
    Something_OFF();
    Light = ON;
    Light_ON();
    
  }

  if((LightDetector < Light_OFF_val)&&(Something == OFF))
  {
    Something = ON;
    Something_ON();
    Light = OFF;
    Light_OFF();
    
  }



   //Upload
   //Upload();

   //MsgReceive
   //if (check_send_cmd(AT, AT_BUZZER_MUTE, DEFAULT_TIMEOUT))Buzzer_mute();
}


void Pin_init() {
  pinMode(SprayPin,OUTPUT);
  digitalWrite(SprayPin,LOW);
  //��������
}

void Upload(){

}