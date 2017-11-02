#include <Arduino.h>
#include <LPD8806.h>
#include <SPI.h>
#include "Constants.h"
#include "DumboData.h"
#include "CommandDispatch.h"
#include "DisplayDriver.h"
#include "LedStrip.h"
#include "ElapsedTime.h"


DumboData dumboData;
CommandDispatch commandDispatch(&dumboData);
ElapsedTime elapsedTime(&dumboData);
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
  
  LedStrip::clearStrip(20);
  
  initTimer2();
  elapsedTime.UpdateMinuteCounter(false);
}

void loop() 
{
  if (commandDispatch.ProcessSerial())
  {
    commandReceived = true;  
  }
}

ISR(TIMER2_OVF_vect)
{
  TCNT2=0x06;
  digitalWrite(4, HIGH);

  dumboData.IncrementDisplayTickCount();
  elapsedTime.MillisecondTick();

  if (commandReceived)
  {
    commandReceived = false;
    elapsedTime.ResetMinuteCounter();
  }
  
  LedStrip::UpdateLedStrip(dumboData);

  int value = dumboData.GetCurrentItemValue();

  DisplayDriver::showCurrentValue(value, dumboData.m_dimLevel);

  digitalWrite(4, LOW);
};



