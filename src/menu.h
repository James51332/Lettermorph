#pragma once

#include "scene.h"

#include <SDL3/SDL.h>

namespace ltrm
{

class MenuScene : public Scene
{
public:
  MenuScene();
  ~MenuScene();
  
  void Load();
  void Unload();
  void Update();
  void KeyDown(SDL_Keycode key);
  
private:
};

}