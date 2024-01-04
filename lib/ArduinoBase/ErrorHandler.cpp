// Arduino Base Libs
// 18.10.2021
// Stefan Rau
// History
// 18.10.2021: 1st version - Stefan Rau
// 27.10.2021: Stores errors in separate EEPROM if installed - Stefan Rau
// 27.10.2021: Constructor requires structure - Stefan Rau
// 11.11.2021: Use base class for instantiating EEPROM - Stefan Rau
// 12.01.2022: extended by ARDUINO_NANO_RP2040_CONNECT - Stefan Rau
// 16.03.2022: ARDUINO_NANO_RP2040_CONNECT removed - Stefan Rau
// 04.04.2022: Improve output of log: use severity enumeration, ISO date formate - Stefan Rau
// 20.06.2022: Debug instantiation of classes - Stefan Rau
// 08.08.2022: Switch to ARDUINO NANO IOT due to memory issues - Stefan Rau
// 08.08.2022: add ARDUINO NANO 33 BLE - Stefan Rau
// 26.08.2022: fix wrong log output - Stefan Rau
// 07.09.2022: transient error list removed - Stefan Rau
// 21.09.2022: use GetInstance instead of Get<Typename> - Stefan Rau
// 26.09.2022: EXTERNAL_EEPROM defined in platform.ini - Stefan Rau
// 26.09.2022: DEBUG_APPLICATION defined in platform.ini - Stefan Rau
// 21.12.2022: extend destructor - Stefan Rau

#include "ErrorHandler.h"

// Text definitions

/// <summary>
/// There is no new EEPROM address required
/// </summary>
TextErrorHandler::TextErrorHandler() : TextBase()
{
	DEBUG_INSTANTIATION("TextErrorHandler");
}

TextErrorHandler::~TextErrorHandler()
{
	DEBUG_DESTROY("TextI2CBase");
}

String TextErrorHandler::GetObjectName()
{
	switch (GetLanguage())
	{
		TextLangE("Error Handler");
		TextLangD("Fehlerbehandlung");
	}
}

#ifdef EXTERNAL_EEPROM
String TextErrorHandler::FunctionNameUnknown(char iModuleIdentifyer, char iParameter)
{
	switch (GetLanguage())
	{
		TextLangE("Unknown function: " + String(iModuleIdentifyer) + ":" + String(iParameter));
		TextLangD("Unbekannte Funktion: " + String(iModuleIdentifyer) + ":" + String(iParameter));
	}
}

String TextErrorHandler::FormatDone()
{
	switch (GetLanguage())
	{
		TextLangE("EEPROM formatted");
		TextLangD("EEPROM formatiert");
	}
}

String TextErrorHandler::FormatFailed()
{
	switch (GetLanguage())
	{
		TextLangE("EEPROM formatting failed");
		TextLangD("EEPROM Formatierung fehlgschlagen");
	}
}

String TextErrorHandler::ErrorListDone()
{
	switch (GetLanguage())
	{
		TextLangE("There are no more entries");
		TextLangD("Es gibt keine weiteren Einträge");
	}
}

String TextErrorHandler::SeverityMessage()
{
	switch (GetLanguage())
	{
		TextLangE("Message");
		TextLangD("Meldung");
	}
}

String TextErrorHandler::SeverityWarning()
{
	switch (GetLanguage())
	{
		TextLangE("Warning");
		TextLangD("Warnung");
	}
}

String TextErrorHandler::SeverityError()
{
	switch (GetLanguage())
	{
		TextLangE("Error");
		TextLangD("Fehler");
	}
}

String TextErrorHandler::SeverityFatal()
{
	switch (GetLanguage())
	{
		TextLangE("Fatal Error");
		TextLangD("Schwerwiegender Fehler");
	}
}

String TextErrorHandler::SeverityUnknown()
{
	switch (GetLanguage())
	{
		TextLangE("Unknow");
		TextLangD("Unbekannt");
	}
}
#endif

/////////////////////////////////////////////////////////////

Error::Error(int iNumber, eSeverity iSeverity, String iErrorMessage)
{
	DEBUG_INSTANTIATION("Error: iNumber=" + String(iNumber) + ", iSeverity=" + String((char)iSeverity) + ", iErrorMessage=" + iErrorMessage);
	mErrorEntry.ErrorHeader.ErrorHeader.Severity = iSeverity;
	mErrorEntry.ErrorHeader.ErrorHeader.Time.tm_year = 2000;
	mErrorEntry.ErrorHeader.ErrorHeader.Time.tm_mon = 11;
	mErrorEntry.ErrorHeader.ErrorHeader.Time.tm_mday = 13;
	mErrorEntry.ErrorHeader.ErrorHeader.Time.tm_hour = 19;
	mErrorEntry.ErrorHeader.ErrorHeader.Time.tm_min = 15;
	mErrorEntry.ErrorHeader.ErrorHeader.Time.tm_sec = 0;
	mErrorEntry.ErrorHeader.ErrorHeader.Count = iNumber;
	mErrorEntry.ErrorMessage = iErrorMessage;
}

