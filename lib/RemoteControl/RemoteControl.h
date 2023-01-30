// Arduino Base Libs
// 18.10.2021
// Stefan Rau
// Remote control class, that encapsulates serial access

#pragma once
#ifndef _RemoteControl_h
#define _RemoteControl_h

#if DEBUG_APPLICATION == 0
#ifndef SPEED_REMOTE_CONTROL
#define SPEED_REMOTE_CONTROL 9600
#endif

#include <Arduino.h>

class RemoteControl
{
private:
	char *_mBuffer;			 // Receiver buffer - given by caller
	int _mBufflen;			 // Length of the buffer
	int _mWritePosition;	 // Position of write pointer
	bool _mEndDetected;		 // true, if the end of a string is detected
	bool _mOverflowDetected; // true, if more bytes are received, than fit into buffer

	RemoteControl(char *iBuffer, uint8_t iBufflen);
	~RemoteControl();

	void WriteChar(char iChar);

public:
	/// <summary>
	/// Gets a singleton
	/// </summary>
	/// <returns>Instance of remote control</returns>
	/// <param name="iBuffer">Pointer to string buffer</param>
	static RemoteControl *GetInstance(char *iBuffer, uint8_t iBufflen);

	/// <summary>
	/// Detects the end of a string, received from serial interface.
	/// Cummulates single characters into buffer from serial interface until CR or LF is detected.
	/// More received characters than iBufflen from GetInstance are ignored.
	/// </summary>
	/// <returns>Set to true after CR or LF was received from serial interface</returns>
	bool Available();

	/// <summary>
	/// Receives a command string from serial interface. Must be called in a cycle.
	/// Cummulates single characters in buffer from serial interface until CR or LF is detected.
	/// </summary>
	void Read();

	/// <summary>
	/// Outputs a string to serial interface
	/// </summary>
	/// <param name="iOutput">String to output</param>
	void Write(char *iOutput);
};

#endif

#endif