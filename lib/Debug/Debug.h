// Arduino Base Libs
// 18.10.2021
// Stefan Rau
// Enables output of text via serial interface if debugging is not simply possible on that device

#pragma once
#ifndef _Debug_h
#define _Debug_h

#ifdef DEBUG_APPLICATION

#if DEBUG_APPLICATION < 1000
#define DEBUG_APPLICATION 9600
#endif

#define DebugPrint(Text) Debug::GetInstance()->Print(Text)
#define DebugPrintLn(Text) Debug::GetInstance()->PrintLn(Text)
#define DebugMethodCalls(Text) Debug::GetInstance()->PrintLn(String("Method ") + String(Text))
#define DebugInstantiation(Text) Debug::GetInstance()->PrintLn(String("New ") + String(Text))
#define DebugDestroy(Text) Debug::GetInstance()->PrintLn(String("Destroy ") + String(Text))
// #define DebugMethodCalls(Text) Debug::GetInstance()->PrintLn(Text)
// #define DebugInstantiation(Text) Debug::GetInstance()->PrintLn(Text)
// #define DebugDestroy(Text) Debug::GetInstance()->PrintLn(Text)
#define DebugPrintFromTask(Text) Debug::GetInstance()->PrintFromTask(Text)
#define DebugLoop() Debug::GetInstance()->loop()

#else

#define DebugPrint(Text)
#define DebugPrintLn(Text)
#define DebugMethodCalls(Text)
#define DebugInstantiation(Text)
#define DebugDestroy(Text)
#define DebugPrintFromTask(Text)
#define DebugLoop()

#endif

#ifdef DEBUG_APPLICATION
#include <Arduino.h>

/// <summary>
/// Enables output of text via serial interface if debugging is not simply possible on that device
/// Debugger shall not use text objects - write debugger texts always in English
/// </summary>
class Debug
{
private:
    Debug();
    ~Debug();

    String _mWriteBuffer = "";
    bool _mBufferContainsData = false;

public:
    /// <summary>
    /// Gets a singleton
    /// </summary>
    /// <returns>Instance of debugger</returns>
    static Debug *GetInstance();

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
