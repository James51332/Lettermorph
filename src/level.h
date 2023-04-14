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
  size_t m_WordLength = 0;
  
  bool m_Won = false;
  static int s_Level;
  
  int m_ShakeAnimation;
  
  float m_ScrollOffset;
  int m_ScrollAnimation;
};

}
