#pragma once

#include <SDL3/SDL.h>
#include <vector>

namespace ltrm
{

// I'm gonna try to create some form of immediate mode gui system
class UI
{
public:
  static void Init();
  static void Shutdown();
  
  static void Begin();
  static void End();
  
  static bool Button(float, float, float, float);
};

}
