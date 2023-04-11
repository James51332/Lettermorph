#include "ui.h"
#include "input.h"
#include "renderer.h"

namespace ltrm
{

void UI::Init()
{
  
}

void UI::Shutdown()
{
  
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
    Renderer::Fill({0xff,0x00,0x00,0xff});
  else
    Renderer::Fill({0x55,0x55,0x55,0xff});
  
  Renderer::Rect(x, y, w, h);
  return highlighted && Input::MousePress(SDL_BUTTON_LEFT);
}

}
