class ColorSteady: public ColorDecider
{
private:
  int m_current;
  int   m_start;

public:
  void SetTarget(Color targetColor)
  {
    m_current = 0;
    m_start = 0;
    //m_current = targetColor;
  }

  void IncreaseStart()
  {
    m_start += 3;
    m_current = m_start;
  }

  virtual Color GetNextColor()
  {
    m_current--;
    int value = m_current;
    if (value > 255)
    {
      value = 255;
    }
    else if (value < 0)
    {
      value = 0;
    }
   
    return Color(value, value, value);
  }

  virtual bool GetInFade()
  {
    return false;
  }
  
  virtual void Push() {}
};
