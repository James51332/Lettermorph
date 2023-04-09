#pragma once

#include "window.h"

namespace ltrm
{
	
class Game
{
public:
  Game();
  ~Game();
  
  void Run();
  void Stop();
  
  static Game* GetInstance() { return s_Instance; }
  
private:
  static Game* s_Instance;
  
  bool m_Running = false;
  Window* m_Window = nullptr;
};

}
