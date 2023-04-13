#pragma once

#include "scene.h"

#include <SDL3/SDL.h>

namespace ltrm
{

class SelectionScene : public Scene
{
public:
  SelectionScene();
  ~SelectionScene();
  
  void Load();
  void Unload();
  void Update();
  void KeyDown(SDL_Keycode key);
};

}
