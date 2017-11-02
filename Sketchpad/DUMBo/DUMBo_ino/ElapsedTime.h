class ElapsedTime
{
public:
  int m_millisecondCount;
  int m_secondCount;
  DumboData* m_pDumboData;

  ElapsedTime(DumboData* pDumboData)
  {
    m_pDumboData = pDumboData;
    m_millisecondCount = 0;
    m_secondCount = 0;
  }

  // Every minute we need to update the appropriate
  // value in the item that is displaying the time since
  // the last update.
  void MillisecondTick()
  {
    m_millisecondCount++;
    if (m_millisecondCount == 1000)
    {
      m_secondCount++;
      if (m_secondCount == 60)
      {
        m_secondCount = 0;
        UpdateMinuteCounter(true);   
      }
     
      m_millisecondCount = 0; 
    }
  }

  void SetMinuteCounterValue(DumboItem* pDumboItem, bool incrementValue)
  {
    if (incrementValue)
    {
      if (pDumboItem->m_displayValue < 9999)
      {
        pDumboItem->m_displayValue++;
      }
    }
    else
    {
      pDumboItem->m_displayValue = 0; 
    }
  
    pDumboItem->m_blue = 0;    

    // 00-30 blend green to yellow
    // 30-60 blend yellow to red

    if (pDumboItem->m_displayValue < 30)
    {
      pDumboItem->m_red = (pDumboItem->m_displayValue * 127) / 30;
      pDumboItem->m_green = 127;
    }
    else if (pDumboItem->m_displayValue < 60)
    {
      int offset = pDumboItem->m_displayValue - 30;
    
      pDumboItem->m_green = 127 - (offset * 127) / 30;
      pDumboItem->m_red = 127;
    }  
    else
    {
      pDumboItem->m_green = 0;
      pDumboItem->m_red = 127;
    }
  }

  void UpdateMinuteCounter(bool incrementValue)
  {
    if (m_pDumboData->m_lastUpdateIndex != -1)
    {
      DumboItem* pDumboItem = &m_pDumboData->m_items[m_pDumboData->m_lastUpdateIndex];
    
      SetMinuteCounterValue(pDumboItem, incrementValue);
    }  
  }
  
  void ResetMinuteCounter()
  {
    UpdateMinuteCounter(false);
  }
};

