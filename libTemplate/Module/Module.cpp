// Module template
// 26.04.2023
// Stefan Rau
// History
// 26.04.2023: 1st version - Stefan Rau

#include "Module.h"

static Module *gInstance = nullptr;

Module::Module()
{
  DEBUG_INSTANTIATION("Module");
}

Module::~Module()
{
  DEBUG_DESTROY("Module");
}

Module *Module::GetInstance()
{
  DEBUG_METHOD_CALL("Module::GetInstance");
  gInstance = (gInstance == nullptr) ? new Module() : gInstance;
  return gInstance;
}

