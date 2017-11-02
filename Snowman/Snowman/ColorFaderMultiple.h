class ColorFaderMultiple: public ColorDecider
{
  private:
    int m_currentTargetSaved;

    Color *m_pTargets;
    int m_targetCount;
    int m_currentTarget;
    int m_steps;
    ColorFader m_fader;

  public:
    ColorFaderMultiple()
    {
        m_currentTarget = -1;
    }

    virtual void Push()
    {
      m_currentTargetSaved = m_currentTarget;
      m_fader.Push();
    }

    void Pop()
    {
      m_currentTarget = m_currentTargetSaved;
      m_fader.Pop();
    }

    virtual Color GetNextColor()
    {
        if (!m_fader.GetInFade())
        {
            m_currentTarget++;
            if (m_currentTarget == m_targetCount)
            {
                m_currentTarget = 0;
            }
/*
            Serial.print("CurrentTarget = "); Serial.println(m_currentTarget);
            Serial.print("Target r = "); Serial.println(m_pTargets[m_currentTarget].red);
            Serial.print("Target g = "); Serial.println(m_pTargets[m_currentTarget].green);
            Serial.print("Target b = "); Serial.println(m_pTargets[m_currentTarget].blue);
*/
            m_fader.SetTarget(m_pTargets[m_currentTarget], m_steps);
        }

        return m_fader.GetNextColor();
    }

    virtual bool GetInFade()
    {
        return m_fader.GetInFade() || m_currentTarget != m_targetCount - 1;
    }

    void SetFade(Color *pTargets, int targetCount, int steps)
    {
        m_pTargets = pTargets;
        m_targetCount = targetCount;
        m_steps = steps;
/*
            Serial.print("CurrentTarget = "); Serial.println(m_currentTarget);
        for (int i = 0; i < targetCount; i++)
        {
            Serial.print("Target r = "); Serial.println(m_pTargets[i].red);
            Serial.print("Target g = "); Serial.println(m_pTargets[i].green);
            Serial.print("Target b = "); Serial.println(m_pTargets[i].blue);
        }
*/
    }
};
