
#include "MatrixDecoder.h"

#define portOfPin(P)\
  (((P)>=0&&(P)<8)?&PORTD:(((P)>7&&(P)<14)?&PORTB:&PORTC))
#define ddrOfPin(P)\
  (((P)>=0&&(P)<8)?&DDRD:(((P)>7&&(P)<14)?&DDRB:&DDRC))
#define pinOfPin(P)\
  (((P)>=0&&(P)<8)?&PIND:(((P)>7&&(P)<14)?&PINB:&PINC))
#define pinIndex(P)((uint8_t)(P>13?P-14:P&7))
#define pinMask(P)((uint8_t)(1<<pinIndex(P)))

#define pinAsInput(P) *(ddrOfPin(P))&=~pinMask(P)
#define pinAsInputPullUp(P) *(ddrOfPin(P))&=~pinMask(P);digitalHigh(P)
#define pinAsOutput(P) *(ddrOfPin(P))|=pinMask(P)
#define digitalLow(P) *(portOfPin(P))&=~pinMask(P)
#define digitalHigh(P) *(portOfPin(P))|=pinMask(P)
#define isHigh(P)((*(pinOfPin(P))& pinMask(P))>0)
#define isLow(P)((*(pinOfPin(P))& pinMask(P))==0)
#define digitalState(P)((uint8_t)isHigh(P))

#define digitalPulseLowHigh(P) *(portOfPin(P))&=~pinMask(P); *(portOfPin(P))|=pinMask(P)

#define PINHIGH(x,y) (x&=(~(1<<y)))
#define PINLOW(x,y) (x|=(1<<y))

#define PINVALUE(x, y) ((x & (1<<y)) != 0)

const int C_STROBEROW = 2;
const int C_STROBECOL = 3;

const int C_SLOAD = 8;
const int C_SCK = 9;
const int C_QH = 10;
const int C_OE1 = 11;
const int C_OE2 = 12;

const int C_DEBUG = 5;

MatrixDecoder matrixDecoder;

void setupTimer2()
{
// Timer/Counter 2 initialization
// Clock source: System Clock
// Clock value: 16000.000 kHz
// Mode: Normal top=FFh
// OC2A output: Disconnected
// OC2B output: Disconnected
ASSR=0x00;
TCCR2A=0x00;
TCCR2B=0x00;
TCNT2=0x00;
OCR2A=0x00;
OCR2B=0x00;

// Timer/Counter 2 Interrupt(s) initialization
TIMSK2=0x01; 
}

void setup() {
  pinMode(C_SLOAD, OUTPUT);
  pinMode(C_STROBEROW, INPUT);
  pinMode(C_SCK, OUTPUT);
  pinMode(C_QH, INPUT);
  pinMode(C_STROBECOL, INPUT);
  
  // Because both outputs of the shift registers are connected together,
  // we need to make sure that they are both set to High (High-Z) before 
  // we turn them to outputs.
  digitalHigh(C_OE1);
  digitalHigh(C_OE2);  
  pinMode(C_OE1, OUTPUT);
  pinMode(C_OE2, OUTPUT);

  digitalWrite(C_SLOAD, HIGH);
  digitalWrite(C_STROBEROW, HIGH);

  pinMode(C_DEBUG, OUTPUT);

  attachInterrupt(0, rowDataHandler, FALLING);      // pin 2
  attachInterrupt(1, columnDataHandler, FALLING);   // pin 3
  
  Serial.begin(9600);
  
  setupTimer2();
}

const int ArraySize = 128;
unsigned char rows[ArraySize];
unsigned char columns[ArraySize];
int dataIndex = 0;

int count = 0;
byte result = 0;

byte readFromSerial()
{
  byte result = 0;
  
   // pulse SLOAD LH to move data from latches into shift register
  digitalPulseLowHigh(C_SLOAD);

  for (int i = 0; i < 8; i++)
  {
    result = (result << 1)  + digitalState(C_QH);
    
    // pulse clock LH to move to next bit
    digitalPulseLowHigh(C_SCK);
  }  
  
  return result;
}

