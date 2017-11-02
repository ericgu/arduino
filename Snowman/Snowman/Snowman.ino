#include <Adafruit_NeoPixel.h>
#include "color.h"
#include "ColorDecider.h"
#include "ColorFader.h"
#include "ColorFaderMultiple.h"
#include "ColorSteady.h"
#include "Constants.h"
#include "Bouncer.h"

#define PIN 6
#define DEBUGPIN 7

// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(44, PIN, NEO_GRB + NEO_KHZ800);

// IMPORTANT: To reduce NeoPixel burnout risk, add 1000 uF capacitor across
// pixel power leads, add 300 - 500 Ohm resistor on first pixel's data input
// and minimize distance between Arduino and first pixel.  Avoid connecting
// on a live circuit...if you must, connect GND first.

Color GetRandomFullColor()
{
  return targets[random(12)];
}

void SetColorSpanRandom(int minIndex, int maxIndex)
{
    Color color = GetRandomFullColor();
    for (int i = minIndex; i <= maxIndex; i++)
    {
      strip.setPixelColor(i, color.red, color.green, color.blue);
    }    
}

void ExtraStealthMode(int currentPixel, Color color)
{
    for (int extraIndex = 0; extraIndex < EXTRA_SIZE; extraIndex++)
    {
      Extra item = extra[extraIndex];
      
      if (item.main == currentPixel)
      {
        Color dimColor = color.Dim(item.dimShift);
        
        strip.setPixelColor(item.secondary, dimColor.red, dimColor.green, dimColor.blue);
      }
    }
}

void spinColorUp(ColorDecider* pColorDecider, int delayCount)
{
  for (int i = 0; i < MAIN_SIZE; i++)
  {
    if (i == 1)
    {
      pColorDecider->Push();
    }

    int pixel = main[i];
    Color color = pColorDecider->GetNextColor();
    strip.setPixelColor(pixel, color.red, color.green, color.blue);

    ExtraStealthMode(pixel, color);

    if (delay != 0)
    { 
      strip.show();
      delay(delayCount);
    }
  }
}

void Larson(int delayCount)
{
  const int STEPS_PER_COLOR = 84;
  
  Bouncer bouncers[] = {Bouncer(SMILE_LEFT, SMILE_RIGHT, 3),
                        Bouncer(SMILE_LEFT, SMILE_RIGHT, 2),
                        Bouncer(SMILE_LEFT, SMILE_RIGHT, 1),
                        Bouncer(SMILE_LEFT, SMILE_RIGHT, 0)};

  Bouncer buttonBouncer(BUTTON_BOTTOM, BUTTON_TOP, 0);

  ColorFader fader;  
  Color color(255, 0, 0);
  Color buttonColor;
  int buttonCount;
  int colorIndex = 1;
  
  fader.SetTarget(targets[0], 1);
  
  for (int steps = 0; steps < STEPS_PER_COLOR * 3; steps++)
  {
    color = fader.GetNextColor();
    
    for (int i = SMILE_LEFT; i <= SMILE_RIGHT; i++)
    {
      strip.setPixelColor(i, 0);
    } 
    strip.setPixelColor(LEFT_EYE, 0);
    strip.setPixelColor(RIGHT_EYE, 0);
    strip.setPixelColor(NOSE, 0);
    
    for (int b = 3; b >= 0; b--)
    {
      bouncers[b].Step();
      
      Color dimColor = color.Dim(b);

      strip.setPixelColor(bouncers[b].currentValue, dimColor.red, dimColor.green, dimColor.blue);
    }
    
    int value = bouncers[0].currentValue;
    int increment = bouncers[0].increment;
    if (value == SMILE_LEFT)
    {
      strip.setPixelColor(LEFT_EYE, color.red, color.green, color.blue); 
      buttonCount++;
      buttonBouncer.Step();
      buttonColor = GetRandomFullColor();
    }
    else if (value == SMILE_RIGHT)
    {
      strip.setPixelColor(RIGHT_EYE, color.red, color.green, color.blue); 
      buttonCount++;
      buttonBouncer.Step();
      buttonColor = GetRandomFullColor();
    }
    else if (value == SMILE_LEFT + 3 && increment == 1 || 
             value == SMILE_LEFT + 4 && increment == -1)
    {
      strip.setPixelColor(NOSE, 0, 255, 0);
    }
    
    if (steps % STEPS_PER_COLOR == 0)
    {
      colorIndex = (colorIndex + 1) % 3;
      fader.SetTarget(targets[colorIndex], STEPS_PER_COLOR);
      color = targets[colorIndex];
      Serial.println(colorIndex);
    }
    
    strip.setPixelColor(BUTTON_BOTTOM, 0, 0, 0);
    strip.setPixelColor(BUTTON_MIDDLE, 0, 0, 0);
    strip.setPixelColor(BUTTON_TOP   , 0, 0, 0);
    strip.setPixelColor(buttonBouncer.currentValue, buttonColor.red, buttonColor.green, buttonColor.blue);

  // use buttons to count up in binary, one tick for each bounce
  // do green on nose only when it hits the first number in that direction
  // do fade out of nose/eyes instead of pure on/off...
  // use colorfadermultiple on smile

    strip.show();
    delay(delayCount);
  }  
}

