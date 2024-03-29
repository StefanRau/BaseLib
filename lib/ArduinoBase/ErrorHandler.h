// Arduino Base Libs
// 18.10.2021
// Stefan Rau
// Records and handles errors inclusive multiple errors

#pragma once
#ifndef _ErrorHandler_h
#define _ErrorHandler_h

#include <Arduino.h>
#include <time.h>
#include "Debug.h"
#include "List.h"
#include "I2CBase.h"

#ifndef EXTERNAL_EEPROM
#warning No storage for error log
#endif

/// <summary>
/// Local text class of the module
/// </summary>
class TextErrorHandler : public TextBase
{
public:
	TextErrorHandler();
	~TextErrorHandler();

	String GetObjectName() override;
#ifdef EXTERNAL_EEPROM
	String FunctionNameUnknown(char iModuleIdentifyer, char iParameter);
	String FormatDone();
	String FormatFailed();
	String ErrorListDone();
	String SeverityMessage();
	String SeverityWarning();
	String SeverityError();
	String SeverityFatal();
	String SeverityUnknown();
#endif
};

/////////////////////////////////////////////////////////////

/// <summary>
/// This instance contains all information about one error
/// </summary>
class Error
{

public:
	/// <summary>
	/// Error severity
	/// </summary>
	enum class eSeverity : char
	{
		TMessage = 'M',
		TWarning = 'W',
		TError = 'E', // shall be raised if a hardware module can't be initialized - fallback shall be possible here
		TFatal = 'F'  // shall be raised if a required hardware module can't be initialized
	};

	struct sErrorHeader
	{
		eSeverity Severity; // severity of an error message
		int Count;			// error count
		tm Time;			// time of the error message
	};

	union uErrorHeader
	{
		sErrorHeader ErrorHeader;
		uint8_t Buffer[sizeof(sErrorHeader)];
	};

	struct sErrorEntry
	{
		union uErrorHeader ErrorHeader; // serializable header of an error message
		String ErrorMessage;			// the error message itself
	};

private:
	sErrorEntry mErrorEntry; // Text of the current error message

public:
	/// <summary>
	/// Constructor
	/// </summary>
	/// <param name="iSeverity">Severtity of the new error message</param>
	/// <param name="iErrorMessage">Text of the new error message</param>
	Error(int iNumber, eSeverity iSeverity, String iErrorMessage);
	~Error();

	/// <summary>
	/// Gets the current error
	/// </summary>
	/// <returns>A complete error structure</returns>
	sErrorEntry GetErrorEntry();
};

#define ERROR_PRINT(iSeverity, iErrorMessage) ErrorHandler::GetInstance()->Print(iSeverity, iErrorMessage)
#define ERROR_DETECTED() ErrorHandler::GetInstance()->ContainsErrors()

#if defined(ARDUINO_AVR_NANO_EVERY) or defined(ARDUINO_AVR_ATTINYX4) or defined(ARDUINO_AVR_ATTINYX5) or defined(ARDUINO_AVR_ATmega8) or defined(ARDUINO_AVR_DIGISPARK)
#define ERROR_HANDLER_START_ADDRESS 0x000 // uses internal EEPROM for settings
#endif
#if defined(ARDUINO_SAMD_NANO_33_IOT) or defined(ARDUINO_ARDUINO_NANO33BLE) or defined(ARDUINO_NANO_RP2040_CONNECT)
#define ERROR_HANDLER_START_ADDRESS 0x0100 // 1st address for logging
#endif

class ErrorHandler : public I2CBase
{
private:
	struct sErrorEEPROMHeader
	{
		char Checksum;
		uint16_t NextErrorWritePointer;
		short NumberOfErrors;
	};

	union uErrorEEPROMHeader
	{
		sErrorEEPROMHeader ErrorHeader;
		uint8_t Buffer[sizeof(sErrorEEPROMHeader)];
	};

	TextErrorHandler *_mText = nullptr;													   // Pointer to current text objekt of the class
	int mEEPROMMemoryIterator = sizeof(sErrorEEPROMHeader) + ERROR_HANDLER_START_ADDRESS; // pointer to address of next error log item, initially that's the bype after the last entry of header
	int mEEPROMErrorIterator = 0;														   // number of next error log item
	bool mErrorDetected = false;														   // signals that an error was detected

#if DEBUG_APPLICATION == 0
	// Commands for remote control
	enum class eFunctionCode : char
	{
		TName = 'E',	  // Code for this class, if controlled remotely
		TFormat = 'F',	  // Formatting of EEPROM
		TReadNext = 'R',  // Read the next item of the error log and increase pointer to error log item
		TReadReset = '0', // Reset pointer to error log item
		TReadSize = 'S'	  // Get number of error entries
	};
#endif

	/// <summary>
	/// Constructor
	/// </summary>
	/// <param name="iInitializeModule">Structure that contains EEPROM settings address (or starting address) as well as I2C address (or starting address) of the module</param>
	ErrorHandler(sInitializeModule iInitializeModule);
	~ErrorHandler();

public:
	// Functions that can be called from within main loop

	/// <summary>
	/// Can be called periodically from main loop => makes no sense for error handler
	/// </summary>
	void loop() override;

#if DEBUG_APPLICATION == 0
	/// <summary>
	/// Dispatches commands got from en external input, e.g. a serial interface - only a dummy implementation here
	/// </summary>
	/// <param name="iModuleIdentifyer">If this matches with the identifyer of this module, then iParameter is analyzed</param>
	/// <param name="iParameter">Parameter or command that is to be analyzed</param>
	/// <returns>Reaction of dispatching</returns>
	String DispatchSerial(char iModuleIdentifyer, char iParameter) override;
#endif

private:
#ifdef EXTERNAL_EEPROM
	/// <summary>
	/// Checks the header of the logger EEPROM
	/// </summary>
	/// <returns>true: EEPROM is o.k., false: EEPROM is not o.k.</returns>
	bool I2ECheckEEPROMHeader();

	/// <summary>
	/// Writes the header of the logger EEPROM
	/// </summary>
	/// <param name="iBuffer">Header to write</param>
	/// <returns>true: EEPROM is o.k., false: EEPROM is not o.k.</returns>
	bool I2EWriteEEPROMHeader(union uErrorEEPROMHeader iBuffer);

	// Functions that can be called from everywhere

	/// <summary>
	/// Returns the check sum of the EEPROM header
	/// </summary>
	/// <returns>checksum value</returns>
	char GetEEPROMHeaderChecksum(union uErrorEEPROMHeader iBuffer);
#endif

public:
	/// <summary>
	/// Gets a singleton. If InitializeErrorHandler was not called before, logging isn't be used.
	/// </summary>
	/// <returns>Instance of the error handler</returns>
	static ErrorHandler *GetInstance();

	/// <summary>
	/// Write a new error message
	/// </summary>
	/// <param name="iSeverity">Severtity of the new error message</param>
	/// <param name="iErrorMessage">Text of the new error message</param>
	void Print(Error::eSeverity iSeverity, String iErrorMessage);

	/// <summary>
	/// Checks if the error list contains error or fatal
	/// </summary>
	/// <returns>true: the list of errors contains one, false: there was no error ot fatal</returns>
	bool ContainsErrors();

	/// <summary>
	/// Readable name of the module
	/// </summary>
	/// <returns>Gets the current name depending on current language</returns>
	String GetName() override;
};

#endif
