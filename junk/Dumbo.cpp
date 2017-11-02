// DumboSupport.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "Dumbo.h"

char* malloc(int n) { return 0;}

char* readValue(char* pString, int* pValue)
{
	*pValue = 0;

	while (true)
	{
		if (*pString >= '0' && *pString <= '9')
		{
			*pValue = *pValue * 10 + *pString - '0';
			pString++;
		}
		else
		{
			return pString;
		}
	}
}

// Command Syntax
// ?
//		Returns "DUMBO v0.x
// Sn
//		Sets the number of values to n
// In
//		Sets the update wait count to n
// Vn,v,r,g,b
//		Sets value of entry:
//			n - number of the entry
//			v - LED value
//			r - red value of status LED (0-127)
//			g - red value of status LED (0-127)
//			b - red value of status LED (0-127)

CommandDispatch::CommandDispatch(Serial serial, DumboData *pDumboData)
{
	m_serial = serial;
	m_pDumboData = pDumboData;

	m_pCommand = new char[16];
	m_pCommandCurrentChar = m_pCommand;
}

void CommandDispatch::ProcessSerial()
{
	int charValue;

	if (m_serial.available())
	{
		charValue = m_serial.read();

		if (charValue == '\n')
		{
			*m_pCommandCurrentChar = 0;
			DispatchCommand();
			m_pCommandCurrentChar = m_pCommand;
		}
		else
		{
			*m_pCommandCurrentChar = (char) charValue;
			m_pCommandCurrentChar++;
		}
	}
}

void CommandDispatch::DispatchCommand()
{
	switch (*m_pCommand)
	{
	case '?':
		m_serial.println("DUMBo V1");
		break;

	case 'S':
		readValue(m_pCommand + 1, &m_pDumboData->m_itemCount);
		m_pDumboData->Init();
		break;

	case 'I':
		readValue(m_pCommand + 1, &m_pDumboData->m_itemWaitCount);
		break;

	case 'V':
		int index;
		char* pCurrent;

		pCurrent = readValue(m_pCommand + 1, &index);
		DumboItem* pCurrentItem = m_pDumboData->m_pItems + index;

		pCurrent = readValue(pCurrent + 1, &(pCurrentItem->m_displayValue));
		pCurrent = readValue(pCurrent + 1, &(pCurrentItem->m_red));
		pCurrent = readValue(pCurrent + 1, &(pCurrentItem->m_green));
		pCurrent = readValue(pCurrent + 1, &(pCurrentItem->m_blue));
		break;
	}
}

DumboData::DumboData()
{
	m_active = 0;
}

void DumboData::Init()
{
	m_pItems = new DumboItem[m_itemCount];
	m_currentItem = 0;
	m_active = 1;
}

TimerHandler::TimerHandler(DumboData* pDumboData)
{
	m_pDumboData = pDumboData;
	m_ledValue = 0;
	m_digitIndex = 0;
	m_repeatCount = 0;
	m_currentItem = 0;
}

void TimerHandler::HandleInterrupt()
{
	if (m_pDumboData->m_active == 0)
	{
		return;
	}

	// disable all segments
	int digit = m_ledValue % 10;
	printf("%d %d\n", m_digitIndex, digit);
	// set digit into output
	// enable m_digitIndex segment

	m_ledValue = m_ledValue / 10;
	m_digitIndex++;

		// Done with the 4 led digits
	if (m_digitIndex == 4)
	{
		m_digitIndex = 0;

		m_repeatCount++;

			// Done with displaying this value
		if (m_repeatCount == 1 /*REPEAT_COUNT_MAX*/)
		{
			m_repeatCount = 0;

			m_currentItem++;
			if (m_currentItem == m_pDumboData->m_itemCount)
			{
				m_currentItem = 0;
			}
		}
			
		m_ledValue = (m_pDumboData->m_pItems + m_currentItem)->m_displayValue;
	}
}
