class CommandDispatch
{
public:
  DriverData* m_pDriverData;

  char m_pCommand[64];
  char* m_pCommandCurrentChar;

  CommandDispatch(DriverData* pDriverData);

  bool ProcessSerial();

  char* readValue(char* pString, int* pValue);
  char* skipComma(char* pString);

  void DispatchCommand();
};


