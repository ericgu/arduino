#include <pins_arduino.h>
#include <SPI.h>

byte* buffer;

void Init(int ledCount)
{
  buffer = (byte*) malloc(ledCount * 3 + 1);
  memset(buffer, ledCount * 3 + 1, 0);
  buffer[ledCount * 3] = 0;  // end-of-message 
}

void setup() 
{
  SPI.begin();
  SPI.setClockDivider(SPI_CLOCK_DIV2);

  Serial.begin(9600);
  
  Init(32);
  
}

int value;

void writeBuffer()
{
  byte* pCurrent = buffer;
  
  while (*pCurrent != 0)
  {
    SPDR = *pCurrent | 0x80;
    pCurrent++;
    
    while (!(SPSR & (1<<SPIF))) {};  
  }

  SPDR = 0;
  while (!(SPSR & (1<<SPIF))) {};  
}

void writeValue(byte b)
{
  SPDR = b;
  while (!(SPSR & (1<<SPIF))) {};  
}

void loop() 
{
  unsigned long start;
  
  value++;
  

  if (value == 128)
  {
    value = 0;
  }

  for (int i = 0; i < 32 * 3; i++)
  {
     buffer[i] = value; 
  }
  start = micros();
  writeBuffer();
  Serial.println(micros() - start);
  

  //delay(100);
}

