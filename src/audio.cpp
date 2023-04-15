#include "audio.h"

#include <SDL3/SDL.h>

namespace ltrm
{

Mix_Music* Mixer::s_Music;
Mix_Chunk* Mixer::s_Pop;
Mix_Chunk* Mixer::s_Click;

void Mixer::Init()
{
  Mix_OpenAudio(22050, AUDIO_S32SYS, 8, 1280);

  
  s_Music = Mix_LoadMUS("resources/lobby-time.mp3");
  if (!s_Music)
  {
    SDL_Log("Failed to load game music: %s", Mix_GetError());
  }
  Mix_PlayMusic(s_Music, 0);
  
  s_Pop = Mix_LoadWAV("resources/pop.wav");
  if (!s_Pop)
  {
    SDL_Log("Failed to load game music: %s", Mix_GetError());
  }
  
  s_Click = Mix_LoadWAV("resources/click.wav");
  if (!s_Click)
  {
    SDL_Log("Failed to load game music: %s", Mix_GetError());
  }
  
  Mix_VolumeChunk(s_Pop, 32);
  Mix_VolumeChunk(s_Click, 48);
}

void Mixer::Shutdown()
{
  Mix_FreeMusic(s_Music);
  Mix_FreeChunk(s_Pop);
  Mix_FreeChunk(s_Click);
  
  Mix_CloseAudio();
}

void Mixer::Pop()
{
  int channel = 0;
  while (Mix_Playing(channel) && channel < 16) channel++;
  Mix_PlayChannelTimed(channel, s_Pop, 0, 50);
}

void Mixer::Click()
{
  int channel = 0;
  while (Mix_Playing(channel) && channel < 16) channel++;
  Mix_PlayChannelTimed(channel, s_Click, 0, 50);
}

}
