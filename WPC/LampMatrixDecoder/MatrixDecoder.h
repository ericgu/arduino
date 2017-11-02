#include "Arduino.h"
#include "LampMatrix.h"

class MatrixDecoder
{
  private:
    LampMatrix m_lampMatrix;
    byte m_strobeCount;
    char m_lampStateString[65];
    byte m_lampStateStringDataReady;

  public:
    void DecodeRowAndColumnData(byte row, byte column)
    {
        m_lampMatrix.DecodeRowAndColumnValues(row, column);

        m_strobeCount++;
        if (m_strobeCount == 128)
        {
           memcpy(m_lampStateString, m_lampMatrix.GetStateString(), 65);
           m_lampStateStringDataReady = 1;
          
           m_strobeCount = 0; 
        }
    }

    char* GetLampStateString()
    {
        if (m_lampStateStringDataReady == 1)
        {
           m_lampStateStringDataReady = 0;
           return m_lampStateString;
        }
        else
        {
          return 0;
        }
    }
};

