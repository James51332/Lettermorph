#include "ui.h"
#include "input.h"
#include "renderer.h"
#include "style.h"
#include "animation.h"

#include <SDL3/SDL_ttf.h>

namespace ltrm
{

TTF_Font* UI::s_Font;
std::unordered_map<std::string, UI::TextTexture*> UI::s_TextTextures;

std::vector<int> UI::s_PulseAnimations;

void UI::Init()
{
	s_Font = TTF_OpenFont("resources/Arial.ttf", 80);
  if (!s_Font)
  {
    SDL_Log("Failed to load font: %s", TTF_GetError());
  }
  
  Animation pulse;
  pulse.Type = AnimationType::Pulse;
  pulse.Min = 0;
  pulse.Max = 10;
  pulse.Loop = false;
  pulse.Duration = 0.1;
  for (size_t i = 0; i < 10; i++)
    s_PulseAnimations.push_back(Animator::RegisterAnimation(pulse));
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
  float width, height;
  ButtonSize(text, &width, &height, large);
  return Button(text, x, y, width, height, large ? Style::LargeScale : Style::SmallScale);
}

bool UI::Button(const char* text, float x, float y, float w, float h, float scale)
{
  SDL_FRect bounds = {x - w / 2, y - h / 2, w, h};
  SDL_FPoint mouse = {Input::GetMouseX(), Input::GetMouseY()};
  bool highlighted = SDL_PointInRectFloat(&mouse, &bounds);
  
  if (highlighted)
    Renderer::Fill(Color::Middle);
  else
    Renderer::Fill(Color::Dark);
  
  Renderer::StrokeWeight(0);
  Renderer::Rect(bounds.x, bounds.y, bounds.w, bounds.h);
  UI::Text(text, x, y, scale);
  
  return highlighted && Input::MousePress(SDL_BUTTON_LEFT);
}

void UI::Text(const char* text, float x, float y, float scale)
{
  // If the text hasn't already been rendered, we need to create the texture
  // This system should be fine because our app is very ui light.
  TextTexture* textTexture;
  
  if (s_TextTextures.find(text) == s_TextTextures.end())
  {
    SDL_Surface* surface = TTF_RenderText_Blended(s_Font, text, Color::Light);
    
    textTexture = new TextTexture();
    textTexture->Texture = new Texture(SDL_CreateTextureFromSurface(Renderer::GetRenderer(), surface));
    
    int w, h;
    SDL_QueryTexture(textTexture->Texture->GetTexture(), nullptr, nullptr, &w, &h);;
    textTexture->Width = w;
    textTexture->Height = h;
    
    s_TextTextures.try_emplace(std::string(text), textTexture);
    
    SDL_DestroySurface(surface);
  } else
  {
    textTexture = s_TextTextures.at(std::string(text));
  }
  
  float sw = textTexture->Width * scale;
  float sh = textTexture->Height * scale;
  Renderer::Image(textTexture->Texture, x - sw/2, y - sh/2, sw, sh);
}

void UI::TiledText(const std::string& word, float x, float y, int pulseType, size_t size)
{
  if (size == 0) // Center if the word is not given with a length
  {
    x -= TiledTextWidth(word.length()) / 2;
    size = word.length();
  }
  
  for (int i = 0; i < size; i++)
  {
    float centerX = x + Style::TileSize / 2;
    float centerY = y + Style::TileSize / 2;
    
    Renderer::NoStroke();
    Renderer::Fill(Color::Dark);
    
    float letterSize, pulse;
    if (pulseType == 2 || (pulseType == 1 && i == word.length() - 1))
    {
      size_t pulseIndex = pulseType == 1 ? 0 : i;
      pulse = Animator::QueryAnimation(s_PulseAnimations[pulseIndex]).Value;
      letterSize = Style::LetterSize + pulse;
    	Renderer::Rect(x - pulse / 2, y - pulse / 2, Style::TileSize + pulse, Style::TileSize + pulse);
    }
    else
    {
      letterSize = Style::LetterSize;
      Renderer::Rect(x, y, Style::TileSize, Style::TileSize);
    }
      
    if (i < word.length())
    {
      char c = word[i];
      Renderer::Letter(c, centerX - letterSize / 2, centerY - letterSize / 2, letterSize);
    }
    
    x += Style::TileSize + Style::SmallMargin;
  }
}

void UI::PulseLastTile()
{
  Animator::ResetAnimation(s_PulseAnimations[0]);
  Animator::SetAnimationActive(s_PulseAnimations[0]);
}

void UI::PulseTiles(float delay)
{
  for (auto id : s_PulseAnimations)
  {
    Animator::QueueAnimationActive(id, delay);
    delay += 0.08f;
  }
}

void UI::TextSize(const char* text, float* w, float* h, float scale)
{
  int tw, th;
  TTF_SizeText(s_Font, text, &tw, &th);
  
  if (w) (*w) = static_cast<float>(static_cast<float>(tw) * scale);
  if (h) (*h) = static_cast<float>(static_cast<float>(th) * scale);
}

float UI::TiledTextWidth(size_t length)
{
  return length * (Style::TileSize + Style::SmallMargin) - Style::SmallMargin;
}


void UI::ButtonSize(const char* text, float* w, float* h, bool large)
{
  TextSize(text, w, h, large ? Style::LargeScale : Style::SmallScale);
  if (w) (*w) += static_cast<float>(2 * (large ? Style::LargePadding.x : Style::SmallPadding.x));
  if (h) (*h) += static_cast<float>(2 * (large ? Style::LargePadding.x : Style::SmallPadding.y));
}

}
