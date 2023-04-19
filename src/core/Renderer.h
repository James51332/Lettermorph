#pragma once

#include "core/Window.h"
#include "core/Texture.h"

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
  
  static void Stroke(const SDL_Color& color);
  static void StrokeWeight(float);
  static float GetStrokeWeight() { return s_StrokeWeight; }
  static void NoStroke() { s_StrokeWeight = 0; }
  
  static void Rect(float, float, float, float);
  static void Image(Texture*, float, float, float, float);
  static void Letter(char c, float x, float y, float h);
  
  static float GetWidth();
  static float GetHeight();
  
private:
  static void Init(SDL_Window* window);
  static void Present();
  static void Shutdown();
  
private:
  static bool s_Initalized;
  static SDL_Renderer* s_Renderer;
  static Texture* s_FontTexture;
  
  static float s_StrokeWeight;
  static SDL_Color s_FillColor;
  static SDL_Color s_StrokeColor;
};

}
