class Bouncer
{
  private:
    short minValue;
    short maxValue;
  
  public:
    short currentValue;
    short increment;

    Bouncer(short _minValue, short _maxValue, short _currentValue)
    {
      minValue = _minValue;
      maxValue = _maxValue; 
      currentValue = _currentValue + _minValue;
      increment = 1;
    }
    
    void Step()
    {
      currentValue += increment;
    
      if (currentValue == maxValue || currentValue == minValue)
      {
        increment = -increment;  
      }  
    }
};
