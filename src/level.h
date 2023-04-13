#pragma once

#include "scene.h"

#include <vector>
#include <string>

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
  
  static void SetLevel(int level);
  
private:
  std::vector<std::string> m_Words;
  std::string m_TargetWord;
  bool m_Won = false;
  
  int m_ShakeAnimation;
  
  float m_ScrollOffset;
  int m_ScrollAnimation;
};

}
