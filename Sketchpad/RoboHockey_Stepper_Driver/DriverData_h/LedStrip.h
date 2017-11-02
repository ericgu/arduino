class LedStrip
{
  public: 
    static void WriteByteToStrip(byte value);
    static void UpdateLedStrip(DumboData& dumboData);
    static void clearStrip(int ledCount);
};

