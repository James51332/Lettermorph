#include "game.h"
#include "renderer.h"
#include "texture.h"
#include "scene.h"
#include "level.h"
#include "menu.h"
#include "dictionary.h"

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
  
  // Dictionary
  Dictionary::Init();
  
  // Create a window
  WindowDesc desc;
  desc.width = 1000;
  desc.height = 800;
  desc.title = "Lettermorph";
  desc.resizeable = true;
  m_Window = new Window(desc);
  
  // Scene Manager
  SceneManager::Init(new MenuScene(), "main");
  SceneManager::AddScene(new LevelScene(), "level");
  
  // Game loop
  while (m_Running)
  {
    m_Window->PollEvents();
    
    SceneManager::Update();
    
    m_Window->SwapBuffers();
  }
  
  // Deinitialize
  delete m_Window;
  
  Dictionary::Shutdown();
  IMG_Quit();
  SDL_Quit();
}

void Game::Stop()
{
  m_Running = false;
}

void Game::KeyDown(SDL_Keycode key)
{
  SceneManager::KeyDown(key);
}

}
