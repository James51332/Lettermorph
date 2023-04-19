#pragma once

#include "scene.h"

#include <SDL3/SDL.h>

namespace ltrm
{

class SelectionScene : public Scene
{
public:  
  void Load();
  void Unload();
  void Update(float timestep);
  
private:
  float m_PulseTime = 0;
};

}
