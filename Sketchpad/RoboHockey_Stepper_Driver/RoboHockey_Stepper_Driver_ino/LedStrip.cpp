#include <ARDUINO.H>
#include <AccelStepper.h>
#include "Constants.h"
#include "DriverData.h"
#include "LedStrip.h"

const int DataPin = 3;
const int ClockPin = 2;

void LedStrip::WriteByteToStrip(byte value)
{
  for (uint8_t bit=0x80; bit; bit >>= 1) 
  {
    if (value & bit) 
    {
      digitalWrite(DataPin, HIGH);
    }
    else
    {
      digitalWrite(DataPin, LOW);
    }
    digitalWrite(ClockPin, HIGH);
    digitalWrite(ClockPin, LOW);
  }  
}

static const int FlashSlowCountMax = 1200;
static const int FlashFastCountMax = 400;

void LedStrip::UpdateLedStrip(DriverData& driverData)
{
  static int ledToUpdate = 0;
  static int flashSlowCounter = 0;
  static int flashFastCounter = 0;
#ifdef fred
  int dimAmount = dumboData.m_stripDimBase;
  if (ledToUpdate == dumboData.m_currentItem)
  {
     if (dumboData.m_itemTickCount < dumboData.m_stripChangeBlankingCount)
     {
       dimAmount = 8;   // off
     }
     else
     {
       dimAmount = dumboData.m_stripDimBright;
     }
  }

  DumboItem* pDumboItem = &dumboData.m_items[ledToUpdate];
  
  flashSlowCounter = (flashSlowCounter + 1) % FlashSlowCountMax;

  if (flashSlowCounter < (FlashSlowCountMax / 4))
  {
    if (pDumboItem->m_flash == DumboItem::FlashSlow)
    {
      dimAmount++;
    }
  }
  
  flashFastCounter = (flashFastCounter + 1) % FlashFastCountMax;

  if (flashFastCounter < (FlashFastCountMax / 4))
  {
    if (pDumboItem->m_flash == DumboItem::FlashFast)
    {
      dimAmount++;
    }
  }
  
  LedStrip::WriteByteToStrip((pDumboItem->m_green >> dimAmount) | 0x80);
  LedStrip::WriteByteToStrip((pDumboItem->m_red >> dimAmount) | 0x80);
  LedStrip::WriteByteToStrip((pDumboItem->m_blue >> dimAmount) | 0x80);
  
  ledToUpdate++;
  
  if (ledToUpdate == dumboData.m_itemCount)
  {
    LedStrip::WriteByteToStrip(0);
    ledToUpdate = 0; 
  }
  #endif
}  

void LedStrip::clearStrip(int ledCount)
{
  for (int i = 0; i < ledCount; i++)
  {
    LedStrip::WriteByteToStrip(0x80);
    LedStrip::WriteByteToStrip(0x80);
    LedStrip::WriteByteToStrip(0x80);
  }
  
  LedStrip::WriteByteToStrip(0);
}

