#include <Arduino.h>
#include <AccelStepper.h>
#include "Constants.h"
#include "DriverData.h"
#include "CommandDispatch.h"

  CommandDispatch::CommandDispatch(DriverData* pDriverData)
  {
    m_pDriverData = pDriverData;
    m_pCommandCurrentChar = m_pCommand;
  }

  long last;
  bool CommandDispatch::ProcessSerial()
  {
    int charValue;
    bool result = false;
 
 
    if (Serial.available())
    {
      charValue = Serial.read();

      if (charValue == '#')
      {
        result = true;
      
        *m_pCommandCurrentChar = 0;
        DispatchCommand();
        m_pCommandCurrentChar = m_pCommand;
      }
      else if (charValue >= 32)
      {
        *m_pCommandCurrentChar = (char) charValue;
        m_pCommandCurrentChar++;

      //Serial.println((int) charValue);
      //Serial.println((char) charValue);
      }
    }
    
    return result;
  }

  char* CommandDispatch::readValue(char* pString, int* pValue)
  {
    *pValue = 0;
    char negative = 0;
    
    if (*pString == '-')
    {
      negative = true;
      pString++;
    }

    while (true)
    {
      if (*pString >= '0' && *pString <= '9')
      {
        *pValue = *pValue * 10 + *pString - '0';
        pString++;
      }
      else
      {
        if (negative)
        {
          *pValue = -*pValue;
        }

        return pString;
      }
    }
  }
  
  char* CommandDispatch::skipComma(char* pString)
  {
    if (*pString == ',')
    {
      pString++;
    }
    
    return pString; 
  }
  
  void CommandDispatch::DispatchCommand()
  {
    char* pCurrent;
    int steps;
    int leftRight;
    int frontBack;
    
    Serial.print("D: ");
    Serial.println(*m_pCommand);

    switch (*m_pCommand)
    {
      case '?':
        Serial.println("RoboHockey V1.0");
        break;

      case 'P':        // full position
        digitalWrite(7, LOW);
        pCurrent = readValue(m_pCommand + 1, &leftRight);
        pCurrent = skipComma(pCurrent);
        readValue(pCurrent, &frontBack);
        m_pDriverData->m_pStepper1->moveTo(frontBack);
        //
        m_pDriverData->m_pStepper2->moveTo(leftRight);
        break;

      case 'F':        // forward
        digitalWrite(7, LOW);
        readValue(m_pCommand + 1, &steps);
        m_pDriverData->m_pStepper1->move(steps);

        //Serial.print("Forward: ");
        //Serial.println(steps);
        break;

      case 'B':        // backwards
        digitalWrite(7, HIGH);
        readValue(m_pCommand + 1, &steps);
        m_pDriverData->m_pStepper1->move(-steps);

        //Serial.print("Backward: ");
        //Serial.println(steps);
        break;

      case 'L':        // forward
        digitalWrite(11, HIGH);
        readValue(m_pCommand + 1, &steps);
        m_pDriverData->m_pStepper2->move(-steps);

        //Serial.print("Left: ");
        //Serial.println(steps);
        break;

      case 'R':        // backwards
        digitalWrite(11, LOW);
        readValue(m_pCommand + 1, &steps);
        m_pDriverData->m_pStepper2->move(steps);

        //Serial.print("Right: ");
        //Serial.println(steps);
        break;
        
      case 'Z':      // Laser
        readValue(m_pCommand + 1, &steps);
        if (steps == 1)
        {
          digitalWrite(A0, HIGH);
        }
        else
        {
          digitalWrite(A0, LOW);
        }
    }
  }


