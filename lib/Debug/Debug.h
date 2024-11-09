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

#if DEBUG_APPLICATION <= 0
#define DEBUG_START(Number)
#define DEBUG_PRINT(Text)
#define DEBUG_PRINT_LN(Text)
#define DEBUG_LOOP()
#define DEBUG_PRINT_FROM_TASK(Text)
#define DEBUG_BINARY_DUMP(Text, Length)
#define DEBUG_INSTANTIATION(Text)
#define DEBUG_DESTROY(Text)
#define DEBUG_METHOD_CALL(Text)
#endif

#if DEBUG_APPLICATION == 1
#define DEBUG_START(Number) Debug::GetInstance(Number)
#define DEBUG_PRINT(Text) Debug::GetInstance()->Print(Text)
#define DEBUG_PRINT_LN(Text) Debug::GetInstance()->PrintLn(Text)
#define DEBUG_LOOP() Debug::GetInstance()->loop()
#define DEBUG_PRINT_FROM_TASK(Text) Debug::GetInstance()->PrintFromTask(Text)
#define DEBUG_BINARY_DUMP(Text, Length)
#define DEBUG_INSTANTIATION(Text)
#define DEBUG_DESTROY(Text)
#define DEBUG_METHOD_CALL(Text)
#endif

#if DEBUG_APPLICATION == 2
#define DEBUG_START(Number) Debug::GetInstance(Number)
#define DEBUG_PRINT(Text) Debug::GetInstance()->Print(Text)
#define DEBUG_PRINT_LN(Text) Debug::GetInstance()->PrintLn(Text)
#define DEBUG_LOOP() Debug::GetInstance()->loop()
#define DEBUG_PRINT_FROM_TASK(Text) Debug::GetInstance()->PrintFromTask(Text)
#define DEBUG_BINARY_DUMP(Text, Length) Debug::GetInstance()->BinaryDump(Text, Length)
#define DEBUG_INSTANTIATION(Text) Debug::GetInstance()->PrintLn(String("New ") + String(Text))
#define DEBUG_DESTROY(Text) Debug::GetInstance()->PrintLn(String("Destroy ") + String(Text))
#define DEBUG_METHOD_CALL(Text)
#endif

#if DEBUG_APPLICATION >= 3
#define DEBUG_START(Number) Debug::GetInstance(Number)
#define DEBUG_PRINT(Text) Debug::GetInstance()->Print(Text)
#define DEBUG_PRINT_LN(Text) Debug::GetInstance()->PrintLn(Text)
#define DEBUG_LOOP() Debug::GetInstance()->loop()
#define DEBUG_PRINT_FROM_TASK(Text) Debug::GetInstance()->PrintFromTask(Text)
#define DEBUG_BINARY_DUMP(Text, Length) Debug::GetInstance()->BinaryDump(Text, Length)
#define DEBUG_INSTANTIATION(Text) Debug::GetInstance()->PrintLn(String("New ") + String(Text))
#define DEBUG_DESTROY(Text) Debug::GetInstance()->PrintLn(String("Destroy ") + String(Text))
#define DEBUG_METHOD_CALL(Text) Debug::GetInstance()->PrintLn(String("Method ") + String(Text))
#endif

#if DEBUG_APPLICATION > 0

#include <Arduino.h>

/// <summary>
/// Enables output of text via serial interface if debugging is not simply possible on that device
/// Debugger shall not use text objects - write debugger texts always in English
/// </summary>
class Debug
{
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

    // /// <summary>
    // /// Writes debugging text to output
    // /// </summary>
    // /// <param name="iOutput">Text to write</param>
    // void Print(bool iOutput);

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

    // /// <summary>
    // /// Writes debugging text to output
    // /// </summary>
    // /// <param name="iOutput">Text to write</param>
    // void PrintLn(bool iOutput);

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

private:
    Debug(int iCountdown);
    ~Debug();

    String mWriteBuffer = "";
    bool mBufferContainsData = false;
};

#endif

#endif