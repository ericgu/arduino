#include <Arduino.h>
#include "PwmChannel.h"

class Pwm
{
    public:
        Pwm();
  
        void OnIntervalTick();
        
    private:
        PwmChannel* m_pwmChannelRed;
        PwmChannel* m_pwmChannelBlue;
        PwmChannel* m_pwmChannelGreen;
        
        byte m_currentAnimationIndex;
};
