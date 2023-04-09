#include "game.h"
#include "renderer.h"
#include "texture.h"

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
  
  // Initailize SDL
  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS))
  {
    SDL_Log("Failed to initialize SDL: %s", SDL_GetError());
    Stop();
    return;
  }
  
  if (!IMG_Init(IMG_INIT_PNG))
  {
    SDL_Log("Failed to initialize SDL_image: %s", IMG_GetError());
    Game::GetInstance()->Stop();
    return;
  }
  
  // Create a window
  WindowDesc desc;
  desc.width = 300;
  desc.height = 500;
  desc.title = "Lettermorph";
  desc.resizeable = true;
  m_Window = new Window(desc);
  
  // Load an image
  Texture* texture = new Texture("resources/discord-btn.png");
  
  // Game loop
  while (m_Running)
  {
    m_Window->PollEvents();
    
    Renderer::Clear({0x10, 0x10, 0x10, 0xff});
    
    Renderer::Fill({0, 0, 0, 255});
    Renderer::Rect(100, 200, 200, 300);
    Renderer::Letter('F', 0, 0, 100);
    
    m_Window->SwapBuffers();
  }
  
  // Deinitialize
  delete texture;
  delete m_Window;
  
  IMG_Quit();
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
