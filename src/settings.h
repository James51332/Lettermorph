#pragma once

#include "scene.h"

#include <SDL3/SDL.h>

namespace ltrm
{

class SettingsScene : public Scene
{
public:
  SettingsScene();
  ~SettingsScene();
  
  void Load();
  void Unload();
  void Update();
  void KeyDown(SDL_Keycode key);
  
private:
  float m_PulseTime = 0, m_LastTime = 0;
  float m_MusicVolume = 100, m_FXVolume = 100;
};

}

