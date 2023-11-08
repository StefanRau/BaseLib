// Arduino Base Libs
// 07.11.2023
// Stefan Rau
// History
// 07.11.2023: 1st version - Stefan Rau

#include "TextWrapper.h"

/// <summary>
/// Dummy instance for setting and getting language
/// </summary>
TextTextWrapper::TextTextWrapper() : TextBase()
{
	DebugInstantiation("TextTextWrapper");
}

TextTextWrapper::~TextTextWrapper()
{
	DebugDestroy("TextTextWrapper");
}

String TextTextWrapper::GetObjectName()
{
	return "";
}

/////////////////////////////////////////////////////////////

TextWrapper::TextWrapper(int iSettingsAddress) : ProjectBase(iSettingsAddress, 1)
{
	DebugInstantiation("TextWrapper: SettingsAddress=" + String(iSettingsAddress));

	_mText = new TextTextWrapper();

	if (iSettingsAddress >= 0)
	{
		_mText->SetLanguage(GetSetting(_cEepromIndexLanguage));
	}
}

TextWrapper::~TextWrapper()
{
	DebugDestroy("TextWrapper");
}

#if DEBUG_APPLICATION == 0
String TextWrapper::DispatchSerial(char iModuleIdentifyer, char iParameter)
{

	switch (iModuleIdentifyer)
	{
	case eFunctionCode::TName:

		if (iParameter == ProjectBase::eFunctionCode::TParameterGetCurrent)
		{
			return ProjectBase::GetVerboseMode() ? _mText->GetSelectedLanguageName() : String(_mText->GetLanguage());
		}
		else if (iParameter == ProjectBase::eFunctionCode::TParameterGetAll)
		{
			return _mText->GetValidLanguages(ProjectBase::GetVerboseMode());
		}
		else if (_mText->GetValidLanguages(false).indexOf(iParameter) >= 0)
		{
			_mText->SetLanguage(iParameter);
			return String(iParameter);
		}

		return String(ProjectBase::eFunctionCode::TReturnUnknown);
	}

	return "";
}
#endif