void readBothShiftRegisters()
{
  digitalHigh(C_DEBUG);
 
   // simulate strobe LH to pull data from input into serial...
  //digitalPulseLowHigh(C_STROBEROW);

  digitalLow(C_OE1); // turn on the output from SR1
  byte row = readFromSerial();
  digitalHigh(C_OE1);

  digitalLow(C_OE2); // turn on the output from SR1
  byte column = readFromSerial();
  digitalHigh(C_OE2);

  digitalLow(C_DEBUG);
  digitalHigh(C_DEBUG);

  matrixDecoder.DecodeRowAndColumnData(row, column);
  //return;  
  
  if (dataIndex < ArraySize)
  {
    rows[dataIndex] = row;
    columns[dataIndex] = column;
    dataIndex++;
  }
  
  digitalLow(C_DEBUG);
}

void dumpRowColumnValues()
{
  if (dataIndex == ArraySize)
  {
    Serial.println("Row Column");
    for (int i = 0; i < ArraySize; i++)
    {
      Serial.print(i);
      Serial.print(' ');
      Serial.print(rows[i], BIN);
      Serial.print(' ');
      Serial.println(columns[i], BIN);
    }
    
    dataIndex = 0;    
  }
}

// the loop function runs over and over again forever
void loop() 
{
  //dumpRowColumnValues();
  //return;  
  
  char* lampStateString = matrixDecoder.GetLampStateString();
  
  if (lampStateString != 0)
  {
    Serial.println(lampStateString);
  }
  
  //  readBothShiftRegisters();
}

const byte TIMER2_START_VALUE = 0x7F;

void handleDataStrobe()
{
digitalHigh(C_DEBUG);
//digitalLow(C_DEBUG);
//digitalHigh(C_DEBUG);
//digitalLow(C_DEBUG);
  //digitalHigh(C_DEBUG);
  //digitalLow(C_DEBUG);
  TCNT2=TIMER2_START_VALUE;
  TCCR2B=0x02;
}

void rowDataHandler()
{
  handleDataStrobe();
}

void columnDataHandler()
{
  handleDataStrobe();
}


ISR(TIMER2_OVF_vect)          // timer compare interrupt service routine
{
  TCCR2B=0x00;
  TCNT2 = 1;
  digitalHigh(C_DEBUG);
  digitalHigh(C_DEBUG);
  digitalHigh(C_DEBUG);
  digitalHigh(C_DEBUG);
  digitalLow(C_DEBUG);
  
  readBothShiftRegisters();
}



/*
void setup() {
  pinMode(0, INPUT);
  pinMode(1, INPUT);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  
  attachInterrupt(0, rowDataHandler, FALLING);      // pin 2
  attachInterrupt(1, columnDataHandler, FALLING);   // pin 3
  
  Serial.begin(9600);
}

unsigned long startTime = 0;
const int ArraySize = 128;
unsigned long rowTimes[ArraySize];
unsigned long columnTimes[ArraySize];
int rowIndex = 0;
int columnIndex = 0;

void rowDataHandler()
{
  digitalWrite(8, HIGH);
  if (rowIndex < ArraySize)
  {
    rowTimes[rowIndex] = micros();
    rowIndex++;
  }
  digitalWrite(8, LOW);
}

void columnDataHandler()
{
  digitalWrite(9, LOW);
  if (columnIndex < ArraySize)
  {
    columnTimes[columnIndex] = micros();
    columnIndex++;
  }
  digitalWrite(9, LOW);
}

// the loop function runs over and over again forever
void loop() {
  
  while (rowIndex < ArraySize && columnIndex < ArraySize)
  {
    Serial.print(".");
  }
  Serial.println();

  Serial.println("Times");
  for (int i = 0; i < ArraySize; i++)
  {
    Serial.print(rowTimes[i] - startTime);
    Serial.print(",");
    Serial.println(columnTimes[i] - startTime);
  }

  Serial.println();

  startTime = micros();
  rowIndex = 0;
  columnIndex = 0;
}


*/
