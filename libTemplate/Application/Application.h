// Application template
// 20.11.2022
// Stefan Rau

#pragma once
#ifndef _Application_h
#define _Application_h

#include <Arduino.h>
#include "Debug.h"

class Application
{
public:
    /// <summary>
    /// Gets a singleton.
    /// </summary>
    /// <returns>Instance of this class</returns>
    static Application *GetInstance();

    /// <summary>
    /// Called by framework one time
    /// </summary>
    void setup();

    /// <summary>
    /// Called by framework periodically
    /// </summary>
    void loop();

protected:
    // Libraries

    // Settings

private:
    /// <summary>
    /// Constructor
    /// </summary>
    Application();
    ~Application();
};

#endif