Error::~Error()
{
	DEBUG_DESTROY("Error");
}

Error::sErrorEntry Error::GetErrorEntry()
{
	return mErrorEntry;
}

/////////////////////////////////////////////////////////////

static ErrorHandler *gInstance = nullptr;

ErrorHandler::ErrorHandler(sInitializeModule iInitializeModule) : I2CBase(iInitializeModule)
{

	DEBUG_INSTANTIATION("ErrorHandler: iInitializeModule[SettingsAddress, I2CAddress]=[" + String(iInitializeModule.SettingsAddress) + ", " + String(iInitializeModule.I2CAddress) + "]");
	_mText = new TextErrorHandler();

#ifdef EXTERNAL_EEPROM
	union uErrorEEPROMHeader lBuffer;

	if (GetI2CGlobalEEPROM() != nullptr)
	{
		// get checksum
		if (!I2ECheckEEPROMHeader())
		{
			// checksum does not match => format EEPROM
			DEBUG_PRINT_LN("Format EEPROM");
			GetI2CGlobalEEPROM()->setBlock(ERROR_HANDLER_START_ADDRESS, 0, GetI2CGlobalEEPROM()->getDeviceSize() - ERROR_HANDLER_START_ADDRESS);

			// get next count of error entries
			lBuffer.ErrorHeader.NumberOfErrors = 0;
			lBuffer.ErrorHeader.NextErrorWritePointer = sizeof(sErrorEEPROMHeader) + ERROR_HANDLER_START_ADDRESS;
			I2EWriteEEPROMHeader(lBuffer);
		}
		else
		{
			// Read EEPROM meta data
			GetI2CGlobalEEPROM()->readBlock(ERROR_HANDLER_START_ADDRESS, lBuffer.Buffer, sizeof(sErrorEEPROMHeader));
			DEBUG_PRINT_LN("EEPROM is already formatted - Log Count: " + String(lBuffer.ErrorHeader.NumberOfErrors) + ", Address for writing: " + String(lBuffer.ErrorHeader.NextErrorWritePointer));
		}

		//	Print(Error::eSeverity::TMessage, "EEPROM for logger is initialized");
		DEBUG_PRINT_LN("EEPROM for logger is initialized");
	}
	else
	{
		// Error handler works also without EEPROM but does not persist errors in a log
		DEBUG_PRINT_LN("EEPROM for logger can't be initialized");
	}
#endif

	mModuleIsInitialized = true;
}

ErrorHandler::~ErrorHandler()
{
	DEBUG_DESTROY("ErrorHandler");
}

ErrorHandler *ErrorHandler::GetInstance()
{
	DEBUG_METHOD_CALL("ErrorHandler::GetInstance");

	// returns a pointer to singleton instance
	sInitializeModule lInitializeModule = {-1, -1};

	gInstance = (gInstance == nullptr) ? new ErrorHandler(lInitializeModule) : gInstance;
	return gInstance;
}

void ErrorHandler::loop()
{
	// nothing implemented => read errors via remote control
}

