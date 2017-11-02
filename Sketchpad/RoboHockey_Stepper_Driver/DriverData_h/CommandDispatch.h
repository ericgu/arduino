class CommandDispatch
{
public:
  DumboData* m_pDumboData;

  char m_pCommand[64];
  char* m_pCommandCurrentChar;

  CommandDispatch(DumboData* pDumboData);

  bool ProcessSerial();

  char* readValue(char* pString, int* pValue);
  char* skipComma(char* pString);
  
    // Command Syntax
    // All commands terminate with a #
    // ?
    //		Returns "DUMBO v0.x
    // Sn
    //		Sets the number of values to n
    // In
    //		Sets the update wait count to n
    // Vn,v,r,g,b
    //		Sets value of entry:
    //			n - number of the entry
    //			v - LED value
    //			r - red value of status LED (0-127)
    //			g - red value of status LED (0-127)
    //			b - red value of status LED (0-127)
    // 

  void DispatchCommand();
};


