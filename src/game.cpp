#include "game.h"
#include "renderer.h"

#include <SDL3/SDL_image.h>

namespace ltrm
{

Game* Game::s_Instance = nullptr;

Game::Game()
{
  SDL_assert(!s_Instance);
  s_Instance = this;
}

Game::~Game()
{
}

// We're going to contain the entire life cycle of the app here.
// All resources should be deinitialized before the destructor is called.
void Game::Run()
{
  // We begin running here, because we may terminate before actual gameloop begins
  m_Running = true;
  
  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS))
  {
    SDL_Log("Failed to initialize SDL: %s", SDL_GetError());
    Stop();
    return;
  }
  
  // Create a window
  WindowDesc desc;
  desc.width = 300;
  desc.height = 500;
  desc.title = "Lettermorph";
  desc.resizeable = true;
  m_Window = new Window(desc);
  
  if ( TTF_Init() < 0 ) {
    SDL_Log("Error initializing SDL_ttf: %s", TTF_GetError());
  }
  
  // Game loop
  while (m_Running)
  {
    m_Window->PollEvents();
    
    Renderer::Clear({0xcc, 0xcc, 0xcc, 0xff});
    
    Renderer::Fill({100, 100, 0, 255});
    Renderer::Rect(100, 200, 200, 300);
    
    m_Window->SwapBuffers();
  }
  
  // Deinitialize
  delete m_Window;
  
  SDL_Quit();
}

void Game::Stop()
{
  m_Running = false;
}

void Game::KeyDown(SDL_Keycode key)
{
  SDL_Log("Key Pressed, %c", SDL_toupper(key));
}

}
