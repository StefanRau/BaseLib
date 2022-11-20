#include <Arduino.h>
#include "Application.h"

Application *gApplication = nullptr;

void setup()
{
  DebugInstantiation("Start Application");
  gApplication = Application::GetInstance();
  gApplication->setup();
}

void loop()
{
  gApplication->loop();
}