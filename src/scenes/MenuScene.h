#pragma once

#include "core/Scene.h"

#include <SDL3/SDL.h>

namespace ltrm
{

class MenuScene : public Scene
{
public:
  void Load();
  void Unload();
  void Update(float timestep);
  void KeyDown(SDL_Keycode key);
  
private:
  float m_PulseTime = 0;
};

}
