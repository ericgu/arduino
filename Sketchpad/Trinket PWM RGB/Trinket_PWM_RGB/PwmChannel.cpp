#include <Arduino.h>
#include "PwmChannel.h"

#define LEVELS 64

PwmChannel::PwmChannel(byte pin)
{
  m_pin = pin;
  m_currentValue = 0;
  m_cycle = 0;
  m_currentStep = 0;
  m_increment = 0;
  
  //m_currentStep = 500;
  //m_increment = 15;
  
  pinMode(pin, OUTPUT);
}

void DumpByte(byte value)
{
    digitalWrite(3, HIGH);
    digitalWrite(3, LOW);
    for (byte j = 0; j < 8; j++)
    {
      digitalWrite(3, value & 0x80);
      value = value << 1;
    }
    digitalWrite(3, HIGH); 
}

void PwmChannel::OnTimerTick()
{
  m_cycle = (m_cycle + 1) % LEVELS;
  
  if (m_cycle == 0)
  {
    NextStep();
    digitalWrite(m_pin, HIGH); 
#if debugging
    byte temp = m_currentValue >> 8;
    //temp = m_targetValue >> 8;
    temp = m_currentStep & 0xFF;
    DumpByte(temp);
#endif
  }

  if (m_cycle == m_currentValue >> 8)
  {
    digitalWrite(m_pin, LOW);
  }
}

void PwmChannel::SetTarget(byte targetValue, unsigned short steps)
{
  m_targetValue = targetValue << 8;
  m_currentStep = steps + 1;
  
  if (m_targetValue >= m_currentValue)
  {
    m_increment = (m_targetValue - m_currentValue) / steps;
  }
  else
  {
    m_increment = ~((m_currentValue - m_targetValue) / steps) + 1; 
  }

  NextStep();
}

bool PwmChannel::GetIsAtTarget()
{
    return m_currentStep == 0;
}

void PwmChannel::NextStep()
{
    if (m_currentStep > 0)
    {
      m_currentStep--;
    }
    
    if (m_currentStep == 0)
    {
//        m_currentStep = 500;
//        m_increment = 15;
//        m_currentValue = 0;

        m_currentValue = m_targetValue;
    }
    else
    {
        m_currentValue += m_increment;
    }
}

