#pragma once

#include "core/Scene.h"

#include <SDL3/SDL.h>

namespace ltrm
{

class HelpScene : public Scene
{
public:  
  void Load();
  void Unload();
  void Update(float timestep);
};

}
