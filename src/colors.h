#pragma once

#include <SDL3/SDL.h>

namespace ltrm
{

// To keep a sense of unity throughout the design of the game
struct Color
{
  constexpr static SDL_Color Light = { 0xeb, 0xeb, 0xeb, 0xff };
  constexpr static SDL_Color Dark = { 0x47, 0x45, 0x54, 0xff };
  constexpr static SDL_Color Accent = { 0xac, 0xa7, 0xcb, 0xff };
};

}
