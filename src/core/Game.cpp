#include "Game.h"

#include "core/Renderer.h"
#include "core/Texture.h"
#include "core/Scene.h"
#include "core/Dictionary.h"
#include "core/UI.h"
#include "core/Animation.h"
#include "core/Mixer.h"

#include "scenes/LevelScene.h"
#include "scenes/MenuScene.h"
#include "scenes/SettingsScene.h"
#include "scenes/SelectionScene.h"
#include "scenes/HelpScene.h"
#include "scenes/LeaderboardScene.h"
#include "scenes/EntryScene.h"

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
  desc.title = "Lettermorph";
  desc.resizeable = false;
  desc.fullscreen = true;
  m_Window = new Window(desc);
  
  // Scene Manager
  SceneStack::Init(new MenuScene(), "main");
  SceneStack::AddScene(new SelectionScene(), "selection");
  SceneStack::AddScene(new LevelScene(), "level");
  SceneStack::AddScene(new SettingsScene(), "settings");
  SceneStack::AddScene(new HelpScene(), "help");
  SceneStack::AddScene(new LeaderboardScene, "leaderboard");
  SceneStack::AddScene(new EntryScene(), "entry");
}

void Game::Shutdown()
{
  // Deinitialize
  delete m_Window;
  
  SceneStack::Shutdown();
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
    
    UI::Begin();
    SceneStack::Update(timestep);
    UI::End();
    
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
  SceneStack::KeyDown(key);
}

}
