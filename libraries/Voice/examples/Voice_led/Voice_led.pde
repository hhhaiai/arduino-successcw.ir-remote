/*
  语音识别例程。
  成都极趣科技有限公司 版权所有
  http://www.dfrobot.com.cn
  http://dfrobot.taobao.com/
  功能描述：语音识别控制开关关灯，实现人机互动。
*/

#include <Voice.h>

#define SUM 2 //SUM识别关键词的个数,最大不超过50个
uint8  nAsrStatus=0;

char sRecog[SUM][80] = {"kai deng", "guan deng"};//每个关键词拼音字母个数不超过79个,用户可以自行修改       ? 

int state=7;  //状态指示灯
int led=8;    //受控制数字口


void finally (unsigned char n)
{
switch(n)  //n为数组中对应关键词的序列号，例如数组sRecog中的第一个关键词为“kai deng”则对应的序列号为0；

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
  Voice.ProcessInt0();	//芯片送出中断信号				
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
                            if (Voice.RunASR(SUM,80,sRecog)==0)  //识别不正确
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
				nAsrRes =Voice. LD_GetResult();//一次ASR识别流程结束，去取ASR识别结果
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