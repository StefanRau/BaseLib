// Arduino Base Libs
// 18.10.2021
// Stefan Rau
// History
// 18.10.2021: 1st version - Stefan Rau
// 21.10.2021: Dispatcher has now chars as input - Stefan Rau
// 20.06.2022: Debug instantiation of classes - Stefan Rau
// 26.09.2022: DEBUG_APPLICATION defined in platform.ini - Stefan Rau
// 21.12.2022: extend destructor - Stefan Rau
// 07.11.2023: Class is now independent of EEPROM settings, that is moved to class TextWrapper if required - Stefan Rau

#include "TextBase.h"
#include "Debug.h"

static char gTextBaseLanguage = TEXTBASE_DEFAULT_LANGUAGE; // Stores the language that is used for all text objects

TextBase::TextBase()
{
	DEBUG_INSTANTIATION("TextBase");
}

TextBase::~TextBase()
{
	DEBUG_DESTROY("TextBase");
}

String TextBase::GetValidLanguages(bool iVerbose)
{
	if (iVerbose)
	{
		return LanguageGerman() + ',' + LanguageEnglish();
	}
	else
	{
		return TEXTBASE_VALID_LANGUAGES;
	}
}

void TextBase::SetLanguage(char iLanguage)
{
	if (String(TEXTBASE_VALID_LANGUAGES).indexOf(iLanguage) >= 0)
	{
		gTextBaseLanguage = iLanguage;
	}
}

char TextBase::GetLanguage()
{
	return gTextBaseLanguage;
}

String TextBase::GetSelectedLanguageName()
{
	switch (gTextBaseLanguage)
	{
	case TEXTBASE_LANGUAGE_D:
		return LanguageGerman();
	case TEXTBASE_LANGUAGE_E:
		return LanguageEnglish();
	}

	return LanguageGerman();
}

String TextBase::LanguageEnglish()
{
	switch (GetLanguage())
	{
		TEXTBASE_LANG_E("English");
		TEXTBASE_LANG_D("Englisch");
	}
}

String TextBase::LanguageGerman()
{
	switch (GetLanguage())
	{
		TEXTBASE_LANG_E("German");
		TEXTBASE_LANG_D("Deutsch");
	}
}
