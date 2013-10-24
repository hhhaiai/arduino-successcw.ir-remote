/*
  ����ʶ�����̡�
  �ɶ���Ȥ�Ƽ����޹�˾ ��Ȩ����
  http://www.dfrobot.com.cn
  http://dfrobot.taobao.com/
  ��������������ʶ����ƿ��عصƣ�ʵ���˻�������
*/

#include <Voice.h>

#define SUM 2 //SUMʶ��ؼ��ʵĸ���,��󲻳���50��
uint8  nAsrStatus=0;

char sRecog[SUM][80] = {"kai deng", "guan deng"};//ÿ���ؼ���ƴ����ĸ����������79��,�û����������޸�       ? 

int state=7;  //״ָ̬ʾ��
int led=8;    //�ܿ������ֿ�


void finally (unsigned char n)
{
switch(n)  //nΪ�����ж�Ӧ�ؼ��ʵ����кţ���������sRecog�еĵ�һ���ؼ���Ϊ��kai deng�����Ӧ�����к�Ϊ0��

	{
	case 0:
		Serial.println( "kai deng");
              Serial.println( " ");
              digitalWrite(led,LOW);
		break;
	case 1:
		Serial.println( "guan deng");
              digitalWrite(led,HIGH);
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