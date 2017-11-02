#ifndef Constants_h
#define Constants_h

const int NumDigits = 4;
const int MaxItems = 32;

// this defines which segments are lit for the digits 0 through 9. Bit 0 is "g", bit 7 is "a".
static int segmentTable[] = {
 #if prototype
	0x7E,
	0x30,
	0x6D,
	0x79,
	0x33,
	0x5B,
	0x5F,
	0x70,
	0x7F,
	0x7B,
        0x00    // blank
#else     
  0x7E,
  0x30,
  0x6D,
  0x79,
  0x33,
  0x5B,
  0x1F,
  0x70,
  0x7F,
  0x73,
  0x00,

// debug & testing entries...

  0x80,
  0x40,
  0x20,
  0x10,
  0x08,
  0x04,
  0x02,
  0x01,
  0x00,
  0x00,
  0x00
  

  
#endif
};

// It's useful to be about to lay out the display board in any way that is convenient. That means that
// the segments may not be hookup up in order, and the common pins may also not be in order. 
// These arrays are used to remap the segments and digits so that they are in order. 

#ifdef prototype
static int digitToPinIndex[] = {A1, A0, A2, A3};
const int segmentToPin[] = {6, 7, 10, 11, 12, 9, 8, 13};

#else
  static int digitToPinIndex[] = {A3, A2, A1, A0};
  const int segmentToPin[] = {11, 13, 7, 8, 9, 10, 12, 6
  
  
};


#endif

#endif

