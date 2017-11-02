// Drive the AdaFruit LPD8806 RGB Led strip
// Optimized for speed
// EricGu 2011
// See .cpp file for comments. 

typedef unsigned char byte;

class LedStrip
{
public:
	LedStrip(int ledCount);

	void SetLed(short ledNumber, byte red, byte green, byte blue);

	void WriteToStripWithDim();
	void WriteToStripRaw();

	void SetDimLevel(byte dimLevel);

	byte* GetBuffer();

private:
	byte* _buffer;
	short _ledCount;
	byte _dimLevel;
};


