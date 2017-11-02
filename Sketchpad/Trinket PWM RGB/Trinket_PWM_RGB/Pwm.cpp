
#include <Arduino.h>
#include "Pwm.h"
#include "RgbState.h"

#define FadeTime 3000
#define WaitTime 1000

#define AnimationCount 14
RgbState animations[AnimationCount] = {
  #if george
  { 63,  63,  63, FadeTime},
  { 63,  63,  63, WaitTime},
  {  0,   0,   0, FadeTime},
  {  0,   0,   0, WaitTime},
  { 63,  63,  63, FadeTime},
  { 63,  63,  63, WaitTime},
  {  0,   0,   0, FadeTime},
  {  0,   0,   0, WaitTime},
  { 63,  63,  63, FadeTime},
  { 63,  63,  63, WaitTime},
  {  0,   0,   0, FadeTime},
  {  0,   0,   0, WaitTime},
  { 63,  63,  63, FadeTime},
  { 63,  63,  63, WaitTime},
  {  0,   0,   0, FadeTime},
  {  0,   0,   0, WaitTime},
  { 63,  63,  63, FadeTime},
  { 63,  63,  63, WaitTime},
  {  0,   0,   0, FadeTime},
  {  0,   0,   0, WaitTime},
  { 63,  63,  63, FadeTime},
  { 63,  63,  63, WaitTime},
  {  0,   0,   0, FadeTime},
  {  0,   0,   0, WaitTime}
#else
  { 63,   0,   0, FadeTime},
  { 63,   0,   0, WaitTime},
  {  0,  63,   0, FadeTime},
  {  0,  63,   0, WaitTime},
  {  0,   0,  63, FadeTime},
  {  0,   0,  63, WaitTime},
  { 63,  63,   0, FadeTime},
  { 63,  63,   0, WaitTime},
  { 63,   0,  63, FadeTime},
  { 63,   0,  63, WaitTime},
  {  0,  63,  63, FadeTime},
  {  0,  63,  63, WaitTime},
  { 63,  63,  63, FadeTime},
  { 63,  63,  63, WaitTime},
#endif
};

Pwm::Pwm()
{
   m_currentAnimationIndex = 0;
  
   m_pwmChannelRed = new PwmChannel(0);
   m_pwmChannelGreen = new PwmChannel(1);
   m_pwmChannelBlue = new PwmChannel(4);
   
    RgbState* pAnimation = animations + 1;
   //m_pwmChannelRed->SetTarget(50, 1000);
   //m_pwmChannelRed->SetTarget(pAnimation->red, pAnimation->count);
}

void Pwm::OnIntervalTick()
{
  if (m_pwmChannelRed->GetIsAtTarget())
  {
    RgbState* pAnimation = animations + m_currentAnimationIndex;
    m_currentAnimationIndex = (m_currentAnimationIndex + 1) % AnimationCount;
   
    m_pwmChannelRed->SetTarget(pAnimation->red, pAnimation->count);
    m_pwmChannelGreen->SetTarget(pAnimation->green, pAnimation->count);
    m_pwmChannelBlue->SetTarget(pAnimation->blue, pAnimation->count);
  }

  m_pwmChannelRed->OnTimerTick();       
  m_pwmChannelBlue->OnTimerTick();       
  m_pwmChannelGreen->OnTimerTick();       
 }