#if DEBUG_APPLICATION == 0
String ErrorHandler::DispatchSerial(char iModuleIdentifyer, char iParameter)
{
	DEBUG_METHOD_CALL("ErrorHandler::DispatchSerial");

#ifdef EXTERNAL_EEPROM
	union uErrorEEPROMHeader lErrorEepromHeader;
	String lReturn = "";

	if (GetI2CGlobalEEPROM() != nullptr)
	{
		GetI2CGlobalEEPROM()->readBlock(ERROR_HANDLER_START_ADDRESS, lErrorEepromHeader.Buffer, sizeof(sErrorEEPROMHeader));

		switch ((ErrorHandler::eFunctionCode)iModuleIdentifyer)
		{
		case ErrorHandler::eFunctionCode::TName:

			switch ((ErrorHandler::eFunctionCode)iParameter)
			{
			case ErrorHandler::eFunctionCode::TReadNext:
				// get header
				union Error::uErrorHeader lErrorHeader;
				char lTimeString[50];
				char *lTimeStringP;

				lTimeStringP = lTimeString;

				GetI2CGlobalEEPROM()->readBlock(mEEPROMMemoryIterator, lErrorHeader.Buffer, sizeof(Error::sErrorHeader));
				mEEPROMMemoryIterator += sizeof(Error::sErrorHeader); // + ERROR_HANDLER_START_ADDRESS;

				// Error count
				lReturn = String(mEEPROMErrorIterator);
				lReturn += ": ";

				// Timestamp of the error
				sprintf(lTimeStringP, "%02i.%02i.%04i %02i:%02i:%02i ",
						lErrorHeader.ErrorHeader.Time.tm_mday,
						lErrorHeader.ErrorHeader.Time.tm_mon,
						lErrorHeader.ErrorHeader.Time.tm_year,
						lErrorHeader.ErrorHeader.Time.tm_hour,
						lErrorHeader.ErrorHeader.Time.tm_min,
						lErrorHeader.ErrorHeader.Time.tm_sec);
				lReturn += String(lTimeStringP);

				// Severity
				switch (lErrorHeader.ErrorHeader.Severity)
				{
				case Error::eSeverity ::TMessage:
					lReturn += _mText->SeverityMessage();
					break;
				case Error::eSeverity::TWarning:
					lReturn += _mText->SeverityWarning();
					break;
				case Error::eSeverity::TError:
					lReturn += _mText->SeverityError();
					break;
				case Error::eSeverity::TFatal:
					lReturn += _mText->SeverityFatal();
					break;
				default:
					lReturn += _mText->SeverityUnknown();
					break;
				}
				lReturn += ": ";

				// get error message
				if (mEEPROMErrorIterator < lErrorEepromHeader.ErrorHeader.NumberOfErrors)
				{
					char lChar;
					do
					{
						lChar = (char)GetI2CGlobalEEPROM()->readByte(mEEPROMMemoryIterator++);
						if (lChar != 0)
						{
							lReturn += String(lChar);
						}
					} while (lChar != 0);

					lReturn += "\n";
					mEEPROMErrorIterator += 1;
					return lReturn;
				}
				else
				{
					return _mText->ErrorListDone();
				};
				break;
			case ErrorHandler::eFunctionCode::TReadReset:
				mEEPROMErrorIterator = 0;
				mEEPROMMemoryIterator = sizeof(sErrorEEPROMHeader) + ERROR_HANDLER_START_ADDRESS; // starting point of memory iteration is after last byte of header
				return String(iParameter);
				break;
			case ErrorHandler::eFunctionCode::TReadSize:
				lReturn = String(lErrorEepromHeader.ErrorHeader.NumberOfErrors);
				return lReturn;
				break;
			case ErrorHandler::eFunctionCode::TFormat:
				if (GetI2CGlobalEEPROM()->setBlock(ERROR_HANDLER_START_ADDRESS, 0, GetI2CGlobalEEPROM()->getDeviceSize() - ERROR_HANDLER_START_ADDRESS) != 0)
				{
					// EEPROM error => set status back
					mModuleIsInitialized = false;
					return _mText->FormatFailed();
				}
				lErrorEepromHeader.ErrorHeader.NumberOfErrors = 0;
				lErrorEepromHeader.ErrorHeader.NextErrorWritePointer = sizeof(sErrorEEPROMHeader) + ERROR_HANDLER_START_ADDRESS;
				if (!I2EWriteEEPROMHeader(lErrorEepromHeader))
				{
					return _mText->FormatFailed();
				}
				Print(Error::eSeverity::TMessage, _mText->FormatDone());
				return _mText->FormatDone();
				break;
			}
			return _mText->FunctionNameUnknown(iModuleIdentifyer, iParameter);
		}
	}
#endif

	return String("");
}
#endif

#ifdef EXTERNAL_EEPROM
bool ErrorHandler::I2ECheckEEPROMHeader()
{
	DEBUG_METHOD_CALL("ErrorHandler::_I2ECheckEEPROMHeader");

	union uErrorEEPROMHeader lBuffer;

	if (GetI2CGlobalEEPROM() != nullptr)
	{
		// Read EEPROM meta data
		GetI2CGlobalEEPROM()->readBlock(ERROR_HANDLER_START_ADDRESS, lBuffer.Buffer, sizeof(sErrorEEPROMHeader));
		// check checksum
		return (lBuffer.ErrorHeader.Checksum == GetEEPROMHeaderChecksum(lBuffer));
	}
	else
	{
		// EEPROM is not ready, if not initialized
		return false;
	}
}

