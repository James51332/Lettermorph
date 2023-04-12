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
  struct TextTexture
  {
    Texture* Texture = nullptr;
    int Width = 0;
    int Height = 0;
    
    ~TextTexture() { if (Texture) delete Texture; }
  };
  
  static std::unordered_map<const char*, TextTexture*> s_TextTextures;
  static TTF_Font* s_Font;
};

}
