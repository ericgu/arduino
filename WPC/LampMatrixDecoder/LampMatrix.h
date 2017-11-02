typedef unsigned char byte;

class LampMatrix
{
  private:
    char m_lampState[64];
    char itemString[65];

  public:
    void DecodeRowAndColumnValues(byte rowVector, byte columnVector)
    {
        for (byte rowIndex = 0; rowIndex < 8; rowIndex++)
        {
            byte columnVectorTemp = columnVector;
            for (byte columnIndex = 0; columnIndex < 8; columnIndex++)
            {
                byte lampIndex = (byte)(columnIndex * 8 + rowIndex);

                if ((rowVector & 0x01) == 0 &&     // row is pulled low, therefore grounded...
                    (columnVectorTemp & 0x01) == 0)    // column is pulled high, therefore 18V
                {
                    m_lampState[lampIndex]++;
                }

                columnVectorTemp = (byte)(columnVectorTemp >> 1);
            }
            rowVector = (byte)(rowVector >> 1);
        }
    }

    void Clear()
    {
        for (byte i = 0; i < 64; i++)
        {
            m_lampState[i] = 0;
        }
    }

    char* GetStateString()
    {
        for (byte i = 0; i < 64; i++)
        {
            itemString[i] = (char)(m_lampState[i] + '0');
            m_lampState[i] = 0;
        }
        itemString[64] = '\0';

        return itemString;
    }
};
