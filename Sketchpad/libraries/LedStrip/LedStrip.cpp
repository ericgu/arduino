// Drive the AdaFruit LPD8806 RGB Led strip
// Optimized for speed
// EricGu 2011

// LPD8806 protocol description can be found at:
// http://riderx.info/post/The-LPD8806-protocol-for-Adafruit-RGB-LED-Strips.aspx
//
// This is a simple, straightforward approach. There are two opportunities for improvement.
// 1) The animation speed is dependant on the speed of the calling code; more complex animations will run faster than
//    simple ones. It would be nice to set things up to work at a specific rate. This could be done by blocking on the write
//    calls until enough time has passed. It could also be done more elegantly with a second buffer and a timer interrupt. 
// 2) When a byte is written (by putting a value into SPDR), the code sits and loops until the byte has been sent, and then 
//    sends the next byte. That wasted time could be recovered by adding an interrupt handler that will be called each time 
//    the byte send has been completed; that handler will send the next byte. 
//    It's not clear whether this one is worth it; it depends upon how much time is using by the WriteToStrip() method, and
//    running at a high speed, it doesn't take that long to do that part. 

#include <stdlib.h>
#include <string.h>
#include <pins_arduino.h>
#include <ledstrip.h>

LedStrip::LedStrip(int ledCount)
{
	_ledCount = ledCount;

	_buffer = (byte*) malloc(_ledCount * 3 + 1);
	memset(_buffer, _ledCount * 3 + 1, 0);

	_dimLevel = 2;
}

// Get the buffer so you can perform operations directly on it. This will be slightly faster than calling
// SetLed for each pixel, but much less convenient. Don't do this unless you need the speed. 
//
// Each Led has three bytes allocated to it, in the order (green, red, blue). There is one extra byte after all
// of the led data; it *must* be set to zero or bad things will happen. 
//
byte* LedStrip::GetBuffer()
{
	return _buffer;
}

// Set a specified LED to the specified color. 
void LedStrip::SetLed(short ledNumber, byte red, byte green, byte blue)
{
	byte* pCurrent = _buffer + ledNumber * 3; // point to the green byte for this led.

	*pCurrent = green;
	pCurrent++;
	*pCurrent = red;
	pCurrent++;
	*pCurrent = blue;
}

// DimLevel reduces the brightness of the strip. This makes it draw less power (so perhaps you can develop your
// system without an external power supply), and makes it much easier on the eyes. 
// The dim levels are:
// 0 - 100%
// 1 -  50%
// 2 -  25%
// 3 -  12%
// 4 -   6%
// and so on...
void LedStrip::SetDimLevel(byte dimLevel)
{
	_dimLevel = dimLevel;
}

// Send the current data out to the strip, performing dimming as specified. This is *slightly* slower than 
// WriteToStripRaw(), so if you need every bit of performance, you might want to try that instead. 
void LedStrip::WriteToStripWithDim()
{
  byte* pCurrent = _buffer;
  
  // loop through all the bytes, until we hit the extra zero byte at the end. 
  while (*pCurrent != 0)
  {
    SPDR = (*pCurrent >> _dimLevel) | 0x80;
    pCurrent++;
    
    while (!(SPSR & (1<<SPIF))) {};  
  }

  // send out a zero byte to mark the end of the data. 
  SPDR = 0;
  while (!(SPSR & (1<<SPIF))) {};  
}

// Send the current data to the strip. Slightly faster but not as nice as WriteToStripWithDim().
void LedStrip::WriteToStripRaw()
{
  byte* pCurrent = _buffer;
  
  while (*pCurrent != 0)
  {
    SPDR = *pCurrent | 0x80;
    pCurrent++;
    
    while (!(SPSR & (1<<SPIF))) {};  
  }

  SPDR = 0;
  while (!(SPSR & (1<<SPIF))) {};  
}
