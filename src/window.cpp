#include "window.h"
#include "game.h"

namespace ltrm
{

Window::Window(const WindowDesc& desc)
{
  SDL_WindowFlags flags = desc.resizeable ? SDL_WINDOW_RESIZABLE : (SDL_WindowFlags)0;
  m_Handle = SDL_CreateWindow(desc.title.c_str(), desc.width, desc.height, flags);
}

Window::~Window()
{
  SDL_DestroyWindow(m_Handle);
}

void Window::PollEvents()
{
  SDL_Event event;
  while (SDL_PollEvent(&event))
  {
    switch (event.type)
    {
      case SDL_EVENT_QUIT:
      {
        Game::GetInstance()->Stop();
        break;
      }
        
      default:
        break;
    }
  }
}

}
