/*
 * IRremote: IRsendDemo - demonstrates sending IR codes with IRsend
 * An IR LED must be connected to Arduino PWM pin 3.
 * Version 0.1 July, 2009
 * Copyright 2009 Ken Shirriff
 * http://arcfn.com
 */

#include <IRremote.h>

IRsend irsend;
int codeType = -1; // The type of code
unsigned long codeValue; // The code value if not raw
int codeLen = 0; // The length of the code
int toggle = 0; // The RC5/6 toggle state
unsigned int rawCodes[RAWBUF]; // The durations if raw

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

void setup()
{
  Serial.begin(115200);
}

void loop() {
  char line[5] = "";
  int val = 0;
  if(Serial.available()) {
    Serial.readBytesUntil('\0', line, 5);
    //Serial.println(line);
    line[3] = '\0';
    val = atoi(line);
    Serial.println(val);
    if(val <= 200) {//living room
      Serial.println("living room");
      LivingRoom(val);
    }
    else if(val <= 400) {//bedroom1
      Serial.println("bed room1");
      BedRoom_1(val - 200);
    }
    else if(val <= 600) {//bedroom2
      Serial.println("bed room2");
      BedRoom_2(val-400);
    }    
    //delay(1000);
    //kaidianshi();
    //delay(4000);
  }
}
