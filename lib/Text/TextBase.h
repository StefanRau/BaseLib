// Arduino Base Libs
// 18.10.2021
// Stefan Rau
// Manages multi language texts for Arduino projects

#pragma once
#ifndef _TextBase_h
#define _TextBase_h

#include <Arduino.h>

#define TEXTBASE_LANGUAGE_D 'D'
#define TEXTBASE_LANGUAGE_E 'E'
#define TEXTBASE_DEFAULT_LANGUAGE TEXTBASE_LANGUAGE_E
#define TEXTBASE_VALID_LANGUAGES "DE"

#if TEXTBASE_LANGUAGE_E == TEXTBASE_LANGUAGE_D
#define TEXTBASE_LANG_D(Text) \
	default:            \
		return Text;    \
		break;
#define TEXTBASE_ACTION_LANG_D(Target, Text) \
	default:                      \
		Text;                     \
		return Target;            \
		break;
#else
#define TEXTBASE_LANG_D(Text)   \
	case TEXTBASE_LANGUAGE_D: \
		return Text;      \
		break;
#define TEXTBASE_ACTION_LANG_D(Target, Text) \
	case TEXTBASE_LANGUAGE_D:         \
		Text;                     \
		return Target;            \
		break;
#endif

#if TEXTBASE_LANGUAGE_E == TEXTBASE_LANGUAGE_E
#define TEXTBASE_LANG_E(Text) \
	default:            \
		return Text;    \
		break;
#define TEXTBASE_ACTION_LANG_E(Target, Text) \
	default:                      \
		Text;                     \
		return Target;            \
		break;
#else
#define TEXTBASE_LANG_E(Text)   \
	case TEXTBASE_LANGUAGE_E: \
		return Text;      \
		break;
#define TEXTBASE_ACTION_LANG_E(Target, Text) \
	case TEXTBASE_LANGUAGE_E:         \
		Text;                     \
		return Target;            \
		break;
#endif

/// <summary>
/// This class manages the text handling and languages. Possible is currently German as default language and English optionally.
/// Reserved language codes are:
/// D: German
/// E: English
/// </summary>
class TextBase
{
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

protected:
	/// <summary>
	/// Constructor
	/// </summary>
	TextBase();
	~TextBase();

private:
	String LanguageEnglish();
	String LanguageGerman();
};

#endif