// Arduino Base Libs
// 18.10.2021
// Stefan Rau
// History
// 18.10.2021: 1st version - Stefan Rau
// 21.10.2021: Dispatcher has now chars as input - Stefan Rau
// 20.06.2022: Debug instantiation of classes - Stefan Rau
// 26.09.2022: DEBUG_APPLICATION defined in platform.ini - Stefan Rau
// 21.12.2022: extend destructor - Stefan Rau

#include "TextBase.h"

static char gTextBaseLanguage; // Stores the language that is used for all text objects

TextBase::TextBase(int iSettingsAddress) : ProjectBase(iSettingsAddress, 1)
{
	DebugInstantiation("TextI2CBase: SettingsAddress=" + String(iSettingsAddress));

	if (iSettingsAddress >= 0)
	{
		gTextBaseLanguage = GetSetting(_cEepromIndexLanguage);
	}

	if (_cValidLanguages.indexOf(gTextBaseLanguage) < 0)
	{
		// If language was never set before, take the default language
		gTextBaseLanguage = _cDefaultLanguage;
		SetSetting(_cEepromIndexLanguage, gTextBaseLanguage);
	}
}

TextBase::~TextBase()
{
	DebugDestroy("TextBase");
}

String TextBase::GetValidLanguages()
{
	return _cValidLanguages;
}

void TextBase::SetLanguage(char iLanguage)
{
	if (_cValidLanguages.indexOf(iLanguage) >= 0)
	{
		gTextBaseLanguage = iLanguage;
		SetSetting(_cEepromIndexLanguage, gTextBaseLanguage);
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
	case 'D':
		return LanguageGerman();
	case 'E':
		return LanguageEnglish();
	}

	return LanguageGerman();
}

#ifndef DEBUG_APPLICATION
String TextBase::DispatchSerial(char iModuleIdentifyer, char iParameter)
{

	switch (iModuleIdentifyer)
	{
	case eFunctionCode::TName:

		if (iParameter == ProjectBase::eFunctionCode::TParameterGetCurrent)
		{
			return ProjectBase::GetVerboseMode() ? GetSelectedLanguageName() : String(gTextBaseLanguage);
		}
		else if (iParameter == ProjectBase::eFunctionCode::TParameterGetAll)
		{
			return ProjectBase::GetVerboseMode() ? LanguageGerman() + ',' + LanguageEnglish() : String(_cValidLanguages);
		}
		else if (_cValidLanguages.indexOf(iParameter) >= 0)
		{
			SetLanguage(iParameter);
			return String(iParameter);
		}

		return String(ProjectBase::eFunctionCode::TReturnUnknown);
	}

	return "";
}
#endif

String TextBase::LanguageEnglish()
{
	switch (GetLanguage())
	{
		TextLangE("English");
		TextLangD("Englisch");
	}
}

String TextBase::LanguageGerman()
{
	switch (GetLanguage())
	{
		TextLangE("German");
		TextLangD("Deutsch");
	}
}
