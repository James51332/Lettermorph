#include "scene.h"

#include <SDL3/SDL.h>

namespace ltrm
{

std::unordered_map<std::string, Scene*> SceneManager::s_SceneMap;
Scene* SceneManager::s_ActiveScene = nullptr;

bool SceneManager::s_ChangeRequested;
std::string SceneManager::s_ChangeKey;

void SceneManager::Init(Scene *scene, std::string key)
{
	s_ActiveScene = scene;
  s_ActiveScene->Load();
  s_SceneMap.try_emplace(key, scene);
}

void SceneManager::Shutdown()
{
  s_ActiveScene->Unload();
  for (auto& scene : s_SceneMap)
  {
    delete scene.second;
  }
}

void SceneManager::AddScene(Scene *scene, std::string key)
{
  s_SceneMap.try_emplace(key, scene);
}

void SceneManager::ChangeScene(std::string key)
{
  s_ChangeRequested = true;
  s_ChangeKey = key;
}

void SceneManager::Update()
{
  s_ActiveScene->Update();
  
  if (s_ChangeRequested)
  {
    s_ChangeRequested = false;
    
    Scene* scene = s_SceneMap.at(s_ChangeKey);
    if (!scene)
    {
      SDL_Log("Unknown Scene! (%s)", s_ChangeKey.c_str());
      return;
    }
    
    s_ActiveScene->Unload();
    s_ActiveScene = scene;
    s_ActiveScene->Load();
  }
}

void SceneManager::KeyDown(SDL_Keycode key)
{
  s_ActiveScene->KeyDown(key);
}

}
