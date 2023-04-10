#pragma once

#include "scene.h"
#include "word.h"

#include <vector>

namespace ltrm
{

class LevelScene : public Scene
{
public:
  LevelScene();
  ~LevelScene();
  
  void Load();
  void Update();
  void Unload();
  void KeyDown(SDL_Keycode key);
  
private:
  std::vector<Word*> m_Words;
  Word* m_TargetWord;
  bool m_Won = false;
};

}
