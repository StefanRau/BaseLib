// Application template
// 20.11.2022
// Stefan Rau
// History
// 20.11.2022: 1st version - Stefan Rau

#include "Application.h"

static Application *gInstance = nullptr;

Application::Application()
{
  DebugInstantiation("Application");

}

Application::~Application()
{
  DebugDestroy("Application");
}

Application *Application::GetInstance()
{
  DebugMethodCalls("Application::GetInstance");
  gInstance = (gInstance == nullptr) ? new Application() : gInstance;
  return gInstance;
}

void Application::setup()
{
  DebugMethodCalls("Application::setup");

}

void Application::loop()
{
  DebugMethodCalls("Application::loop");

}
