#include "scene.h"

#include <SDL3/SDL.h>

namespace ltrm
{

std::unordered_map<std::string, Scene*> SceneManager::s_SceneMap;
Scene* SceneManager::s_ActiveScene = nullptr;

void SceneManager::Init(Scene *scene, std::string key)
{
	s_ActiveScene = scene;
  s_ActiveScene->Load();
  s_SceneMap.try_emplace(key, scene);
}

void SceneManager::Shutdown()
{
  for (auto& scene : s_SceneMap)
  {
    scene.second->Unload();
    delete scene.second;
  }
}

void SceneManager::AddScene(Scene *scene, std::string key)
{
  s_SceneMap.try_emplace(key, scene);
}

void SceneManager::ChangeScene(std::string key)
{
  Scene* scene = s_SceneMap.at(key);
  if (!scene) return;
  
  s_ActiveScene->Unload();
  s_ActiveScene = scene;
  s_ActiveScene->Load();
}

void SceneManager::Update()
{
  s_ActiveScene->Update();
}

void SceneManager::KeyDown(SDL_Keycode key)
{
  s_ActiveScene->KeyDown(key);
}

}
