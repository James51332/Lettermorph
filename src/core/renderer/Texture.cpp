#include "Texture.h"

#include "core/renderer/Renderer.h"

#include <SDL3/SDL_image.h>

namespace ltrm
{

Texture::Texture(const char* file)
{
  SDL_Surface* surface = IMG_Load(file);
  if (!surface)
  {
    SDL_Log("Failed to load image: %s", IMG_GetError());
    return;
  }
  
  m_Texture = SDL_CreateTextureFromSurface(Renderer::GetRenderer(), surface);
  SDL_DestroySurface(surface);
  if (!m_Texture)
  {
    SDL_Log("Failed to create texture from surface: %s", SDL_GetError());
    return;
  }
}

Texture::Texture(SDL_Texture* texture)
{
  m_Texture = texture;
}

Texture::~Texture()
{
  SDL_DestroyTexture(m_Texture);
}

}
