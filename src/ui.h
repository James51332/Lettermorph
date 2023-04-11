#pragma once

#include "texture.h"

#include <SDL3/SDL.h>
#include <SDL3/SDL_ttf.h>
#include <vector>
#include <unordered_map>

namespace ltrm
{

// I'm gonna try to create some form of immediate mode gui system
class UI
{
public:
  static void Init();
  static void Shutdown();
  
  static void Begin();
  static void End();
  
  static bool Button(float, float, float, float);
  static bool Button(const char*, float, float);
  static void Text(const char*, float, float);
  
  static void TextSize(const char*, int*, int*);
  
private:
  static std::unordered_map<const char*, Texture*> s_TextTextures;
  static TTF_Font* s_Font;
};

}
