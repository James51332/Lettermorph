#pragma once

#include "scene.h"

#include <SDL3/SDL.h>
#include <string>
#include <list>

namespace ltrm
{

class LeaderboardScene : public Scene
{
  struct Entry
  {
    std::string Name;
    int Score;
    
    Entry(const std::string& name, int score)
    	: Name(name), Score(score) {}
  };
  
public:
  LeaderboardScene();
  ~LeaderboardScene();
  
  void Load();
  void Unload();
  void Update();
  void KeyDown(SDL_Keycode key);
  
  static void AddEntry(const std::string& name, int score);
  
private:
  float m_LastTime = 0, m_PulseTime = 0;
  
  SDL_RWops* m_Leaderboard;
  static std::list<Entry> s_Entries;
};

}