ColorFaderMultiple colorFaderMultiple;
    ColorSteady colorSteady;

void setup() {
  pinMode(DEBUGPIN, OUTPUT);

  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  
  Serial.begin(9600);
  Serial.println("Start");

  colorFaderMultiple.SetFade(targets, 6, 25);
}


const int UPSPEED = 40;
void Startup()
{
  colorSteady.SetTarget(Color(255, 255, 255));
    
  for (int i = 0; i < 100; i++)
  {
    spinColorUp(&colorSteady, 0);
    colorSteady.IncreaseStart();
    strip.show();
  }
  
  delay(500);

  strip.setPixelColor(BUTTON_BOTTOM, 0, 0, 255);
  strip.show();
  delay(UPSPEED * 12);

  strip.setPixelColor(BUTTON_MIDDLE, 0, 0, 255);
  strip.show();
  delay(UPSPEED * 12);
  
  strip.setPixelColor(BUTTON_TOP, 0, 0, 255);
  strip.show();
  delay(UPSPEED * 15);
  
  for (int i = 0; i < 4; i++)
  {
    strip.setPixelColor(SMILE_LEFT + 3 - i, 255, 255, 0);
    strip.setPixelColor(SMILE_LEFT + 4 + i, 255, 255, 0);
    strip.show();
    delay(UPSPEED);
  }
  delay(UPSPEED * 8);

  strip.setPixelColor(NOSE, 255, 0, 0);
  strip.show();
  delay(UPSPEED * 8);

  strip.setPixelColor(LEFT_EYE, 0, 255, 0);  
  strip.setPixelColor(RIGHT_EYE, 0, 255, 0);  
  strip.show();
  delay(3000);  
}

void BigSpin()
{
  for (int i = 0; i < 300; i++)
  {
    colorFaderMultiple.Pop();
  
    spinColorUp(&colorFaderMultiple, 0);
    digitalWrite(DEBUGPIN, HIGH);
    strip.show();
    digitalWrite(DEBUGPIN, LOW);
    delay(25);
  }
}

void GlobeColor(int delayCount)
{
    SetColorSpanRandom(BOTTOM_FIRST, BOTTOM_LAST);
    SetColorSpanRandom(MIDDLE_FIRST, MIDDLE_LAST);
    SetColorSpanRandom(TOP_FIRST, TOP_LAST);
    
    SetColorSpanRandom(BUTTON_BOTTOM, BUTTON_TOP);

    SetColorSpanRandom(SMILE_LEFT, SMILE_RIGHT);
    
    SetColorSpanRandom(NOSE, NOSE);
    SetColorSpanRandom(LEFT_EYE, LEFT_EYE);
    SetColorSpanRandom(RIGHT_EYE, RIGHT_EYE);
    
    strip.show();
    delay(delayCount);
}

void DoGlobe()
{
  Serial.println("DoGlobe");
  unsigned short delayValue = 200 << 8;
  
  for (int i = 0; i < 10; i++)
  {
    GlobeColor(delayValue >> 6);
  }
  
  for (int i = 0; i < 70; i++)
  {
    GlobeColor(delayValue >> 6);
    delayValue = (((unsigned long int)delayValue) * 95) / 100;
  }
  
  for (int quickSpin = 0; quickSpin < 10; quickSpin++)
  {
    for (int flash = 0; flash < 60; flash++)
    {
      GlobeColor(10);
    }
    delay(1500);
  }
}

// TODO
// Make color change speed up slowly...
// Do something with the highlight colors

void loop() 
{
  Startup();
  DoGlobe();
  BigSpin();
  Larson(150);
  delay(2000);
}

