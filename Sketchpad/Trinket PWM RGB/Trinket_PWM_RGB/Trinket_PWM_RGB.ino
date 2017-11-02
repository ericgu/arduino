 #include <avr/power.h>
 #include "Pwm.h"

Pwm pwm;

int debugPin = 2;
 
void setup() {
  if (F_CPU == 16000000) clock_prescale_set(clock_div_1); // 16MHz
   
  // initialize timer1 
  noInterrupts();           // disable all interrupts  
  
  CLKPR=0x80;
  CLKPR=0x00;  
  
  PLLCSR=0x00;

  TCCR1=0x04;
  GTCCR=0x00;
  TCNT1=0x00;
  OCR1A=0x00;
  OCR1B=0x00;
  OCR1C=0x00;
  // Timer(s)/Counter(s) Interrupt(s) initialization
  TIMSK=0x04;
  
  interrupts();  
  pinMode(0, OUTPUT);
  pinMode(1, OUTPUT);
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
}

unsigned short animationCounter = 0;

ISR(TIMER1_OVF_vect)        // interrupt service routine that wraps a user defined function supplied by attachInterrupt
{
    digitalWrite(debugPin, HIGH);
    pwm.OnIntervalTick();
  
    digitalWrite(debugPin, LOW);
} 
 
// the loop routine runs over and over again forever:
void loop() 
{
}
