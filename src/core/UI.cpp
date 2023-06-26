#include "UI.h"

#include "core/Input.h"
#include "core/renderer/Renderer.h"
#include "core/Style.h"
#include "core/Animation.h"
#include "core/Mixer.h"

#include <SDL3/SDL_ttf.h>

namespace ltrm
{

TTF_Font* UI::s_Font;
std::unordered_map<std::string, UI::TextTexture*> UI::s_TextTextures;

float UI::s_PulseTime = 0;
std::vector<int> UI::s_PulseAnimations;

bool UI::s_Hovered = false;
int UI::s_HoverAnimation;

std::vector<bool> UI::m_SliderStates;
int UI::m_CurrentSlider = 0;

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
  pulse.ResetOnComplete = true;
  pulse.Duration = 0.1;
  for (size_t i = 0; i < 15; i++)
    s_PulseAnimations.push_back(Animator::RegisterAnimation(pulse));
  
  ColorAnimation hover;
  hover.Start = Color::Dark;
  hover.End = Color::Middle;
  hover.Duration = 0.2f;
  hover.ResetOnComplete = false;
  hover.ResetOnInactive = true;
  hover.Loop = false;
  s_HoverAnimation = Animator::RegisterColorAnimation(hover);
}

void UI::Shutdown()
{
  TTF_CloseFont(s_Font);

  for (auto pair : s_TextTextures)
  {
    delete pair.second;
  }
}

void UI::BeginFrame()
{
  m_CurrentSlider = 0; // Reset the slider IDs, so sliders get the same each frame (with static UI)
  s_Hovered = false;
  
  // Handle Pulsing Tiles
  {
    // Init Timer (runs once)
    static float pulseTime = 0.0f;
    static float lastTime = SDL_GetTicks();
    
    // Update (each call)
    float curTime = SDL_GetTicks();
    pulseTime += (curTime - lastTime);
    lastTime = curTime;
    
    if (pulseTime >= 3000.0f) // Time since last pulse >= 3000ms (3 sec)
    {
      pulseTime = 0.0f; // Reset timer
      PulseTiles();
    }
  }
}

