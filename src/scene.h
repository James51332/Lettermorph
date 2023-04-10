#pragma once

#include <SDL3/SDL.h>
#include <string>
#include <unordered_map>

namespace ltrm
{

// The game will always have an active scene
class Scene
{
public:
  virtual ~Scene() = default;
  
  virtual void Load() = 0;
  virtual void Update() = 0;
  virtual void Unload() = 0;
  virtual void KeyDown(SDL_Keycode key) = 0;
};

class SceneManager
{
public:
  static void Init(Scene* scene, std::string key = "main");
  static void Shutdown();
  
  static void AddScene(Scene* scene, std::string key);
  static void ChangeScene(std::string key);
  
  static void Update();
  static void KeyDown(SDL_Keycode key);
  
private:
  static std::unordered_map<std::string, Scene*> s_SceneMap;
  static Scene* s_ActiveScene;
};

}
