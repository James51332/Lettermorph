#pragma once

#include <SDL3/SDL.h>

namespace ltrm
{

struct Color
{
  constexpr static SDL_Color Light = { 0xeb, 0xeb, 0xeb, 0xff };
  constexpr static SDL_Color Middle = { 0xd0, 0xcd, 0xe0, 0xff };
  constexpr static SDL_Color Dark = { 0x37, 0x37, 0x37, 0xff };
  constexpr static SDL_Color Accent = { 0x00, 0x90, 0x63, 0xff };
};

struct Style
{
  constexpr static float SmallScale = 0.5f;
  constexpr static float LargeScale = 1.0f;
  
  constexpr static SDL_FPoint SmallPadding{20, 10};
  constexpr static SDL_FPoint LargePadding{50, 30};
  constexpr static float SmallMargin = 20;
  
  constexpr static float TileSize = 200;
  constexpr static float LetterSize = 90;
};

}
