#pragma once

#include "core/Scene.h"

#include <SDL3/SDL.h>

namespace ltrm
{

class SelectionScene : public Scene
{
public:  
  void Update(float timestep);
};

}
