#include "renderer.h"
#include "game.h"

#include <SDL3/SDL_image.h>

namespace ltrm
{

bool Renderer::s_Initalized = false;
SDL_Renderer* Renderer::s_Renderer = nullptr;
Texture* Renderer::s_FontTexture = nullptr;

void Renderer::Init(SDL_Window *window)
{
  SDL_assert(!s_Initalized); // We can only use one renderer per app (design isn't perfect b/c initialized by non-singleton)
  s_Initalized = true;
  
  // TODO: RenderAPIs
  s_Renderer = SDL_CreateRenderer(window, "opengl", SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  
  s_FontTexture = new Texture("resources/letters.png");
}

void Renderer::Present()
{
  SDL_RenderPresent(s_Renderer);
}

void Renderer::Shutdown()
{
  delete s_FontTexture;
  
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

void Renderer::Image(Texture* tex, float x, float y, float w, float h)
{
  SDL_FRect dst = {x,y,w,h};
  SDL_RenderTexture(s_Renderer, tex->GetTexture(), tex->HasClipRect() ? &tex->GetClipRect() : nullptr, &dst);
}

void Renderer::Letter(char c, float x, float y, float h)
{
  SDL_FRect dstRect = {x,y,h,h};
  
  // Calculate source rect from character
  constexpr int bitmapSize = 512;
  constexpr int letterSize = 64;
  constexpr int lettersPerRow = bitmapSize / letterSize;
  
  int index = ((int)SDL_toupper(c)) - 'A';
  float clipX = static_cast<float>((index % lettersPerRow));
  float clipY = static_cast<float>(((index - clipX) / lettersPerRow));
  float size = static_cast<float>(letterSize);
  SDL_FRect srcRect = { clipX * size, clipY * size, size, size};
  
  SDL_RenderTexture(s_Renderer, s_FontTexture->GetTexture(), &srcRect, &dstRect);
}

float Renderer::GetWidth()
{
  int w;
  SDL_GetRenderOutputSize(s_Renderer, &w, nullptr);
  return static_cast<float>(w);
}

float Renderer::GetHeight()
{
  int h;
  SDL_GetRenderOutputSize(s_Renderer, nullptr, &h);
  return static_cast<float>(h);
}


}
