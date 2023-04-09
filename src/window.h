#pragma once

#include <string>
#include <SDL3/SDL.h>

namespace ltrm
{

struct WindowDesc
{
  uint32_t width;
  uint32_t height;
  std::string title;
  bool resizeable = false;
  
  WindowDesc(uint32_t w = 800, uint32_t h = 600, std::string title = "App")
  	: width(w), height(h), title(title) {}
};

class Window
{
public:
  Window(const WindowDesc& desc);
  ~Window();
  
  void PollEvents();
  const WindowDesc& GetDesc() const { return m_Desc; }
  
private:
  WindowDesc m_Desc;
  SDL_Window* m_Handle;
};

}
