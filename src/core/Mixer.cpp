#include "Mixer.h"

#include <SDL3/SDL.h>

namespace ltrm
{

Mix_Music* Mixer::s_Music;
Mix_Chunk* Mixer::s_Pop;
Mix_Chunk* Mixer::s_Click;
Mix_Chunk* Mixer::s_Win;

void Mixer::Init()
{
  if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) == -1)
  {
    SDL_Log("Failed to Open Audio Device: %s", Mix_GetError());
  }
  
  s_Music = Mix_LoadMUS("resources/lobby-time.mp3");
  if (!s_Music)
  {
    SDL_Log("Failed to load game music: %s", Mix_GetError());
  }
  Mix_PlayMusic(s_Music, -1);
  
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
  
  s_Win = Mix_LoadWAV("resources/win.wav");
  if (!s_Win)
  {
    SDL_Log("Failed to load game music: %s", Mix_GetError());
  }
  
  Mix_VolumeChunk(s_Pop, 32);
  Mix_VolumeChunk(s_Click, 48);
  Mix_VolumeChunk(s_Win, 60);
}

void Mixer::Shutdown()
{
  Mix_FreeChunk(s_Pop);
  Mix_FreeChunk(s_Click);
  
  Mix_FreeMusic(s_Music);
  
  Mix_CloseAudio();
}

void Mixer::Pop()
{
  Mix_PlayChannel(-1, s_Pop, 0);
}

void Mixer::Click()
{
  Mix_PlayChannel(-1, s_Click, 0);
}

void Mixer::Win()
{
  Mix_PlayChannel(-1, s_Win, 0);
}

void Mixer::SetMusicVolume(float volume)
{
  Mix_VolumeMusic(volume * 128);
}

void Mixer::SetFXVolume(float volume)
{
  Mix_VolumeChunk(s_Pop, 32 * volume);
  Mix_VolumeChunk(s_Click, 48 * volume);
  Mix_VolumeChunk(s_Win, 60 * volume);
}

}