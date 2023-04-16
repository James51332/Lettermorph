#pragma once

#include "scene.h"

#include <SDL3/SDL.h>

namespace ltrm
{

class HelpScene : public Scene
{
public:
  HelpScene();
  ~HelpScene();
  
  void Load();
  void Unload();
  void Update();
  void KeyDown(SDL_Keycode key);
  
private:
  float m_LastTime = 0, m_PulseTime = 0;
};

}
