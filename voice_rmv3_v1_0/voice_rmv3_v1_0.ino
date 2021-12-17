/**
   @file    voice_rmv3_v1_0
   @author  Daniel M
   @brief   Train & load voice commands to change LED colours

*/

//  WHAT THIS FILE DOES
// Change LED colours with preset colour commands
/**
   Commands
   0: Off     --> Turns LED OFF
   1: Red     --> Colour red
   2: Green   --> Colour green
   3: Blue    --> Colour blue
   4: Yellow  --> Colour yellow
   5: Cyan    --> Colour cyan
   6: Purple  --> Colour purple
*/
// red, green, blue
// yellow, cyan, purple
// OFF

#include <SoftwareSerial.h>
#include "VoiceRecognitionV3.h"

/*
   Pin Connection
   Arudino    VRMV3
      2        TXD
      3        RXD
*/

VR myVR(2, 3);  //2:RX, 3:TX

uint8_t records[7]; // save record
uint8_t buf[64];

int led_R = 11;
int led_B = 10;
int led_G = 9;

#define offRec      (0)
#define redRec      (1)
#define blueRec     (2)
#define greenRec    (3)
#define yellowRec   (4)
#define cyanRec     (5)
#define purpleRec   (6)

/**
   @brief   Print Signiture, no character -> hex value
   @param   buf --> Command Length
   @param   len --> number of parameters
*/
void printSignature(uint8_t *buf, int len)
{
  int i;
  for (i = 0; i < len; i++) {
    if (buf[i] > 0x19 && buf[i] < 0x7F) {
      Serial.write(buf[i]);
    }
    else {
      Serial.print("[");
      Serial.print(buf[i], HEX);
      Serial.print("]");
    }
  }
}

/**
   @brief   Print signature, no character -> hex value
   @param   buf   --> VRMV3 return value on voice recognized
            buf[0]  -->  Group mode(FF: None Group, 0x8n: User, 0x0n:System
            buf[1]  -->  number of record which is recognized.
            buf[2]  -->  Recognizer index(position) value of the recognized record.
            buf[3]  -->  Signature length
            buf[4]~buf[n] --> Signature
*/
void printVR(uint8_t *buf)
{
  Serial.println("VR Index\tGroup\tRecordNum\tSignature");

  Serial.print(buf[2], DEC);
  Serial.print("\t\t");

  if (buf[0] == 0xFF) {
    Serial.print("NONE");
  }
  else if (buf[0] & 0x80) {
    Serial.print("UG ");
    Serial.print(buf[0] & (~0x80), DEC);
  }
  else {
    Serial.print("SG ");
    Serial.print(buf[0], DEC);
  }
  Serial.print("\t");

  Serial.print(buf[1], DEC);
  Serial.print("\t\t");
  if (buf[3] > 0) {
    printSignature(buf + 4, buf[3]);
  }
  else {
    Serial.print("NONE");
  }
  Serial.println("\r\n");
}

void setup() {
  myVR.begin(9600);

  Serial.begin(115200);
  Serial.println("Elechouse Voice Recognition Module V3");

  // RGB LED setup
  pinMode(led_R, OUTPUT);   // red
  pinMode(led_B, OUTPUT);   // blue
  pinMode(led_G, OUTPUT);   // green

  // ON
  digitalWrite(led_R, LOW);
  digitalWrite(led_G, LOW);
  digitalWrite(led_B, LOW);
  delay(500);  
  // OFF
  digitalWrite(led_R, HIGH);
  digitalWrite(led_G, HIGH);
  digitalWrite(led_B, HIGH);

  if (myVR.clear() == 0) {
    Serial.println("Recognizer cleared.");
  } else {
    Serial.println("VoiceRecognitionModuleV3 not found.");
    Serial.println("Please check connection and restart Arduino.");
    while (1);
  }

  //Check that records have been loaded
  if (myVR.load((uint8_t)offRec) >= 0) {
    Serial.println("offRecord loaded");
  }

  if (myVR.load((uint8_t)redRec) >= 0) {
    Serial.println("redRec loaded");
  }

  if (myVR.load((uint8_t)blueRec) >= 0) {
    Serial.println("blueRec loaded");
  }

  if (myVR.load((uint8_t)greenRec) >= 0) {
    Serial.println("greenRec loaded");
  }

  if (myVR.load((uint8_t)yellowRec) >= 0) {
    Serial.println("yellowRec loaded");
  }

  if (myVR.load((uint8_t)cyanRec) >= 0) {
    Serial.println("cyanRec loaded");
  }

  if (myVR.load((uint8_t)purpleRec) >= 0) {
    Serial.println("purpleRec loaded");
  }

}

void loop() {
  int ret;
  ret = myVR.recognize(buf, 50);
  if (ret > 0) {
    switch (buf[1]) {
      case offRec:
        /** turn OFF LED */
//        RGB(255,255,255);
        digitalWrite(led_R, HIGH);
        digitalWrite(led_G, HIGH);
        digitalWrite(led_B, HIGH);
        
       // readAnalogVal();
        break;
      case redRec:
        RGB(255, 0, 0);
        break;
      case blueRec:
        RGB(0, 0, 255);
        break;
      case greenRec:
        RGB(0, 255, 0);
        break;
      case yellowRec:
        RGB(255, 0, 255);
        break;
      case cyanRec:
        RGB(0, 255, 255);
        break;
      case purpleRec:
        RGB(255, 255, 0);
        break;
      default:
        Serial.println("Undefined Record");
        break;
    }
    /** voice recognized */
    printVR(buf);
  }

}

/**
   @brief   Values for RGB LED Common Anode
   @param   red   --> Red value (0-255)
   @param   green --> Green value (0-255)
   @param   blue  --> Blue value (0-255)
*/
void RGB(int red, int green, int blue)
{
  //For common anode, must send "negative" volatge so 255 - value is needed
  analogWrite(led_R, (255 - red));
  analogWrite(led_G, (255 - green));
  analogWrite(led_B, (255 - blue));
 // readAnalogVal();
}

void readAnalogVal()
{
  Serial.print("\nR\tG\tB\n");
  Serial.print(analogRead(led_R)/4);
  Serial.print("\t");
  Serial.print(analogRead(led_G)/4);
  Serial.print("\t");
  Serial.print(analogRead(led_B)/4);
  Serial.println(" ");
}
