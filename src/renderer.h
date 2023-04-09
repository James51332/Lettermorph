#pragma once

#include "window.h"
#include "texture.h"

#include <SDL3/SDL.h>

namespace ltrm
{

class Renderer
{
  friend class Window;
public:
  static SDL_Renderer* GetRenderer() { return s_Renderer; }
  
  static void Clear(const SDL_Color& color);
  static void Fill(const SDL_Color& color);
  
  static void Rect(float, float, float, float);
  static void Image(Texture*, float, float, float, float);
  
private:
  static void Init(SDL_Window* window);
  static void Present();
  static void Shutdown();
  
private:
  static bool s_Initalized;
  static SDL_Renderer* s_Renderer;
};

}
