// Arduino Base Libs
// 07.11.2023
// Stefan Rau
// Manages multi language texts for Arduino projects

#pragma once
#ifndef _TextWrapper_h
#define _TextWrapper_h

#include "Debug.h"
#include "ProjectBase.h"
#include "TextBase.h"

/// <summary>
/// Local text class of the module
/// </summary>
class TextTextWrapper : public TextBase
{
public:
	TextTextWrapper();
	~TextTextWrapper();

	String GetObjectName() override;
};

/// <summary>
/// This class manages the text handling and languages with saving using ProjectBase. Possible is currently German as default language and English optionally.
/// Reserved language codes are:
/// D: German
/// E: English
/// </summary>
class TextWrapper : public ProjectBase
{
private:
	const int _cEepromIndexLanguage = 1; // Entry used for language

	/// <summary>
	/// Pointer to current text objekt of the class
	/// </summary>
	TextTextWrapper *_mText = nullptr;

#if DEBUG_APPLICATION == 0
	// Remote commands
	enum eFunctionCode : char
	{
		TName = 'L' // Code, if controlled remotely
	};
#endif

public:
	/// <summary>
	/// Constructor
	/// </summary>
	/// <param name="iSettingsAddress">Address of the EEPROM where language code is stored</param>
	TextWrapper(int iSettingsAddress);
	~TextWrapper();

#if DEBUG_APPLICATION == 0
	/// <summary>
	/// Dispatches commands got from en external input, e.g. a serial interface
	/// </summary>
	/// <param name="iModuleIdentifyer">If this matches with the identifyer of this module, then iParameter is analyzed:
	/// 'L' : Command for language operations</param>
	/// <param name="iParameter">Parameter or command that is to be analyzed:
	/// 'D' : Selects German => returns "D"
	/// 'E' : Selects English => returns "E"
	/// '*' : Returns a list of all installed languages => "DE"
	/// '?' : Returns the current language e.g. "D"
	/// </param>
	/// <returns>Reaction of dispatching</returns>
	String DispatchSerial(char iModuleIdentifyer, char iParameter) override;
#endif
};

#endif