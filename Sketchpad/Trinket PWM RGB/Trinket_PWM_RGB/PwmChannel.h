#include <Arduino.h>

class PwmChannel
{
    public:
        PwmChannel(byte pin);
  
        void OnTimerTick();
        void SetTarget(byte targetValue, unsigned short steps);
        bool GetIsAtTarget();
        void NextStep();
       
    private:
        unsigned short m_currentValue;
        unsigned short m_currentStep;
        unsigned short m_targetValue;
        unsigned short m_increment;
        byte m_pin;
        byte m_cycle; 
};
