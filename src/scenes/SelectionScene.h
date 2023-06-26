#pragma once

#include "core/Scene.h"

#include <SDL3/SDL.h>

namespace ltrm
{

class SelectionScene : public Scene
{
public:  
  void Load();
  void Unload();
  void Update(float timestep);
};

}
