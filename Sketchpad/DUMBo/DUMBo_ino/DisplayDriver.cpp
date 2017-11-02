#include <ARDUINO.H>
#include "Constants.h"
#include "DisplayDriver.h"

// digits are numbere from right to left; [0] is the ones place, [3] is the thousands place.
// We allocate one extra spot so that the far left digit can be the "hidden" value (10).
int digitValue[NumDigits + 1];
int digit = 0;

void DisplayDriver::showCurrentValue(int value, int dimLevel)
{
  digitValue[NumDigits] = 10;  // special value that is blank.

    // convert the value to the individual digits. 
  int current = value;
  for (int i = 0; i < NumDigits; i++)
  {
    digitValue[i] = current % 10;
    current = current / 10;
  }

  // blank out leading zeroes. We do not blank out the ones digit.
  for (int i = NumDigits - 1; i > 0; i--)
  {
      if (digitValue[i + 1] == 10 &&
          digitValue[i] == 0)
      {
        digitValue[i] = 10;      
      }
  }

  if (digit < NumDigits)
  {
    digitalWrite(digitToPinIndex[digit], LOW);
  }  

  digit = (digit + 1) % (NumDigits << dimLevel);

  // if digit < NumDigits, we display the digit. 
  // higher values are used to implement dimming.
  if (digit < NumDigits)
  {
    DisplayDriver::setSegment(digitValue[digit]);
    digitalWrite(digitToPinIndex[digit], HIGH);
  }
}

void DisplayDriver::setSegment(int value)
{
  int digitSegementVector = segmentTable[value];

  for (int bit = 0; bit < 8; bit++)
  {
    digitalWrite(segmentToPin[bit], digitSegementVector & 0x01);
    digitSegementVector = digitSegementVector >> 1;
  }
}

