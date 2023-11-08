// Module template
// 26.04.2023
// Stefan Rau
// History
// 26.04.2023: 1st version - Stefan Rau

#include "Module.h"

static Module *gInstance = nullptr;

Module::Module()
{
  DebugInstantiation("Module");
}

Module::~Module()
{
  DebugDestroy("Module");
}

Module *Module::GetInstance()
{
  DebugMethodCalls("Module::GetInstance");
  gInstance = (gInstance == nullptr) ? new Module() : gInstance;
  return gInstance;
}

