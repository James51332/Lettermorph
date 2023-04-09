#include "window.h"
#include "game.h"
#include "renderer.h"

namespace ltrm
{

Window::Window(const WindowDesc& desc)
{
  SDL_WindowFlags flags = desc.resizeable ? SDL_WINDOW_RESIZABLE : (SDL_WindowFlags)0;
  m_Handle = SDL_CreateWindow(desc.title.c_str(), desc.width, desc.height, flags);
  
  Renderer::Init(m_Handle);
}

Window::~Window()
{
  Renderer::Shutdown();
  
  SDL_DestroyWindow(m_Handle);
  m_Handle = nullptr;
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
        
      case SDL_EVENT_KEY_DOWN:
      {
        Game::GetInstance()->KeyDown(event.key.keysym.sym);
      }
        
      default:
        break;
    }
  }
}

void Window::SwapBuffers()
{
  Renderer::Present();
}

}
