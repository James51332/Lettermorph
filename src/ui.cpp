#include "ui.h"
#include "input.h"
#include "renderer.h"
#include "style.h"

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

  for (auto pair : s_TextTextures)
  {
    delete pair.second;
  }
}

bool UI::Button(const char* text, float x, float y, bool large)
{
  // Calculate Button Size
  int w, h;
  float width, height;
  ButtonSize(text, &w, &h, large);
  width = static_cast<float>(w);
  height = static_cast<float>(h);
  
  SDL_FRect bounds = {x - width / 2, y - height / 2, width, height};
  SDL_FPoint mouse = {Input::GetMouseX(), Input::GetMouseY()};
  bool highlighted = SDL_PointInRectFloat(&mouse, &bounds);
    
  if (highlighted)
    Renderer::Fill(Color::Middle);
  else
    Renderer::Fill(Color::Dark);
    
  Renderer::Rect(bounds.x, bounds.y, bounds.w, bounds.h);
  UI::Text(text, x, y, large ? Style::LargeScale : Style::SmallScale);
  
	return highlighted && Input::MousePress(SDL_BUTTON_LEFT);
}

void UI::Text(const char* text, float x, float y, float scale)
{
  // If the text hasn't already been rendered, we need to create the texture
  // This system should be fine because our app is very ui light.
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
  w *= scale;
  h *= scale;
  
  Renderer::Image(texture, x - w/2, y - h/2, w, h);
}

void UI::TextSize(const char* text, int* w, int* h, float scale)
{
  TTF_SizeText(s_Font, text, w, h);
  
  if (w) (*w) = static_cast<int>(static_cast<float>(*w) * scale);
  if (h) (*h) = static_cast<int>(static_cast<float>(*h) * scale);
}

void UI::ButtonSize(const char* text, int* w, int* h, bool large)
{
  TextSize(text, w, h, large ? Style::LargeScale : Style::SmallScale);
  if (w) (*w) += 2 * (large ? Style::LargePadding.x : Style::SmallPadding.x);
  if (h) (*h) += 2 * (large ? Style::LargePadding.x : Style::SmallPadding.y);
}

}
