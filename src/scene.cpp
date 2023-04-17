#include "scene.h"

#include <SDL3/SDL.h>

namespace ltrm
{

std::unordered_map<std::string, Scene*> SceneManager::s_SceneMap;
Scene* SceneManager::s_ActiveScene = nullptr;

Scene* SceneManager::s_CoveredScene = nullptr;
bool SceneManager::s_Covered = false;
bool SceneManager::s_CoverRequested = false;

bool SceneManager::s_ChangeRequested;
std::string SceneManager::s_ChangeKey;

void SceneManager::Init(Scene *scene, const std::string& key)
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

void SceneManager::AddScene(Scene *scene, const std::string& key)
{
  s_SceneMap.try_emplace(key, scene);
}

void SceneManager::ChangeScene(const std::string& key)
{
  s_ChangeRequested = true;
  s_ChangeKey = key;
}

void SceneManager::CoverScene(const std::string &key)
{
  s_ChangeKey = key;
  s_CoverRequested = true;
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
    
    if (!s_Covered)
    {
    	s_ActiveScene->Unload();
    	s_ActiveScene = scene;
    	s_ActiveScene->Load();
    } else
    {
      s_ActiveScene = s_CoveredScene;
      s_Covered = false;
    }
    
  }
  
  if (s_CoverRequested)
  {
    s_CoverRequested = false;
    
    Scene* scene = s_SceneMap.at(s_ChangeKey);
    if (!scene)
    {
      SDL_Log("Unknown Scene! (%s)", s_ChangeKey.c_str());
      return;
    }
    
    s_CoveredScene = s_ActiveScene;
    s_ActiveScene = scene;
    s_Covered = true;
  }
}

void SceneManager::KeyDown(SDL_Keycode key)
{
  s_ActiveScene->KeyDown(key);
}

}
