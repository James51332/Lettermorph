#pragma once

#include "scene.h"

#include <SDL3/SDL.h>

namespace ltrm
{

class EntryScene : public Scene
{
public:
  EntryScene();
  ~EntryScene();
  
  void Load();
  void Unload();
  void Update();
  void KeyDown(SDL_Keycode key);
  
  static void SetScore(int score) { s_Score = score; }
  
private:
  std::string m_Word;
  
  static int s_Score;
};

}
