#include "ui.h"
#include "input.h"
#include "renderer.h"
#include "colors.h"

#include <SDL3/SDL_ttf.h>

namespace ltrm
{

TTF_Font* UI::s_Font;
std::unordered_map<const char*, Texture*> UI::s_TextTextures;

void UI::Init()
{
	s_Font = TTF_OpenFont("resources/Arial.ttf", 100);
  if (!s_Font)
  {
    SDL_Log("Failed to load font: %s", TTF_GetError());
  }
}

void UI::Shutdown()
{
  TTF_CloseFont(s_Font);

  // TODO: Delete all textures in map
}

void UI::Begin()
{
  
}

void UI::End()
{
}

bool UI::Button(float x, float y, float w, float h)
{
  SDL_FRect bounds = {x,y,w,h};
  SDL_FPoint mouse = {Input::GetMouseX(), Input::GetMouseY()};
  bool highlighted = SDL_PointInRectFloat(&mouse, &bounds);
  
  if (highlighted)
    Renderer::Fill(Color::Light);
  else
    Renderer::Fill(Color::Dark);
  
  Renderer::Rect(x, y, w, h);
  return highlighted && Input::MousePress(SDL_BUTTON_LEFT);
}

void UI::Text(const char* text, float x, float y)
{
  // If the text hasn't already been rendered, we need to create the texture
  Texture* texture;
  
  if (s_TextTextures.find(text) == s_TextTextures.end())
  {
    SDL_Surface* surface = TTF_RenderText_Blended(s_Font, text, Color::Light);
    texture = new Texture(SDL_CreateTextureFromSurface(Renderer::GetRenderer(), surface));
    s_TextTextures.try_emplace(text, texture);
    SDL_DestroySurface(surface);
  } else
  {
    texture = s_TextTextures.at(text);
  }
  
  int w, h;
  SDL_QueryTexture(texture->GetTexture(), nullptr, nullptr, &w, &h);
  Renderer::Image(texture, x - w/2, y - h/2, w, h);
}

}
