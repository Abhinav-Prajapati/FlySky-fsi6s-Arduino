#include <Arduino.h>
#include <Servo.h>
#include <IBusBM.h>

#define sw_a_1  7

#define sw_a_2  8

#define sw_b_1_1  2
#define sw_b_1_2  10
#define sw_b_1_3  11

#define sw_b_2_1  12
#define sw_b_2_2  13
#define sw_b_2_3  14

IBusBM ibusRc;

Servo Servo_black;
Servo Servo_black_2;

HardwareSerial &ibusRcSerial = Serial1;
HardwareSerial &debugSerial = Serial;

void setup()
{

  debugSerial.begin(9600);
  Servo_black.attach(9);
  // Servo_black.attach(10);
  ibusRc.begin(ibusRcSerial);

  pinMode(sw_b_1_1, OUTPUT);
  pinMode(sw_b_1_2, OUTPUT);
  pinMode(sw_b_1_3, OUTPUT);
}

// Read the number of a given channel and convert to the range provided.
// If the channel is off, return the default value
int readChannel(byte channelInput, int minLimit, int maxLimit, int defaultValue)
{
  uint16_t ch = ibusRc.readChannel(channelInput);
  if (ch < 100)
    return defaultValue;
  return map(ch, 1000, 2000, minLimit, maxLimit);
}

// Red the channel and return a boolean value
bool redSwitch(byte channelInput, bool defaultValue)
{
  int intDefaultValue = (defaultValue) ? 100 : 0;
  int ch = readChannel(channelInput, 0, 100, intDefaultValue);
  return (ch > 50);
}

void loop()
{
  for (byte i = 0; i < 10; i++)
  {
    int value = readChannel(i, 0, 1023, 0);
    char msg[40];
    debugSerial.print("Ch");
    sprintf(msg, "%-4d", value);
    debugSerial.print(i + 1);
    debugSerial.print(": ");
    debugSerial.print(msg);
    debugSerial.print(" ");

  }
  debugSerial.print("Ch5: ");
  debugSerial.print(redSwitch(4, false));
  debugSerial.println();

  int servo_pos = readChannel(2, 0, 180, 0);
  // int servo_pos_2 = readChannel(3, 0, 180, 0);

  int sw_b_1 = readChannel(6, 0, 3, 0);
  if( sw_b_1==0){
    digitalWrite(sw_b_1_1, HIGH);
    digitalWrite(sw_b_1_2, LOW);
    digitalWrite(sw_b_1_3, LOW);
  }
  else if (sw_b_1==1)
  {
    digitalWrite(sw_b_1_1, LOW);
    digitalWrite(sw_b_1_2, HIGH);
    digitalWrite(sw_b_1_3, LOW);
  }
  else{

    digitalWrite(sw_b_1_1, LOW);
    digitalWrite(sw_b_1_2, LOW);
    digitalWrite(sw_b_1_3, HIGH);
  }
  

  Servo_black.write(servo_pos);
  // Servo_black_2.write(servo_pos_2);

  delay(1);
}