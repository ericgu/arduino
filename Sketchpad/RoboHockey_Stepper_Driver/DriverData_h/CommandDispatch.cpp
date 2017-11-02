#include <Arduino.h>
#include "Constants.h"
#include "DumboData.h"
#include "CommandDispatch.h"

  CommandDispatch::CommandDispatch(DumboData* pDumboData)
  {
    m_pDumboData = pDumboData;
    m_pCommandCurrentChar = m_pCommand;
  }

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
        Serial.println("");
        DispatchCommand();
        m_pCommandCurrentChar = m_pCommand;
      }
      else
      {
        *m_pCommandCurrentChar = (char) charValue;
        m_pCommandCurrentChar++;
      }
      Serial.print((char) charValue);
    }
    
    return result;
  }

  char* CommandDispatch::readValue(char* pString, int* pValue)
  {
    *pValue = 0;

    while (true)
    {
      if (*pString >= '0' && *pString <= '9')
      {
        *pValue = *pValue * 10 + *pString - '0';
        pString++;
      }
      else
      {
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


    // Command Syntax
    // ?
    //		Returns "DUMBO v0.x
    // Sn
    //		Sets the number of values to n
    // In
    //		Sets the update wait count to n
    // Dl,d,b
    //		Sets the dimming levels:
    //                  l - dim level of LEDs
    //                  d - dim level of the strip
    //                  b - bright level of the strip
    //
    // Bc
    //          Sets the blanking interval when the current item changes. 
    //
    // Vn,v,r,g,b,n
    //		Sets value of entry:
    //			n - number of the entry
    //			v - LED value
    //			r - red value of status LED (0-127)
    //			g - red value of status LED (0-127)
    //			b - red value of status LED (0-127)
    //                  n - if present, flag:
    //                      M = this is the "minutes since last update" entry
    //                      F = fast pulse
    //                      S = slow pulse
  
  void CommandDispatch::DispatchCommand()
  {
    char* pCurrent;

    switch (*m_pCommand)
    {
      case '?':
        Serial.println("DUMBo V1");
        break;

      case 'S':
        readValue(m_pCommand + 1, &m_pDumboData->m_itemCount);
        m_pDumboData->Init();
        Serial.print("Items: ");
        Serial.print(m_pDumboData->m_itemCount);
        break;

      case 'I':
        readValue(m_pCommand + 1, &m_pDumboData->m_timeBetweenItems);
        Serial.print("Wait count: ");
        Serial.print(m_pDumboData->m_timeBetweenItems);
        break;

      case 'B':
        readValue(m_pCommand + 1, &m_pDumboData->m_stripChangeBlankingCount);
        Serial.print("Blanking: ");
        Serial.print(m_pDumboData->m_stripChangeBlankingCount);
        break;

      case 'D':
        pCurrent = readValue(m_pCommand + 1, &m_pDumboData->m_dimLevel);
        pCurrent = skipComma(pCurrent);

        pCurrent = readValue(pCurrent, &m_pDumboData->m_stripDimBase);
        pCurrent = skipComma(pCurrent);

        readValue(pCurrent, &m_pDumboData->m_stripDimBright);
        Serial.print("Dim level (led, strip dim, strip bright):");
        Serial.print(m_pDumboData->m_dimLevel);
        Serial.print(", ");
        Serial.print(m_pDumboData->m_stripDimBase);
        Serial.print(", ");
        Serial.print(m_pDumboData->m_stripDimBright);
        break;

      case 'V':
        int index;

        pCurrent = readValue(m_pCommand + 1, &index);
        DumboItem* pCurrentItem = &m_pDumboData->m_items[index];
        pCurrent = skipComma(pCurrent);

        pCurrent = readValue(pCurrent, &(pCurrentItem->m_displayValue));
        pCurrent = skipComma(pCurrent);

        pCurrent = readValue(pCurrent, &(pCurrentItem->m_red));
        pCurrent = skipComma(pCurrent);

        pCurrent = readValue(pCurrent, &(pCurrentItem->m_green));
        pCurrent = skipComma(pCurrent);

        pCurrent = readValue(pCurrent, &(pCurrentItem->m_blue));
        pCurrent = skipComma(pCurrent);
        
        pCurrentItem->m_flash = DumboItem::FlashNone;
        
        switch (*pCurrent)
        {
          case 'M':
            m_pDumboData->m_lastUpdateIndex = index;
            break;

          case 'F':
            pCurrentItem->m_flash = DumboItem::FlashFast;
            break;

          case 'S':
            pCurrentItem->m_flash = DumboItem::FlashSlow;
            break;
        }

        Serial.print("Line: ");
        Serial.print(index);
        Serial.print(", ");
        Serial.print(pCurrentItem->m_displayValue);
        Serial.print(", ");
        Serial.print(pCurrentItem->m_red);
        Serial.print(", ");
        Serial.print(pCurrentItem->m_green);
        Serial.print(", ");
        Serial.print(pCurrentItem->m_blue);
        Serial.println("");
        break;
    }
  }



