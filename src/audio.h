#pragma once

#include <SDL3/SDL_mixer.h>

namespace ltrm
{

class Mixer
{
public:
  static void Init();
  static void Shutdown();
  
  static void Pop();
  static void Click();
  
  static void SetMusicVolume(float volume);
  static void SetFXVolume(float volume);
  
private:
  static Mix_Music* s_Music;
  static int s_Volume;
  
  static Mix_Chunk* s_Pop;
  static Mix_Chunk* s_Click;
};

}