bool ErrorHandler::I2EWriteEEPROMHeader(union uErrorEEPROMHeader iBuffer)
{
	DEBUG_METHOD_CALL("ErrorHandler::_I2EWriteEEPROMHeader");

	if (GetI2CGlobalEEPROM() != nullptr)
	{
		// calculate next checksum
		iBuffer.ErrorHeader.Checksum = GetEEPROMHeaderChecksum(iBuffer);
		//	DEBUG_PRINT("Write Checksum: " + String(iBuffer.ErrorEntry.Checksum) + ", LastCount: " + String(iBuffer.ErrorEntry.LastCount) + ", StartAddress: " + String(iBuffer.ErrorEntry.StartAddress));

		if (GetI2CGlobalEEPROM()->writeBlock(ERROR_HANDLER_START_ADDRESS, iBuffer.Buffer, sizeof(sErrorEEPROMHeader)) != 0)
		{
			// EEPROM error => set status back
			mModuleIsInitialized = false;
			DEBUG_PRINT_LN("EEPROM write error");
			return false;
		};
		return true;
	}
	else
	{
		// EEPROM is not ready, if not initialized
		return false;
	}
}

char ErrorHandler::GetEEPROMHeaderChecksum(union uErrorEEPROMHeader iBuffer)
{
	DEBUG_METHOD_CALL("ErrorHandler::_GetEEPROMHeaderChecksum");

	char lChecksum = 0;

	for (int lIterator = 1; lIterator < (int)sizeof(sErrorEEPROMHeader); lIterator++)
	{
		lChecksum += (char)iBuffer.Buffer[lIterator];
	}

	return lChecksum;
}
#endif

String ErrorHandler::GetName()
{
	DEBUG_METHOD_CALL("ErrorHandler::GetName");

	return _mText->GetObjectName();
}

void ErrorHandler::Print(Error::eSeverity iSeverity, String iErrorMessage)
{
	DEBUG_METHOD_CALL("ErrorHandler::Print");

	// write transient - no log messages
	if (iSeverity != Error::eSeverity::TMessage)
	{
		mErrorDetected = false;
	}

#ifdef EXTERNAL_EEPROM
	union uErrorEEPROMHeader lErrorEEPROMHeader;
	uint16_t lMessageLength = iErrorMessage.length();

	if (GetI2CGlobalEEPROM() != nullptr)
	{
		// Read EEPROM meta data
		GetI2CGlobalEEPROM()->readBlock(ERROR_HANDLER_START_ADDRESS, lErrorEEPROMHeader.Buffer, sizeof(sErrorEEPROMHeader));
	}
	else
	{
		lErrorEEPROMHeader.ErrorHeader.NumberOfErrors = 0;
	}

	Error *lError = new Error(lErrorEEPROMHeader.ErrorHeader.NumberOfErrors, iSeverity, iErrorMessage);

	// write persistent
	if (GetI2CGlobalEEPROM() != nullptr)
	{
		// check if memory is large enough for data
		if ((lErrorEEPROMHeader.ErrorHeader.NextErrorWritePointer + lMessageLength) > GetI2CGlobalEEPROM()->getDeviceSize() - ERROR_HANDLER_START_ADDRESS)
		{
			DEBUG_PRINT_LN("EEPROM is full");
			return;
		}

		// save data - write error header
		if (GetI2CGlobalEEPROM()->writeBlock(lErrorEEPROMHeader.ErrorHeader.NextErrorWritePointer, (const uint8_t *)lError->GetErrorEntry().ErrorHeader.Buffer, sizeof(Error::sErrorHeader)) != 0)
		{
			// EEPROM error => set status back
			mModuleIsInitialized = false;
			DEBUG_PRINT_LN("EEPROM write error - header");
			return;
		};
		lErrorEEPROMHeader.ErrorHeader.NextErrorWritePointer += sizeof(Error::sErrorHeader);

		// save data - write message
		if (GetI2CGlobalEEPROM()->writeBlock(lErrorEEPROMHeader.ErrorHeader.NextErrorWritePointer, (const uint8_t *)iErrorMessage.begin(), lMessageLength) != 0)
		{
			// EEPROM error => set status back
			mModuleIsInitialized = false;
			DEBUG_PRINT_LN("EEPROM write error - message");
			return;
		};
		lErrorEEPROMHeader.ErrorHeader.NextErrorWritePointer += lMessageLength;

		// save data - write message terminator
		if (GetI2CGlobalEEPROM()->writeByte(lErrorEEPROMHeader.ErrorHeader.NextErrorWritePointer++, '\0') != 0)
		{
			// EEPROM error => set status back
			mModuleIsInitialized = false;
			DEBUG_PRINT_LN("EEPROM write error - message terminator");
			return;
		};

		// get next count of error entries
		lErrorEEPROMHeader.ErrorHeader.NumberOfErrors += 1;

		if (!I2EWriteEEPROMHeader(lErrorEEPROMHeader))
		{
			return;
		};
	}
#endif
}

bool ErrorHandler::ContainsErrors()
{
	DEBUG_METHOD_CALL("ErrorHandler::ContainsErrors");

	return mErrorDetected;
}
