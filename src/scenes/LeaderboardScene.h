#pragma once

#include "core/Scene.h"

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
  void Update(float timestep);
  
  static void AddEntry(const std::string& name, int score);
  
private:
  SDL_RWops* m_Leaderboard;
  static std::list<Entry> s_Entries;
};

}
