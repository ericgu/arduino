/*

void bounce(int delayCount)
{
  for (int i = SMILE_LEFT; i <= SMILE_RIGHT; i++)
  {
    strip.setPixelColor(i, 0);
  }  
  
  int index = SMILE_LEFT;
  int delta = 1;

  for (int steps = 0; steps < 200; steps++)
  {
    strip.setPixelColor(index, 0);
    
    index += delta;
    
    if (index == SMILE_LEFT || delta == SMILE_RIGHT)
    {
      delta = -delta;
    }
    
    strip.setPixelColor(index, strip.Color(255, 0, 0));
    delay(delayCount);
  }  
}

void larson(int delayCount)
{
  for (int i = SMILE_LEFT; i <= SMILE_RIGHT; i++)
  {
    strip.setPixelColor(i, 0);
  }  

  int last[4] = {SMILE_LEFT + 3, SMILE_LEFT + 2, SMILE_LEFT + 1, SMILE_LEFT};
 
  int index = SMILE_LEFT;
  int delta = 1;

  for (int steps = 0; steps < 200; steps++)
  {
    strip.setPixelColor(last[0], 0);
    
    for (int move = 0; move < 3; move++)
    {
      last[move] = last[move + 1];
    }
    last[3] = last[2] + delta;

    for (int i = 0; i < 4; i++)
    {
      //strip.SetPixelColor
    }    
    
    strip.setPixelColor(index, strip.Color(255, 0, 0));
    delay(delayCount);

    if (last[3] == SMILE_LEFT || last[3] == SMILE_RIGHT)
    {
      delta = -delta;
    }

  }  
}
*/
  
  
//  strip.setPixelColor(BUTTON_MIDDLE, strip.Color(255, 0, 0));
//  strip.show();
/*
  Color color = fader.GetNextColor();

  if (!fader.GetInFade())
  {
    if (color.red < 5)
    {
      fader.SetTarget(Color(255, 0, 0), 64);
    }
    else
    {
      fader.SetTarget(Color(0, 0, 0), 64);
    }
  }
*/
  //Serial.println("rgb");
  //Serial.println(color.red);
  //Serial.println(color.green);
  //Serial.println(color.blue);
  //strip.setPixelColor(BUTTON_MIDDLE, strip.Color(color.red, color.green, color.blue));

  /*
  color = colorFaderMultiple.GetNextColor();
  strip.setPixelColor(BUTTON_TOP, strip.Color(color.red, color.green, color.blue));
  strip.show();
  delay(10);
  return;
  */
  
  /*
  const int spinDelay = 100;
  
  spinBetween(252,   0,   0, 63, 1);
  spinBetween(  0, 252,   0, 63, 1);
  spinBetween(  0,   0, 252, 63, 1);
  spinBetween(252, 252,   0, 63, 1);
  spinBetween(252,   0, 252, 63, 1);
  spinBetween(  0,   0, 252, 63, 1);
  return;
  
  for (int i = 0; i < 8; i++)
  {
    byte pixel = smile[i];
    Serial.println(pixel);
    strip.setPixelColor(smile[i], strip.Color(intensity, intensity, intensity));    
  }
  strip.show();
  delay(50);
  intensity += 2;
  return;
*/

int number = 0;

byte intensity = 0;

byte pixel = 0;


  if (Serial.available() > 0)
  {
    byte value = Serial.read();
  
    if (value == '+')
    {
      Serial.print(number);
      Serial.println(" on");
      //strip.setPixelColor(number, strip.Color(255, 255, 255));
      strip.setPixelColor(pixel, strip.Color(number, number, number));
      number = 0;
    }
    else if (value == '-')
    {
      Serial.print(number);
      Serial.println(" off");
      strip.setPixelColor(number, strip.Color(0, 0, 0));
      number = 0;
    }
    else if (value == 'p')
    {
      Serial.print("Pixel: ");
      Serial.println(number);
      pixel = number;
      number = 0;
    }
    else
    {
      Serial.println("key:");
      Serial.println(value - '0');
      number = number * 10 + (value - '0');  
    }

    Serial.println(number);  
    strip.show();
  }

