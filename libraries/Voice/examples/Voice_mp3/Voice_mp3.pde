/*
  ����ʶ�����̡�
  �ɶ���Ȥ�Ƽ����޹�˾ ��Ȩ����
  http://www.dfrobot.com.cn
  http://dfrobot.taobao.com/
  ��������������ʶ����ƿ��عصƣ���ͨ��MP3ģ�鲥��������ʵ���˻�������
*/

#include <Wire.h>
#include <Voice.h>
#define ArduinoPlayer_address 0x35 //����ArduinoPlayer ��������ģ��I2C��ַ 

#define SUM 4  //SUMʶ��ؼ��ʵĸ���,��󲻳���50��

uint8  nAsrStatus=0;

char sRecog[SUM][80] = {"kai deng", "guan deng", "yin yue", "ting zhi"};//ÿ���ؼ���ƴ����ĸ����������79��,�û����������޸�

int state=7;  //״ָ̬ʾ��
int led=8;    //�ܿ������ֿ�

void TwiSend(const char  *cmd)  //I2C���������
{   
char len = 0;   
len = strlen(cmd); 
Wire.beginTransmission(ArduinoPlayer_address); 
while(len--)   
{     Wire.write(*(cmd++));        }  
Wire.endTransmission();   
}

void finally (unsigned char n)
{
switch(n)  //nΪ�����ж�Ӧ�ؼ��ʵ����кţ���������sRecog�еĵ�һ���ؼ���Ϊ��kai deng�����Ӧ�����к�Ϊ0��
	{
	case 0:
		Serial.println( "kai deng");
                Serial.println( " ");
                TwiSend("\\:v 250\r\n");    
                TwiSend("\\kaideng\r\n");    //ָ���ļ�����������     
                delay(2000);
                digitalWrite(led,LOW);
		break;
	case 1:
		Serial.println( "guan deng");
                Serial.println( " ");
                TwiSend("\\:v 250\r\n");    
                TwiSend("\\guandeng\r\n");   
                delay(2000);
                digitalWrite(led,HIGH);
		break;
	case 2:
		Serial.println( "muisc ok!");
                Serial.println( " ");
                TwiSend("\\:v 250\r\n");    
                TwiSend("\\:n\r\n");   
		break;
	case 3:
		Serial.println( "muisc stop!");
                Serial.println( " ");
                TwiSend("\\:v 250\r\n");    
                TwiSend("\\:p\r\n");   
		break;
	default:
	        Serial.println( "error");
                Serial.println( " ");
		break;	
	}
}

void ExtInt0Handler ()
{
  Voice.ProcessInt0();	//оƬ�ͳ��ж��ź�				
}

void setup()
{
  Wire.begin(); 
  Serial.begin(9600);
  Voice.Initialise(MIC,VoiceRecognitionV1);//Initialise mode MIC or MONO,default is MIC
                                           //VoiceRecognitionV1 is VoiceRecognitionV1.0 shield
                                           //VoiceRecognitionV2 is VoiceRecognitionV2.1 module
  attachInterrupt(0,ExtInt0Handler,LOW);
  pinMode(led,OUTPUT);
  pinMode(state,OUTPUT);
  digitalWrite(state,HIGH);
  digitalWrite(led,HIGH);
}
void loop()
{  
        uint8 nAsrRes;
	nAsrStatus = LD_ASR_NONE;

	while(1)
	{
		switch(nAsrStatus)
		{
			case LD_ASR_RUNING:
			case LD_ASR_ERROR:		
				break;
			case LD_ASR_NONE:
			{
				nAsrStatus=LD_ASR_RUNING;
                            if (Voice.RunASR(SUM,80,sRecog)==0)  //ʶ����ȷ
				{      
					nAsrStatus= LD_ASR_ERROR;
                                        Serial.println( "ASR_ERROR"); 
				}
                              digitalWrite(state,LOW);
                              Serial.println( "ASR_RUNING.....");
				break;
			}
			case LD_ASR_FOUNDOK:
			{
                                digitalWrite(state,HIGH);
				nAsrRes =Voice. LD_GetResult();//һ��ASRʶ�����̽�����ȥȡASRʶ����
                                finally(nAsrRes); 
				nAsrStatus = LD_ASR_NONE;
				break;
			}
			case LD_ASR_FOUNDZERO:
			default:
			{
				
				nAsrStatus = LD_ASR_NONE;
				break;
			}
	         }// switch
              delay(500);
	}// while
}