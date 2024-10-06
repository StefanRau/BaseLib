// Arduino Base Libs
// 18.10.2021
// Stefan Rau
// History
// 18.10.2021: 1st version - Stefan Rau
// 20.06.2022: Debug instantiation of classes - Stefan Rau
// 21.09.2022: use GetInstance instead of Get<Typename> - Stefan Rau
// 26.09.2022: DEBUG_APPLICATION defined in platform.ini - Stefan Rau
// 11.10.2022: remove text object - Stefan Rau
// 11.10.2022: baudrate of remote control can be defined by pragma - Stefan Rau
// 19.10.2022: no usage of String lib anymore => use char and char* only - Stefan Rau
// 05.12.2023: additional output for "const Printable &iOutput"

#include "RemoteControl.h"

#if DEBUG_APPLICATION == 0

static RemoteControl *gInstance = nullptr;

RemoteControl::RemoteControl(char *iBuffer, uint8_t iBufflen)
{
    Serial.begin(SPEED_REMOTE_CONTROL);
    delay(10);
    Serial.println("SRC");
    Serial.flush();
    mWritePosition = 0;
    mOverflowDetected = false;
    mEndDetected = false;
    mBuffer = iBuffer;
    mBufflen = iBufflen;
}

RemoteControl::~RemoteControl()
{
}

RemoteControl *RemoteControl::GetInstance(char *iBuffer, uint8_t iBufflen)
{
    // returns a pointer to singleton instance
    gInstance = (gInstance == nullptr) ? new RemoteControl(iBuffer, iBufflen) : gInstance;
    return gInstance;
}

bool RemoteControl::Available()
{
    // Characters received by input stream are collected in the given buffer and returned after CR or LF is detected.
    // Method must be called from main loop and reads only single characters, that processing is not slowed down.
    int lChar;

    lChar = Serial.read();

    // If end of string is already detected, all further receipts are ignored
    if (mEndDetected || mOverflowDetected)
    {
        return true;
    }

    // Check for end of string
    if (lChar == '\n')
    {
        Serial.println('#'); // Acknowledge receipt of string end
        Serial.flush();
        WriteChar(0);
        mEndDetected = true;
        return true;
    }

    // cummulate buffer
    if (lChar >= 32)
    {
        Serial.print((char)lChar); // Echo last character
        Serial.flush();
        WriteChar((char)lChar);
    }

    return false;
}

void RemoteControl::Read()
{
    // Reset string
    mEndDetected = false;
    mOverflowDetected = false;
    mWritePosition = 0;
    mBuffer[0] = 0;
}

void RemoteControl::Write(char *iOutput)
{
    Serial.print(iOutput);
    Serial.flush();
}

void RemoteControl::Write(const char *iOutput)
{
    Serial.print(iOutput);
    Serial.flush();
}

void RemoteControl::Write(int iOutput)
{
    Serial.print(iOutput);
    Serial.flush();
}

void RemoteControl::Write(const Printable &iOutput)
{
    Serial.print(iOutput);
    Serial.flush();
}

void RemoteControl::Write(String iOutput)
{
    Serial.print(iOutput);
    Serial.flush();
}

void RemoteControl::WriteLn(char *iOutput)
{
    Serial.println(iOutput);
    Serial.flush();
}

void RemoteControl::WriteLn(const char *iOutput)
{
    Serial.println(iOutput);
    Serial.flush();
}

void RemoteControl::WriteLn(int iOutput)
{
    Serial.println(iOutput);
    Serial.flush();
}

void RemoteControl::WriteLn(const Printable &iOutput)
{
    Serial.println(iOutput);
    Serial.flush();
}

void RemoteControl::WriteLn(String iOutput)
{
    Serial.println(iOutput);
    Serial.flush();
}

void RemoteControl::WaitForInput()
{
    Serial.print(">");
    Serial.flush();
}

void RemoteControl::WriteChar(char iChar)
{
    // check for buffer overflow
    if (mWritePosition >= mBufflen)
    {
        mOverflowDetected = true;
    }
    else
    {
        mBuffer[mWritePosition++] = iChar;
    }
}

#endif