#pragma once

#include "texture.h"

#include <SDL3/SDL.h>
#include <SDL3/SDL_ttf.h>
#include <vector>
#include <unordered_map>

namespace ltrm
{

class UI
{
public:
  static void Init();
  static void Shutdown();

  static bool Button(const char*, float, float, bool large);
  static void Text(const char*, float, float, float scale);
  
  static void TextSize(const char*, int*, int*, float scale);
  static void ButtonSize(const char*, int*, int*, bool large);
  
private:
  static std::unordered_map<const char*, Texture*> s_TextTextures;
  static TTF_Font* s_Font;
};

}
