#include <SPI.h>
#include <LPD8806.h>

/*
StripDriver for LPD8806 strips

Implements a simple serial protocol to control the color of a strip

C<index>,r,g,b#		- set specific LED to red, green, blue
U					- Update strip with current values

*/

int ledCount = 32;
const int dataPin  = 2;
const int clockPin = 3;

// Pin 13 has an LED connected on most Arduino boards.
// give it a name:
const int led = 13; 

LPD8806 strip = LPD8806(32, dataPin, clockPin);

// the setup routine runs once when you press reset:
void setup() {
	Serial.begin(9600);

	pinMode(led, OUTPUT);

  strip.setPixelColor(15, 255, 0, 0);
  strip.begin();
  strip.show();
}

int readNumber()
{
	int number = 0;

	while (true)
	{
		while (Serial.available() == 0)
		{

		}

		char character = Serial.read();

		if ((character < '0') || (character > '9'))
		{
			return number;
		}

		number = number * 10 + (character - '0');
	}
}

int setPixelColor(int index, int red, int green, int blue)
{
	Serial.print("index, red, green, blue: ");
	Serial.print(index);
	Serial.print(",");
	Serial.print(red);
	Serial.print(",");
	Serial.print(green);
	Serial.print(",");
	Serial.println(blue);
}


// the loop routine runs over and over again forever:
void loop() 
{
	int index;
	int red;
	int green;
	int blue;

	if (Serial.available() > 0)
	{
		char character = Serial.read();
		Serial.println(character);

		switch (character)
		{
                case 'N':
                      ledCount = readNumber();
                      strip.updateLength(ledCount);
                      break;
  
		case 'C':
			index = readNumber();
			red = readNumber();
			green = readNumber();
			blue = readNumber();
			setPixelColor(index, red, green, blue);
                        strip.setPixelColor(index, red, green, blue);
			break;

		case 'U':
                        strip.show();
                        Serial.println("show");
			break;
		}


		digitalWrite(led, HIGH);   // turn the LED on (HIGH is the voltage level)
		delay(50);               // wait for a second
		digitalWrite(led, LOW);    // turn the LED off by making the voltage LOW
	}
}
