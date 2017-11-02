// junk.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Dumbo.h"

int _tmain(int argc, _TCHAR* argv[])
{
#ifdef fred
	DumboData dumboData;
	dumboData.m_active = false;
	dumboData.m_itemCount = 4;
	dumboData.m_currentItem = 0;
	dumboData.Init();
	dumboData.m_pItems[0].m_displayValue = 55;
	dumboData.m_pItems[1].m_displayValue = 255;
	dumboData.m_pItems[2].m_displayValue = 6;
	dumboData.m_pItems[3].m_displayValue = 4321;
	dumboData.m_active = 1;

	TimerHandler timerHandler(&dumboData);

	while (true)
	{
		timerHandler.HandleInterrupt();

	}
#endif

	Serial serial;
	serial.m_pData = "V0,345,127,126,125\n";

	DumboData dumboData;
	dumboData.m_itemCount = 5;
	dumboData.Init();

	CommandDispatch dispatch(serial, &dumboData);

	while (true)
	{
		dispatch.ProcessSerial();
		printf("ItemCount %d\n", dumboData.m_itemCount);
	}


	return 0;
}

