#include "game.h"
#include "renderer.h"
#include "texture.h"
#include "scene.h"
#include "level.h"
#include "menu.h"
#include "settings.h"
#include "selection.h"
#include "dictionary.h"
#include "ui.h"
#include "animation.h"
#include "audio.h"

#include <SDL3/SDL_image.h>
#include <SDL3/SDL_ttf.h>
#include <SDL3/SDL_mixer.h>

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

void Game::Init()
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
    Stop();
    return;
  }
  
  if (TTF_Init() == -1)
  {
    SDL_Log("Failed to initialize SDL_ttf: %s", TTF_GetError());
    Stop();
    return;
  }
  
  if (Mix_Init(MIX_INIT_MP3) == 0)
  {
    SDL_Log("Failed to initialize SDL_mixer: %s", Mix_GetError());
    Stop();
    return;
  }
  
  Animator::Init();
  Dictionary::Init();
  UI::Init();
  Mixer::Init();
  
  // Create a window
  WindowDesc desc;
  desc.width = 1000;
  desc.height = 800;
  desc.title = "Lettermorph";
  desc.resizeable = true;
  m_Window = new Window(desc);
  
  // Scene Manager
  SceneManager::Init(new MenuScene(), "main");
  SceneManager::AddScene(new SelectionScene(), "selection");
  SceneManager::AddScene(new LevelScene(), "level");
  SceneManager::AddScene(new SettingsScene(), "settings");
}

void Game::Shutdown()
{
  // Deinitialize
  delete m_Window;
  
  SceneManager::Shutdown();
  Dictionary::Shutdown();
  UI::Shutdown();
  Mixer::Shutdown();
  Animator::Shutdown();
  Mix_Quit();
  TTF_Quit();
  IMG_Quit();
  SDL_Quit();
}

// We're going to contain the entire life cycle of the app here.
// All resources should be deinitialized before the destructor is called.
void Game::Run()
{
  Init();
  
  // Game loop
  float lastTime = SDL_GetTicks();
  while (m_Running)
  {
    m_Window->PollEvents();
    
    float timestep = (SDL_GetTicks() - lastTime) / 1000.0f;
    lastTime = SDL_GetTicks();
    Animator::Update(timestep);
    
    SceneManager::Update();
    
    m_Window->SwapBuffers();
  }
  
  Shutdown();
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
