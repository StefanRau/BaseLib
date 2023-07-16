// Arduino Base Libs
// 18.10.2021
// Stefan Rau
// Enables output of text via serial interface if debugging is not simply possible on that device

#pragma once
#ifndef _Debug_h
#define _Debug_h

#ifndef DEBUG_SPEED
#define DEBUG_SPEED 9600
#endif

#if DEBUG_APPLICATION == 0
#define DebugStart(Text)
#define DebugPrint(Text)
#define DebugPrintLn(Text)
#define DebugLoop()
#define DebugPrintFromTask(Text)
#define DebugBinaryDump(Text, Length)
#define DebugInstantiation(Text)
#define DebugDestroy(Text)
#define DebugMethodCalls(Text)
#endif

#if DEBUG_APPLICATION == 1
#define DebugStart(Text) Debug::GetInstance(Text)
#define DebugPrint(Text) Debug::GetInstance()->Print(Text)
#define DebugPrintLn(Text) Debug::GetInstance()->PrintLn(Text)
#define DebugLoop() Debug::GetInstance()->loop()
#define DebugPrintFromTask(Text) Debug::GetInstance()->PrintFromTask(Text)
#define DebugBinaryDump(Text, Length)
#define DebugInstantiation(Text)
#define DebugDestroy(Text)
#define DebugMethodCalls(Text)
#endif

#if DEBUG_APPLICATION ==2
#define DebugStart(Text) Debug::GetInstance(Text)
#define DebugPrint(Text) Debug::GetInstance()->Print(Text)
#define DebugPrintLn(Text) Debug::GetInstance()->PrintLn(Text)
#define DebugLoop() Debug::GetInstance()->loop()
#define DebugPrintFromTask(Text) Debug::GetInstance()->PrintFromTask(Text)
#define DebugBinaryDump(Text, Length) Debug::GetInstance()->BinaryDump(Text, Length)
#define DebugInstantiation(Text) Debug::GetInstance()->PrintLn(String("New ") + String(Text))
#define DebugDestroy(Text) Debug::GetInstance()->PrintLn(String("Destroy ") + String(Text))
#define DebugMethodCalls(Text)
#endif

#if DEBUG_APPLICATION >= 3
#define DebugStart(Text) Debug::GetInstance(Text)
#define DebugPrint(Text) Debug::GetInstance()->Print(Text)
#define DebugPrintLn(Text) Debug::GetInstance()->PrintLn(Text)
#define DebugLoop() Debug::GetInstance()->loop()
#define DebugPrintFromTask(Text) Debug::GetInstance()->PrintFromTask(Text)
#define DebugBinaryDump(Text, Length) Debug::GetInstance()->BinaryDump(Text, Length)
#define DebugInstantiation(Text) Debug::GetInstance()->PrintLn(String("New ") + String(Text))
#define DebugDestroy(Text) Debug::GetInstance()->PrintLn(String("Destroy ") + String(Text))
#define DebugMethodCalls(Text) Debug::GetInstance()->PrintLn(String("Method ") + String(Text))
#endif

#if DEBUG_APPLICATION > 0

#include <Arduino.h>

/// <summary>
/// Enables output of text via serial interface if debugging is not simply possible on that device
/// Debugger shall not use text objects - write debugger texts always in English
/// </summary>
class Debug
{
private:
    Debug(int iCountdown);
    ~Debug();

    String _mWriteBuffer = "";
    bool _mBufferContainsData = false;

public:
    /// <summary>
    /// Gets a singleton
    /// </summary>
    /// <returns>Instance of debugger</returns>
    static Debug *GetInstance(int iCountdown = 0);

    /// <summary>
    /// Writes debugging text to output
    /// </summary>
    /// <param name="iOutput">Text to write</param>
    void Print(String iOutput);

    /// <summary>
    /// Writes debugging text to output
    /// </summary>
    /// <param name="iOutput">Text to write</param>
    void Print(const Printable &iOutput);

    /// <summary>
    /// Writes debugging text to output
    /// </summary>
    /// <param name="iOutput">Text to write</param>
    void Print(char iOutput);

    /// <summary>
    /// Writes debugging text to output with line break
    /// </summary>
    /// <param name="iOutput">Text to write</param>
    void PrintLn(String iOutput);

    /// <summary>
    /// Writes debugging text to output with line break
    /// </summary>
    /// <param name="iOutput">Text to write</param>
    void PrintLn(const Printable &iOutput);

    /// <summary>
    /// Writes debugging text to output
    /// </summary>
    /// <param name="iOutput">Text to write</param>
    void PrintLn(char iOutput);

    /// <summary>
    /// Makes a binary dump of memory area
    /// </summary>
    /// <param name="iOutput">Text to write</param>
    void BinaryDump(void *iData, size_t iLength);

    /// <summary>
    /// Writes debugging text into buffer
    /// </summary>
    /// <param name="iOutput">Text to write</param>
    void PrintFromTask(String iOutput);

    /// <summary>
    /// Is called periodically from main loop
    /// </summary>
    void loop();
};

#endif

#endif