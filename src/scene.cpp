#include "scene.h"

#include <SDL3/SDL.h>

namespace ltrm
{

std::unordered_map<std::string, Scene*> SceneStack::s_SceneMap;
std::vector<Scene*> SceneStack::s_SceneStack;

SceneStack::ChangeType SceneStack::s_ChangeType = ChangeType::None;
Scene* SceneStack::s_ChangeScene;

void SceneStack::Init(Scene *scene, const std::string& key)
{
  AddScene(scene, key);
  s_SceneStack.push_back(scene);
}

void SceneStack::AddScene(Scene *scene, const std::string& key)
{
  auto [iterator, success] = s_SceneMap.try_emplace(key, scene);
  if (!success) SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Failed to add scene with key %s", key.c_str());
}

void SceneStack::Shutdown()
{
  // Delete all scenes in the SceneStack
  for (auto& scene : s_SceneMap)
  {
    delete scene.second;
  }
}

void SceneStack::Reload()
{
  s_ChangeType = ChangeType::Reload;
}

void SceneStack::ChangeScene(const std::string& key)
{
  s_ChangeType = ChangeType::Change;
  s_ChangeScene = GetScene(key);
}

void SceneStack::PushScene(const std::string &key)
{
  s_ChangeType = ChangeType::Push;
  s_ChangeScene = GetScene(key);
}

void SceneStack::PopScene()
{
  s_ChangeType = ChangeType::Pop;
}

void SceneStack::ClearStack()
{
  s_ChangeType = ChangeType::Clear;
}

void SceneStack::Update(float timestep)
{
  // First update the active scene
  s_SceneStack.back()->Update(timestep);
  
  // If a scene change is requested, move to the other scene.
  switch (s_ChangeType)
  {
    case ChangeType::None: break;
    case ChangeType::Change:
    {
      // We can be silent here because an error will be thrown already, just don't switch
      if (!s_ChangeScene) return;
      
      s_SceneStack.back()->Unload();
      s_SceneStack.back() = s_ChangeScene;
      s_SceneStack.back()->Load();
      break;
    }
    case ChangeType::Push:
    {
      if (!s_ChangeScene) return;
      
      s_SceneStack.back()->Unload();
      s_SceneStack.push_back(s_ChangeScene);
      s_SceneStack.back()->Load();
      break;
    }
    case ChangeType::Pop:
    {
      if (s_SceneStack.size() <= 1)
      {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Failed to pop scene at bottom of Scene Stack!");
        return;
      }

      s_SceneStack.back()->Unload();
      s_SceneStack.pop_back();
      s_SceneStack.back()->Load();
      break;
    }
    case ChangeType::Clear:
    {
      s_SceneStack.back()->Unload();
      while (s_SceneStack.size() > 1) s_SceneStack.pop_back();
      s_SceneStack.back()->Load();
      
      break;
    }
    case ChangeType::Reload:
    {
      s_SceneStack.back()->Unload();
      s_SceneStack.back()->Load();
      break;
    }
  }
  
  // Reset the change state
  s_ChangeType = ChangeType::None;
  s_ChangeScene = nullptr;
}

void SceneStack::KeyDown(SDL_Keycode key)
{
  s_SceneStack.back()->KeyDown(key);
}

Scene* SceneStack::GetScene(const std::string &key)
{
  Scene* tmp = s_SceneMap.at(key);
  if (!tmp) SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Failed to find scene with key: %s", key.c_str());
  return tmp;
}

}
