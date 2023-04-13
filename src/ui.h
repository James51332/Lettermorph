#pragma once

#include "texture.h"

#include <SDL3/SDL.h>
#include <SDL3/SDL_ttf.h>
#include <vector>
#include <unordered_map>
#include <string>

namespace ltrm
{

class UI
{
public:
  static void Init();
  static void Shutdown();

  static bool Button(const char*, float, float, bool large); // Button with automatic padding
  static bool Button(const char*, float, float, float, float, float scale); // Button with fixed size
  
  static void Text(const char*, float, float, float scale);
  
  static void TextSize(const char*, float*, float*, float scale);
  static void ButtonSize(const char*, float*, float*, bool large);
  
private:
  struct TextTexture
  {
    Texture* Texture = nullptr;
    float Width = 0;
    float Height = 0;
    
    ~TextTexture() { if (Texture) delete Texture; }
  };
  
  static std::unordered_map<std::string, TextTexture*> s_TextTextures;
  static TTF_Font* s_Font;
};

}
