// Arduino Base Libs
// 18.10.2021
// Stefan Rau
// History
// 18.10.2021: 1st version - Stefan Rau
// 27.10.2021: Constructor requires structure - Stefan Rau
// 20.06.2022: Debug instantiation of classes - Stefan Rau
// 21.12.2022: extend destructor - Stefan Rau

#include "I2CBase.h"

/// <summary>
/// There is no new EEPROM address required
/// </summary>
TextI2CBase::TextI2CBase() : TextBase()
{
    DEBUG_INSTANTIATION("TextI2CBase");
}

TextI2CBase::~TextI2CBase()
{
    DEBUG_DESTROY("TextI2CBase");
}

String TextI2CBase::GetObjectName()
{
    return "";
}

String TextI2CBase::ModuleInitialized()
{
    switch (GetLanguage())
    {
        TEXTBASE_LANG_E(" is initialized");
        TEXTBASE_LANG_D(" ist initialisiert");
    }
}

String TextI2CBase::ModuleNotInitialized()
{
    switch (GetLanguage())
    {
        TEXTBASE_LANG_E(" is not initialized");
        TEXTBASE_LANG_D(" ist nicht initialisiert");
    }
}

/////////////////////////////////////////////////////////////

// Module implementation

I2CBase::I2CBase(sInitializeModule iInitializeModule) : ProjectBase(iInitializeModule.SettingsAddress, iInitializeModule.NumberOfSettings)
{
    DEBUG_INSTANTIATION("I2CBase: iInitializeModule[SettingsAddress, NumberOfSettings, I2CAddress]=[" + String(iInitializeModule.SettingsAddress) + ", " + String(iInitializeModule.NumberOfSettings) + ", " + String(iInitializeModule.I2CAddress) + "]");

    mText = new TextI2CBase();

    if (iInitializeModule.I2CAddress >= 0)
    {
        mI2CAddress = iInitializeModule.I2CAddress;
    }
}

I2CBase::~I2CBase()
{
    DEBUG_DESTROY("I2CBase");
}

String I2CBase::GetStatus()
{
    DEBUG_METHOD_CALL("I2CBase::GetStatus");
    // Concatenates name of the module with text, if initialized or not
    String lReturn = GetName();
    lReturn += (mModuleIsInitialized) ? mText->ModuleInitialized() + "\n" : mText->ModuleNotInitialized() + "\n";
    return lReturn;
}

uint8_t I2CBase::Bool2State(bool iValue)
{
    DEBUG_METHOD_CALL("I2CBase::Bool2State");
    return (iValue) ? HIGH : LOW;
}
