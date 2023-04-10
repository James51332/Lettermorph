#pragma once

#include "window.h"
#include "word.h"

#include <SDL3/SDL.h>

namespace ltrm
{
	
class Game
{
public:
  Game();
  ~Game();
  
  void Run();
  void Stop();
  void KeyDown(SDL_Keycode key);
  
  static Game* GetInstance() { return s_Instance; }
  
private:
  static Game* s_Instance;
  
  bool m_Running = false;
  Window* m_Window = nullptr;
};

}
