int segmentTable[] = {
	0x7E,
	0x30,
	0x6D,
	0x79,
	0x33,
	0x5B,
	0x5F,
	0x70,
	0x7F,
	0x7B
};


typedef class Serial
{
public:
	bool available() { return true;}

	int read() { return 1;} 
	void println(char* data) {}

} Serial;

class DumboItem
{
public:
	int m_displayValue;
	int m_red;
	int m_green;
	int m_blue;
};

class DumboData
{
public:
	bool m_active;
	int m_itemCount;
	int m_itemWaitCount;
	
	DumboItem* m_pItems;

	int m_currentItem;

	DumboData();
	void Init();
};

class CommandDispatch
{
private:
	Serial m_serial;
	DumboData* m_pDumboData;
	char* m_pCommand;
	char* m_pCommandCurrentChar;

public:
	CommandDispatch(Serial serial, DumboData* pDumboData);

	void ProcessSerial();
	void DispatchCommand();	
};

class TimerHandler
{
private:
	DumboData* m_pDumboData;

	int m_currentItem;
	int m_digitIndex;
	int m_ledValue;
	int m_repeatCount;

public:
	TimerHandler(DumboData* pDumboData);

	void HandleInterrupt();
};

#define REPEAT_COUNT_MAX 255


