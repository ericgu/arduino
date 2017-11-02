class LedStrip
{
  public: 
    static void WriteByteToStrip(byte value);
    static void UpdateLedStrip(DriverData& driverData);
    static void clearStrip(int ledCount);
};

