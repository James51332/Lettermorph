#pragma once

#include <SDL3/SDL.h>
#include <string>
#include <unordered_map>
#include <vector>

namespace ltrm
{

/**
 * Pure virtual type that can be inherited from. Child types can be added to Manager
 * A scene is effectively a layer in the game. One scene is rendered at any given time.
 * The active scene is updated once per frame.
 */
class Scene
{
public:
  virtual ~Scene() = default;
  
  virtual void Load() {}
  virtual void Unload() {}
  virtual void Update(float timestep) = 0;
  
  // This function doesn't need to be overriden
  virtual void KeyDown(SDL_Keycode key) {}
};

/**
 * Maintains a stack of scenes. The top of the scene is always of the scene that is
 * presented to the user.
 *
 * Some menu ui should be pushed on top of other scenes, and can be popped back to.
 * Changing the scene results in a switch of the top level scene.
 *
 * All scenes are referred to by std::string names.
 */
class SceneStack
{
public:
  static void Init(Scene* scene, const std::string& key);
  static void AddScene(Scene* scene, const std::string& key);

  static void Shutdown();
  
  static void Reload();
  
  // Set's the scene at the top of the scene stack
  static void ChangeScene(const std::string& key);
  
  // Pushes a new scene on the top of the scene stack
  static void PushScene(const std::string& key);
  static void PopScene();
  
  static void ClearStack();
  
  static void Update(float timestep);
  static void KeyDown(SDL_Keycode key);
  
private:
  static Scene* GetScene(const std::string& key);
private:
  // Allows us to refer to scenes by name
  static std::unordered_map<std::string, Scene*> s_SceneMap;
  
  // A stack of the scenes. The top of the scene stack is always presentd.
  static std::vector<Scene*> s_SceneStack;
  
  // We don't want to change a scene during a frame, so this stores the data for after.
  enum class ChangeType
  {
    None,
    Change,
    Push,
    Pop,
    Clear,
    Reload
  };
  
  static ChangeType s_ChangeType;
  static Scene* s_ChangeScene;
};

}
