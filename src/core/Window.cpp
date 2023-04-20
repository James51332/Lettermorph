#include "Window.h"

#include "core/Game.h"
#include "core/renderer/Renderer.h"
#include "core/Input.h"

namespace ltrm
{

Window::Window(const WindowDesc& desc)
{
  SDL_WindowFlags flags = desc.resizeable ? SDL_WINDOW_RESIZABLE : (SDL_WindowFlags)0;
  flags = static_cast<SDL_WindowFlags>(flags | (desc.fullscreen ? (SDL_WINDOW_FULLSCREEN | SDL_WINDOW_MAXIMIZED) : (SDL_WindowFlags)0));
  m_Handle = SDL_CreateWindow(desc.title.c_str(), desc.width, desc.height, flags);
  
  if (!m_Handle)
  {
    SDL_Log("Failed to create window: %s", SDL_GetError());
    return;
  }
  
  Input::Init();
  Renderer::Init(m_Handle);
  
  // For High DPI Displays
  {
  	int w;
  	SDL_GetRenderWindowSize(Renderer::GetRenderer(), &w, nullptr);
  	m_RenderScale = Renderer::GetWidth() / static_cast<float>(w);
  }
}

Window::~Window()
{
  Renderer::Shutdown();
  Input::Shutdown();
  
  SDL_DestroyWindow(m_Handle);
  m_Handle = nullptr;
}

void Window::PollEvents()
{
  Input::Update();
  
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
        if (event.key.keysym.sym == SDLK_ESCAPE)
        {
          Game::GetInstance()->Stop();
          break;
        }
        
        Input::SetKeyDown(event.key.keysym.sym);
        Game::GetInstance()->KeyDown(event.key.keysym.sym);
        break;
      }
      case SDL_EVENT_KEY_UP:
      {
        Input::SetKeyUp(event.key.keysym.sym);
        break;
      }
      case SDL_EVENT_MOUSE_BUTTON_DOWN:
      {
        Input::SetMouseDown(event.button.button);
        break;
      }
      case SDL_EVENT_MOUSE_BUTTON_UP:
      {
        Input::SetMouseUp(event.button.button);
        break;
      }
      case SDL_EVENT_MOUSE_MOTION:
      {
        Input::SetMousePos(event.motion.x * m_RenderScale, event.motion.y * m_RenderScale);
        break;
      }
        
      default: break;
    }
  }
}

void Window::SwapBuffers()
{
  Renderer::Present();
}

}
