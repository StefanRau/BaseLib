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

#include "Debug.h"

#ifdef DEBUG_APPLICATION

static Debug *gInstance = nullptr;

Debug::Debug()
{
	Serial.begin(DEBUG_APPLICATION);

	while (!Serial)
	{
		; // wait for serial port to connect. Needed for native USB port only
	}

	delay(10);
	// That does not work using text objects, because we would get a circular reference there
	Serial.println("Start Debugger");
	// Wait until buffer is empty
	Serial.flush();
}

Debug::~Debug()
{
}

Debug *Debug::GetInstance()
{
	// returns a pointer to singleton instance
	gInstance = (gInstance == nullptr) ? new Debug : gInstance;
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

void Debug::PrintFromTask(String iOutput)
{
	// Write into a string buffer
	_mWriteBuffer += iOutput;
	_mBufferContainsData = true;
}

void Debug::loop()
{
	if (_mBufferContainsData)
	{
		Serial.print(_mWriteBuffer);
		// Serial.println();
		//  Wait until buffer is empty
		Serial.flush();
		_mWriteBuffer = "";
		_mBufferContainsData = false;
	}
}

#endif