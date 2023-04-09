#include "game.h"

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
  
  // Game loop
  while (m_Running)
  {
    m_Window->PollEvents();
    
    SDL_Delay(100);
  }
  
  // Deinitialize
  delete m_Window;
  
  SDL_Quit();
}

void Game::Stop()
{
  m_Running = false;
}

}
