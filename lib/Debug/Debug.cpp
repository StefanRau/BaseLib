// Arduino Base Libs
// 18.10.2021
// Stefan Rau
// History
// 18.10.2021: 1st version - Stefan Rau
// 14.01.2022: using macros from timer interrupts for writing debugger output - Stefan Rau
// 21.09.2022: use GetInstance instead of Get<Typename> - Stefan Rau
// 23.09.2022: Get rid of TimerInterrupt_Generic_Debug - Stefan Rau
// 26.09.2022: DEBUG_APPLICATION defined in platform.ini - Stefan Rau
// 04.11.2022: Wait until serial port is ready; that starts debuggernot before the connected terminal is ready - Stefan Rau
// 08.01.2023: Reorganized debugging by implementing different levels - Stefan Rau
// 16.04.2023: Optional count down at start up - Stefan Rau
// 04.05.2023: New functionality: binary dump - Stefan Rau
// 04.05.2023: Writes single characters to output - Stefan Rau
// 08.05.2023: Strict definitions for debug possibilities - Stefan Rau

#include "Debug.h"

#if DEBUG_APPLICATION > 0

static Debug *gInstance = nullptr;

Debug::Debug(int iCountdown)
{
	int lCount = iCountdown + 1;

	Serial.begin(DEBUG_SPEED);

	while (!Serial)
	{
		; // wait for serial port to connect. Needed for native USB port only
	}

	delay(10);
	// That does not work using text objects, because we would get a circular reference there
	Serial.print("Start Debugger");

	if (lCount > 1)
	{
		Serial.print(" in   ");

		while (lCount-- > 0)
		{
			Serial.print("\b\b");
			Serial.print(lCount);
			Serial.print("s");
			delay(1000);
		}
	}
	Serial.println("");

	// Wait until buffer is empty
	Serial.flush();
}

Debug::~Debug()
{
}

Debug *Debug::GetInstance(int iCountdown)
{
	// returns a pointer to singleton instance
	gInstance = (gInstance == nullptr) ? new Debug(iCountdown) : gInstance;
	return gInstance;
}

void Debug::Print(String iOutput)
{
	Serial.print(iOutput);
	// Wait until buffer is empty
	Serial.flush();
}

void Debug::Print(const Printable &iOutput)
{
	Serial.print(iOutput);
	// Wait until buffer is empty
	Serial.flush();
}

void Debug::Print(char iOutput)
{
	Serial.print(iOutput);
	// Wait until buffer is empty
	Serial.flush();
}

void Debug::PrintLn(String iOutput)
{
	Serial.println(iOutput);
	// Wait until buffer is empty
	Serial.flush();
}

void Debug::PrintLn(const Printable &iOutput)
{
	Serial.println(iOutput);
	// Wait until buffer is empty
	Serial.flush();
}

void Debug::PrintLn(char iOutput)
{
	Serial.print(iOutput);
	// Wait until buffer is empty
	Serial.flush();
}

void Debug::BinaryDump(void *iData, size_t iLength)
{
	void **lAddress = &iData;
	char *lData = (char *)iData;
	char lChar;
	char lBufferHex[80];
	char lBufferChar[80];
	int lCharCount = 0;
	int lHexCount = 0;

	Serial.println(">>>>>>>>>>>>>>>> Binary dump start");
	Serial.print("Length: ");
	Serial.println(iLength);
	Serial.print("Address: ");
	Serial.println((unsigned)lAddress);
	memset(lBufferHex, '\0', 80);
	memset(lBufferChar, '\0', 80);

	// Dump data
	for (int lCount = iLength; lCount > 0; lCount -= 1)
	{
		lChar = *lData;
		sprintf(&lBufferHex[lHexCount], "%02x-", (int)lChar);

		if (lChar < 32)
		{
			lChar = '_';
		}
		lBufferChar[lCharCount] = lChar;

		if (lCharCount > 10)
		{
			lBufferHex[lHexCount + 2] = '\0';
			lBufferChar[lCharCount + 1] = '\0';
			lCharCount = 0;
			lHexCount = 0;
			Serial.print(lBufferHex);
			Serial.print(" :: ");
			Serial.println(lBufferChar);
			memset(lBufferHex, '\0', 80);
			memset(lBufferChar, '\0', 80);
		}
		else
		{
			lCharCount += 1;
			lHexCount += 3;
		}

		lData += 1;
	}

	// Print remaining chars
	if (lCharCount > 10)
	{
		lBufferHex[lHexCount + 2] = '\0';
		lBufferChar[lCharCount + 1] = '\0';
		Serial.print(lBufferHex);
		Serial.print(" :: ");
		Serial.println(lBufferChar);
	}

	Serial.println("<<<<<<<<<<<<<<<< Binary dump end");

	// Wait until buffer is empty
	Serial.flush();
}

void Debug::PrintFromTask(String iOutput)
{
	// Write into a string buffer
	mWriteBuffer += iOutput;
	mBufferContainsData = true;
}

void Debug::loop()
{
	if (mBufferContainsData)
	{
		Serial.print(mWriteBuffer);
		//  Wait until buffer is empty
		Serial.flush();
		mWriteBuffer = "";
		mBufferContainsData = false;
	}
}

#endif