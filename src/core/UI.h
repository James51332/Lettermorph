#pragma once

#include "core/renderer/Texture.h"

#include <SDL3/SDL.h>
#include <SDL3/SDL_ttf.h>
#include <vector>
#include <unordered_map>
#include <string>

namespace ltrm
{

/** Immediate Mode UI System with Buttons, Sliders, and Text
 */
class UI
{
public:
  static void Init();
  static void Shutdown();
  
  static void BeginFrame();
  static void EndFrame();
  
  static bool Button(const char*, float, float, bool large); // Button with automatic padding
  static bool Button(const char*, float, float, float, float, float scale); // Button with fixed size
  
  static bool Slider(float x, float y, float w, float h, float lower, float upper, float& value);
  
  static void Text(const char*, float, float, float scale);
  static void WrappedText(const char*, float, float, uint32_t, float scale);
  
  // Pulse Types:
  // 0 => Don't pulse
  // 1 => Pulse last tile
  // 1 => Pulse all tiles along word
  
  // Sizing:
  // If size == 0, the word is centered horitzontally and vertically around (x, y)
  // If size != 0, the word is left aligned and centered vertically around (x, y)
  static void TiledText(const std::string&, float, float, int pulseType = 0, size_t size = 0);
  
  static void PulseLastTile();
  static void PulseTiles(float delay = 0.0f);
  
  // Helper functions that calculate the size of text
  static void TextSize(const char*, float*, float*, float scale);
  static float TiledTextWidth(size_t length);
  static void ButtonSize(const char*, float*, float*, bool large);
  
private:
  // Whenever text is generated, a texture is created that stores the image.
  // Since UI is static, we can cache these textures and reuse from frame to frame.
  // This is accomplished with a hash table using std::string (of text) as the key.
  // While not the most efficient, it is convenient for the user.
  struct TextTexture
  {
    Texture* Texture = nullptr;
    float Width = 0;
    float Height = 0;
    
    ~TextTexture() { if (Texture) delete Texture; }
  };
  
  static std::unordered_map<std::string, TextTexture*> s_TextTextures;
  static TTF_Font* s_Font;
  
  // This will enable us to do cascaded pulses
  static std::vector<int> s_PulseAnimations;
  
  static bool s_Hovered;
  static int s_HoverAnimation;
  
  static std::vector<bool> m_SliderStates;
  static int m_CurrentSlider;
};

}
