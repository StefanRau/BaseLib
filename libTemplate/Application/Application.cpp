// Application template
// 20.11.2022
// Stefan Rau
// History
// 20.11.2022: 1st version - Stefan Rau

#include "Application.h"

static Application *gInstance = nullptr;

Application::Application()
{
  DEBUG_INSTANTIATION("Application");
}

Application::~Application()
{
  DEBUG_DESTROY("Application");
}

Application *Application::GetInstance()
{
  DEBUG_METHOD_CALL("Application::GetInstance");
  gInstance = (gInstance == nullptr) ? new Application() : gInstance;
  return gInstance;
}

void Application::setup()
{
  DEBUG_METHOD_CALL("Application::setup");

}

void Application::loop()
{
  DEBUG_METHOD_CALL("Application::loop");

}
