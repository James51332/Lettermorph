#pragma once

#include <SDL3/SDL.h>

namespace ltrm
{

class Texture
{
public:
  Texture(const char* file);
  ~Texture();
  
  SDL_Texture* GetTexture() { return m_Texture; }
  
  void SetClipRect(float x, float y, float w, float h) { m_ClipRect = {x,y,w,h}; }
  const SDL_FRect& GetClipRect() const { return m_ClipRect; }
  bool HasClipRect() const { return m_ClipRect.x != 0 || m_ClipRect.y != 0 || m_ClipRect.w != 0 || m_ClipRect.h != 0; }
  
private:
  SDL_Texture* m_Texture = nullptr;
  SDL_FRect m_ClipRect = {0,0,0,0};
};

}
