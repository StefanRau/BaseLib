#pragma once
#ifndef _main_h
#define _main_h

#include <Arduino.h>

#ifdef __cplusplus
extern "C"
{
#endif

    /// <summary>
    /// Called by framework one time
    /// </summary>
    void setup();

    /// <summary>
    /// Called by framework periodically
    /// </summary>
    void loop();

#ifdef __cplusplus
}
#endif

#endif
