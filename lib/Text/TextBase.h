// Arduino Base Libs
// 18.10.2021
// Stefan Rau
// Manages multi language texts for Arduino projects

#pragma once
#ifndef _TextBase_h
#define _TextBase_h

#include <Arduino.h>
#include "Debug.h"

#define TEXT_DEFAULT_LANGUAGE 'D'
#define TEXT_VALID_LANGUAGES "DE"

#define TextLangD(Text) \
	default:            \
		return Text;    \
		break
#define TextLangE(Text) \
	case 'E':           \
		return Text;    \
		break

/// <summary>
/// This class manages the text handling and languages. Possible is currently German as default language and English optionally.
/// Reserved language codes are:
/// D: German
/// E: English
/// </summary>
class TextBase
{
protected:
	/// <summary>
	/// Constructor
	/// </summary>
	TextBase();
	~TextBase();

public:
	/// <summary>
	/// Returns a list of all installed languages
	/// </summary>
	/// <param name="iVerbose">true: output concatenated language texts; false: output concatenated language codes</param>
	/// <returns>String with single character language codes</returns>
	String GetValidLanguages(bool iVerbose);

	/// <summary>
	/// Sets a language
	/// </summary>
	/// <param name="iLanguage">One character with language code</param>
	void SetLanguage(char iLanguage);

	/// <summary>
	/// Returns the current language
	/// </summary>
	/// <returns>One character with language code</returns>
	char GetLanguage();

	/// <summary>
	/// Returns the name of the currently selected language
	/// </summary>
	/// <returns>Readable name</returns>
	String GetSelectedLanguageName();

	/// <summary>
	/// Returns the readable name of the object where the derivation of this text object is implemented
	/// </summary>
	/// <returns>Object name</returns>
	virtual String GetObjectName() = 0;

	String LanguageEnglish();
	String LanguageGerman();
};

#endif