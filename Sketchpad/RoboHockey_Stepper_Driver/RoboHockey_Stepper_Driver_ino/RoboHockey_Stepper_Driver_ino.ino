#include <AccelStepper.h>

#include <Arduino.h>
#include <LPD8806.h>
#include <SPI.h>
#include "Constants.h"
#include "DriverData.h"
#include "CommandDispatch.h"
//#include "DisplayDriver.h"
#include "LedStrip.h"

DriverData driverData;
CommandDispatch commandDispatch(&driverData);
//ElapsedTime elapsedTime(&dumboData);
bool commandReceived = false;

void initTimer2()
{
  // Timer/Counter 2 initialization
  // Clock source: System Clock
  // Clock value: 62.500 kHz
  // Mode: Normal top=FFh
  // OC2A output: Disconnected
  // OC2B output: Disconnected
  ASSR=0x00;
  TCCR2A=0x00;
  TCCR2B=0x04;
  TCNT2=0x06;
  OCR2A=0x20;
  OCR2B=0x00;
  TIMSK2=0x01;

  interrupts();    
}

void setup()
{
  for (int i = 2; i <= 13; i++)
  {
    pinMode(i, OUTPUT);
    digitalWrite(i, LOW);
  }
  
  for (int i = A0; i <= A3; i++)
  {
    pinMode(i, OUTPUT); 
  }
  
  Serial.begin(9600);
  
  //LedStrip::clearStrip(20);
  
//  initTimer2();
  //elapsedTime.UpdateMinuteCounter(false);
}

int countA = 0;
int countB = 0;

int freeRam () 
{
  extern int __heap_start, *__brkval; 
  int v; 
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval); 
}

void loop() 
{
  countA++;
  if (countA % 10000 == 0)
  {
    countB++;

    if (countB % 5 == 0)
    {
      //Serial.print(countB);
      //Serial.print(" ");
    }
  }
  //Serial.print("1");
  if (commandDispatch.ProcessSerial())
  {
    commandReceived = true;  
  }
  //Serial.print("2");
  driverData.m_pStepper1->run();
  //Serial.print("3");
  driverData.m_pStepper2->run();
  //Serial.print("4");
}



