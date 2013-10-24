#include <Voice.h>
#include <IRremote.h>

IRsend irsend;
int codeType = -1; // The type of code
unsigned long codeValue; // The code value if not raw
int codeLen = 0; // The length of the code
int toggle = 0; // The RC5/6 toggle state
unsigned int rawCodes[RAWBUF]; // The durations if raw

#define SUM  16//SUM识别关键词的个数,最大不超过50个
uint8  nAsrStatus=0;

char sRecog[SUM][80] = {
  "kai dian shi", "kai ji ding he", "guan dian shi", 
  "guan ji ding he", "qie huan", "sheng yin da dian",
  "sheng yin xiao dian", "zhi bo", "hui kan", "dian bo", "xin xi",
  "shang", "xia", "zuo", "you", "que ding"};//每个关键词拼音字母个数不超过79个,用户可以自行修改       ? 

//char sRecog[SUM][80] = { "kai dian shi", "kai ji ding he", "guan dian shi", "guan ji ding he", "qie huan", "sheng yin da dian"};
int state=7;  //状态指示灯
int led=8;    //受控制数字口

void sendCode(int repeat) {
  if (codeType == NEC) {
    if (repeat) {
      irsend.sendNEC(REPEAT, codeLen);
      Serial.println("Sent NEC repeat");
    } 
    else {
      irsend.sendNEC(codeValue, codeLen);
      Serial.print("Sent NEC ");
      Serial.println(codeValue, HEX);
    }
  } 
  else if (codeType == SHARP) {
    irsend.sendSharp(codeValue, codeLen);
    //Serial.print("Send Sharp ");
    //Serial.println(codeValue, HEX);
    //Serial.println(codeLen, DEC);
  }
  else if (codeType == SONY) {
    irsend.sendSony(codeValue, codeLen);
    Serial.print("Sent Sony ");
    Serial.println(codeValue, HEX);
  } 
  else if (codeType == RC5 || codeType == RC6) {
    if (!repeat) {
      // Flip the toggle bit for a new button press
      toggle = 1 - toggle;
    }
    // Put the toggle bit into the code to send
    codeValue = codeValue & ~(1 << (codeLen - 1));
    codeValue = codeValue | (toggle << (codeLen - 1));
    if (codeType == RC5) {
      Serial.print("Sent RC5 ");
      Serial.println(codeValue, HEX);
      irsend.sendRC5(codeValue, codeLen);
    } 
    else {
      irsend.sendRC6(codeValue, codeLen);
      Serial.print("Sent RC6 ");
      Serial.println(codeValue, HEX);
    }
  } 
  else if (codeType == UNKNOWN /* i.e. raw */) {
    // Assume 38 KHz
    irsend.sendRaw(rawCodes, codeLen, 38);
    Serial.println("Sent raw");
  }
}

void LivingRoom(unsigned int val) {
  if(val <= 50) { //TV
    Serial.println("TV");
    Serial.println(val);
    switch(val) {
    case 1: //open
      codeValue = 0x22c1;
      break;
    case 2: //close
      codeValue = 0x22c1;
      break;
    case 3: //volume up
      codeValue = 0x2281;
      break;
    case 4: //volume down
      codeValue = 0x22a1;
      break;
    case 5: //TV/AV
      codeValue = 0x2261;
      break;
    case 49:
      Serial.println("49");
      break;
    }
    codeType=SHARP;
    codeLen=15;
    sendCode(0);    
  }
  else if(val <=100) {//STB
    Serial.println("STB");
    Serial.println(val);
    switch(val - 50) {
    case 1: //open
      Serial.println("open STB");
      codeValue = 0xFFC23D;
      break;
    case 2: //close
      codeValue = 0xFFC23D;
      break;
    case 3: //volume up
      codeValue = 0xFFD22D;
      break;
    case 4: //volume down
      codeValue = 0xFF38C7;
      break;
    case 5: //zhibo
      codeValue = 0xFFFA05;
      break;
    case 6: //huikan
      codeValue = 0xFFDA25;
      break;
    case 7: //dianbo
      codeValue = 0xFFEA15;
      break;
    case 8: //xinxi
      codeValue = 0xFFCA35;
      break;
    case 9: //up
      codeValue = 0xFF28D7;
      break;
    case 10: //down
      codeValue = 0xFF6897;
      break;
    case 11: //left
      codeValue = 0xFFB847;
      break;
    case 12: //right
      codeValue = 0xFF8877;
      break;
    case 13: //ok
      codeValue = 0xFFA857;
      break;
    }
    codeType=NEC;
    codeLen=32;
    sendCode(0);
  }
  else {
    //TO DO
  }
  //  delay(1000);
  //  Serial.println("channel 6");
  //  codeType=SHARP;
  //  codeValue=0x2261;
  //  codeLen=15;
  //  sendCode(0);
}
void BedRoom_1(unsigned int val)
{
  //TO DO
}
void BedRoom_2(unsigned int val)
{
  //TO DO
}

void finally (unsigned char n)
{
  switch(n)  //n为数组中对应关键词的序列号，例如数组sRecog中的第一个关键词为“kai deng”则对应的序列号为0；

  {
  case 0:
    Serial.println( "kaidianshi");
    Serial.println( " ");
    LivingRoom(1);
    break;
  case 1:
    Serial.println( "kaijidinghe");
    LivingRoom(51);
    break;

  case 2:
    Serial.println( "guandianshi");
    LivingRoom(1);
    break;

  case 3:
    Serial.println( "guanjidinghe");
    LivingRoom(51);
    break;
  case 4:
    Serial.println( "qiehuan");
    LivingRoom(5);
    break;
  case 5:
    Serial.println( "shengyindadian");
    LivingRoom(3);
    break;
  case 6:
    Serial.println( "shenyingxiaodian");
    LivingRoom(4);
    break;
  case 7:
    Serial.println( "zhibo");
    LivingRoom(55);
    break;
  case 8:
    Serial.println( "huikan");
    LivingRoom(56);
    break;
  case 9:
    Serial.println( "dianbo");
    LivingRoom(57);
    break;
  case 10:
    Serial.println( "xinxi");
    LivingRoom(58);
    break;
  case 11:
    Serial.println( "shang");
    LivingRoom(59);
    break;
  case 12:
    Serial.println( "xia");
    LivingRoom(60);
    break;
  case 13:
    Serial.println( "zuo");
    LivingRoom(61);
    break;
  case 14:
    Serial.println( "you");
    LivingRoom(62);
    break;
  case 15:
    Serial.println( "queding");
    LivingRoom(63);
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
  Serial.begin(115200);
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
  Serial.println("start");
  if (Voice.RunASR(SUM,80,sRecog)==0)  //识别不正确
  {      
    nAsrStatus= LD_ASR_ERROR;
    Serial.println( "ASR_ERROR"); 
  }
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
        Voice.LD_AsrRun_successcw();
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


