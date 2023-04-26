// Module template
// 26.04.2023
// Stefan Rau

#pragma once
#ifndef _Module_h
#define _Modulen_h

#include <Arduino.h>
#include "Debug.h"

class Module
{
public:
    /// <summary>
    /// Gets a singleton.
    /// </summary>
    /// <returns>Instance of this class</returns>
    static Module *GetInstance();

protected:
    // Libraries

    // Settings

private:
    /// <summary>
    /// Constructor
    /// </summary>
    Module();
    ~Module();
};

#endif