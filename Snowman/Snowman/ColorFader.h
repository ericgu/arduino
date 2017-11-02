class ColorFader: ColorDecider
{
private:
  Color m_currentSaved;
  Color m_deltaSaved;
  int m_stepsRemainingSaved;

  Color m_current;
  Color m_delta;
  int m_stepsRemaining;

  void Step()
  {
    if (m_stepsRemaining != 0)
    {
      m_current.red += m_delta.red;
      m_current.green += m_delta.green;
      m_current.blue += m_delta.blue;

      m_stepsRemaining--;
    }
  }

  unsigned short GetDelta(unsigned short current, unsigned short target, unsigned short steps)
  {
    if (current <= target)
    {
      return (target - current) / steps;
    }
    else
    {
      return ~((current - target) / steps) + 1;
    }
  }

public:
  void SetTarget(Color targetColor, int steps)
  {
     Color targetExtended = targetColor.ShiftUp();

     m_delta = Color(
             GetDelta(m_current.red, targetExtended.red, steps),
             GetDelta(m_current.green, targetExtended.green, steps),
             GetDelta(m_current.blue, targetExtended.blue, steps));

      m_stepsRemaining = steps;
  }

  void Push()
  {
    m_stepsRemainingSaved = m_stepsRemaining;
    m_currentSaved = m_current;
    m_deltaSaved = m_delta; 
  }
  
  void Pop()
  {
    m_stepsRemaining = m_stepsRemainingSaved;
    m_current = m_currentSaved;
    m_delta = m_deltaSaved;
  }    
  
  virtual Color GetNextColor()
  {
    Step();
    return m_current.ShiftDown();
  }

  virtual bool GetInFade()
  {
    return m_stepsRemaining != 0;
  }
};
