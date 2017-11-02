class DumboItem
{
public:
  static const int FlashNone = 0;
  static const int FlashSlow = 1;
  static const int FlashFast = 2;

  int m_displayValue;
  int m_red;
  int m_green;
  int m_blue;
  int m_flash;
  
  DumboItem()
  {
    m_displayValue = 1;
    m_red = 0;
    m_green = 0;
    m_blue = 0;
    m_flash = FlashNone; 
  }
};

class DumboData
{
public:
  bool m_active;
  int m_itemCount;            // # of metric values to display
  int m_itemTickCount;
  int m_timeBetweenItems;     // the time to wait between switching items, in mS
  int m_dimLevel;             // the amount to dim
  int m_currentItem;          // index of the current item
  int m_lastUpdateIndex;      // index of the item that is showing the minutes since the last update
  int m_stripDimBase;         // the base dimming level for the strip
  int m_stripDimBright;       // the bright dimming level for the strip
  int m_stripChangeBlankingCount;   // Count of how long the strip blanks when current item changes
	
  DumboItem m_items[MaxItems];

  void SetItem(int index, int value, int red, int green, int blue)
  {
    m_items[index].m_displayValue = value;
    m_items[index].m_red = red;
    m_items[index].m_green = green;
    m_items[index].m_blue = blue;
  }

  DumboData()
  {
    m_active = 0;     
    m_itemTickCount = 0;    
    m_timeBetweenItems = 1000;
    m_dimLevel = 0;
    m_lastUpdateIndex = -1;
    m_currentItem = 0;
    m_stripDimBase = 3;
    m_stripDimBright = 2;
    m_stripChangeBlankingCount = 50;

    for (int i = 0; i < MaxItems; i++)
    {
      SetItem(i, 0, 0, 0, 0);
    }

    m_itemCount = 8;
    
    SetItem(0, 1234, 127,   0,   0);
    SetItem(1, 9999,   0, 127,   0);
    SetItem(2,   12,   0,   0, 127);
    SetItem(3,   34, 127, 127,   0);
    SetItem(4,  505, 127,   0, 127);
    SetItem(5,  451,   0, 127, 127);
    SetItem(6,  404, 127, 127, 127);
    SetItem(7,    0,   0,   0,   0);
    
    m_items[1].m_flash = DumboItem::FlashFast;
    m_items[2].m_flash = DumboItem::FlashSlow;
  }
  
  void Init()
  {
    m_currentItem = 0;
    m_active = 1;
  }
  
  int GetCurrentItemValue()
  {
    return m_items[m_currentItem].m_displayValue;
  }
  
  void MoveToNextItem()
  {
    m_currentItem++;
  
    if (m_currentItem >= m_itemCount)
    {
      m_currentItem = 0; 
    }    
  }
  
  void IncrementDisplayTickCount()
  {
    m_itemTickCount = (m_itemTickCount + 1) % m_timeBetweenItems;

    if (m_itemTickCount == 0)
    {
      MoveToNextItem();
    }    
  }
};

