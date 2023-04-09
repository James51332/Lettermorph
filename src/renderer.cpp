#include "renderer.h"

namespace ltrm
{

bool Renderer::s_Initalized = false;
SDL_Renderer* Renderer::s_Renderer = nullptr;

void Renderer::Init(SDL_Window *window)
{
  SDL_assert(!s_Initalized); // We can only use one renderer per app (design isn't perfect b/c initialized by non-singleton)
  s_Initalized = true;
  
  // TODO: RenderAPIs
  s_Renderer = SDL_CreateRenderer(window, "opengl", SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
}

void Renderer::Present()
{
  SDL_RenderPresent(s_Renderer);
}

void Renderer::Shutdown()
{
  SDL_DestroyRenderer(s_Renderer);
  s_Renderer = nullptr;
}

void Renderer::Clear(const SDL_Color &color)
{
  SDL_SetRenderDrawColor(s_Renderer, color.r, color.g, color.b, color.a);
  SDL_RenderClear(s_Renderer);
}

void Renderer::Fill(const SDL_Color &color)
{
  SDL_SetRenderDrawColor(s_Renderer, color.r, color.g, color.b, color.a);
}

void Renderer::Rect(float x, float y, float w, float h)
{
  SDL_FRect rect =  {x,y,w,h};
  SDL_RenderFillRect(s_Renderer, &rect);
}

}