void UI::EndFrame()
{
  Animator::SetColorAnimationActive(s_HoverAnimation, s_Hovered);
  if (!s_Hovered) Animator::ResetColorAnimation(s_HoverAnimation);
  
  if (!Input::MouseDown(SDL_BUTTON_LEFT))
  {
    for (size_t i = 0; i < m_SliderStates.size(); i++)
      m_SliderStates[i] = false;
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
  {
    s_Hovered = true;
    Renderer::Fill(Animator::QueryColorAnimation(s_HoverAnimation).Value);
  }
  else
    Renderer::Fill(Color::Dark);
  
  Renderer::StrokeWeight(0);
  Renderer::Rect(bounds.x, bounds.y, bounds.w, bounds.h);
  UI::Text(text, x, y, scale);
  
  bool clicked = highlighted && Input::MousePress(SDL_BUTTON_LEFT);
  if (clicked) Mixer::Click();
  return clicked;
}

bool UI::Slider(float x, float y, float w, float h, float lower, float upper, float &value)
{
  SDL_FRect bounds = {x, y, w, h};
  SDL_FPoint mouse = {Input::GetMouseX(), Input::GetMouseY()};
  float boxSize = h - 2 * Style::SmallMargin;
  float boxLeft = x + (Style::SmallMargin + boxSize / 2);
  float boxSpace = w - (2 * Style::SmallMargin + boxSize);
  
  bool move = Input::MousePress(SDL_BUTTON_LEFT) && SDL_PointInRectFloat(&mouse, &bounds);
  int slider = m_CurrentSlider++;
  if (m_SliderStates.size() <= slider) m_SliderStates.push_back(move);
  if (move) m_SliderStates[slider] = move;
  
  if (m_SliderStates[slider])
  {
    s_Hovered = true;
    
    float mousePercent = (mouse.x - boxLeft) / (boxSpace);
    if (mousePercent < 0) mousePercent = 0;
    if (mousePercent > 1) mousePercent = 1;
    
    value = mousePercent * (upper - lower) + lower;
  }
    
  Renderer::NoStroke();
  Renderer::Fill(Color::Dark);
  Renderer::Rect(x, y, w, h);
  
  float boxPos = (value / (upper - lower)) * (boxSpace) + boxLeft;
	Renderer::Stroke(Color::Light);
  Renderer::StrokeWeight(5);
  Renderer::Fill(m_SliderStates[slider] ? Animator::QueryColorAnimation(s_HoverAnimation).Value : Color::Dark);
  Renderer::Rect(boxPos - boxSize / 2, y + (h - boxSize) / 2 , boxSize, boxSize);
  
  std::string valueText = std::to_string(static_cast<int>(value));
  UI::Text(valueText.c_str(), boxPos, y + h / 2, 0.6f);
  
  return m_SliderStates[slider];
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

void UI::WrappedText(const char* text, float x, float y, uint32_t wrap, float scale)
{
  TextTexture* textTexture;
  
  if (s_TextTextures.find(text) == s_TextTextures.end())
  {
    TTF_SetFontWrappedAlign(s_Font, TTF_WRAPPED_ALIGN_CENTER);
    SDL_Surface* surface = TTF_RenderText_Blended_Wrapped(s_Font, text, Color::Light, wrap / scale);
    
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
  
  // Setup Render Settings for all tiles
  Renderer::NoStroke();
  Renderer::Fill(Color::Dark);
  
  for (int i = 0; i < size; i++)
  {
    float letterSize, pulseValue;
    bool shouldPulse = (pulseType == 2) || (pulseType == 1 && i == word.length() - 1); // Pulse if we pulse all tiles
    
    if (shouldPulse)
    {
      // We use multiple animations for the various tiles. This fetches the correct animation for the tile.
      size_t pulseIndex = (pulseType == 1) ? 0 : i; // 1st element if only tile. nth if sequential.
      pulseValue = Animator::QueryAnimation(s_PulseAnimations[pulseIndex]).Value;
      
      letterSize = Style::LetterSize + pulseValue;
      Renderer::Rect(x - pulseValue / 2, y - pulseValue / 2, Style::TileSize + pulseValue, Style::TileSize + pulseValue);
    }
    else
    {
      letterSize = Style::LetterSize;
      Renderer::Rect(x, y, Style::TileSize, Style::TileSize);
    }
      
    if (i < word.length())
    {
      char c = word[i];
      Renderer::Letter(c, x + Style::TileSize/2 - letterSize / 2, y + Style::TileSize/2 - letterSize / 2, letterSize);
    }
    
    x += Style::TileSize + Style::SmallMargin;
  }
}

void UI::PulseLastTile()
{
  Animator::ResetAnimation(s_PulseAnimations[0]);
  Animator::SetAnimationActive(s_PulseAnimations[0]);
}

void UI::Title(const std::string &title, size_t row)
{
  // Setup Render Settings for all tiles
  Renderer::NoStroke();
  Renderer::Fill(Color::Dark);
  
  // It's not the most optimal, but I don't want to render too many letters in one line.
  size_t size = title.length();
  
  float x = Renderer::GetWidth() * 0.5f - TiledTextWidth(size) * 0.5f;
  float y = Style::SmallMargin + Style::TileSize / 2 + row * (Style::SmallMargin + Style::TileSize);
  
  for (size_t i = 0; i < size; i++)
  {
    // We use multiple animations for the various tiles. This fetches the correct animation for the tile.
    float pulseValue = Animator::QueryAnimation(s_PulseAnimations[i]).Value;
    float letterSize = Style::LetterSize + pulseValue;
    char c = title[i];
  
    Renderer::Rect(x - pulseValue / 2, y - pulseValue / 2, Style::TileSize + pulseValue, Style::TileSize + pulseValue);
    Renderer::Letter(c, x + Style::TileSize / 2 - letterSize / 2, y + Style::TileSize/2 - letterSize / 2, letterSize);
  
    x += Style::TileSize + Style::SmallMargin;
  }
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
