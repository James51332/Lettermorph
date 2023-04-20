#pragma once

#include "core/Window.h"

#include <SDL3/SDL.h>

namespace ltrm
{
	
/** Singleton class containing the run loop for the game.
*/
class Game
{
public:
  Game();
  ~Game();
  
  static Game* GetInstance() { return s_Instance; }
  
  void Run();
  void Stop();
  
  void KeyDown(SDL_Keycode key);
  
private:
  void Init();
  void Shutdown();
  
private:
  static Game* s_Instance;
  
  bool m_Running = false;
  Window* m_Window = nullptr;
};

}
