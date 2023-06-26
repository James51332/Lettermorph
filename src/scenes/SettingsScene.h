#pragma once

#include "core/Scene.h"

#include <SDL3/SDL.h>

namespace ltrm
{

class SettingsScene : public Scene
{
public:  
  void Update(float timestep);
  
private:
  float m_MusicVolume = 100, m_FXVolume = 100;
};

}

