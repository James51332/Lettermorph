#pragma once

#include <SDL3/SDL.h>
#include <vector>

namespace ltrm
{

// The animation system for the game should be relatively simple.
// Anytime an animation is needed, a new animation should be registered
// with the animator, which will update the animation each frame, and return
// an adjusted value which can be queried by the application. Usually, one
// float should be enough to animate whatever we need. However, we'll also
// create a color animation to represent fades between colors.

enum class AnimationType
{
	Lerp,
  Wave,
  Pulse
};

struct Animation
{
  AnimationType Type = AnimationType::Lerp;
  
  // TODO: We could use bit logic to make this type smaller (probably unneeded though)
  bool Active = false;
  bool ResetOnInactive = true;
  bool ResetOnComplete = false;
  bool Loop = true;
  
  float Value = 0;
  float Progress = 0;
  float Duration = 1;
  float Max = 1;
  float Min = 0;
};

struct ColorAnimation
{
  bool Active = false;
  bool ResetOnInactive = true;
  bool ResetOnComplete = false;
  bool Loop = false;
  
  SDL_Color Value = {0, 0, 0, 255};
  float Progress = 0;
  float Duration = 1;
  SDL_Color Start = {0, 0, 0, 255};
  SDL_Color End = {255, 255, 255, 255};
};

class Animator
{
public:
  static void Init();
  static void Shutdown();
  
  static int RegisterAnimation(const Animation& animation);
  static int RegisterColorAnimation(const ColorAnimation& animation);
  
  static void SetAnimationActive(int ID, bool active = true);
  static void SetColorAnimationActive(int ID, bool active = true);
  static void ResetAnimation(int ID);
  static void ResetColorAnimation(int ID);
  
  static const Animation& QueryAnimation(int ID);
  static const ColorAnimation& QueryColorAnimation(int ID);
  
  static void Update(float deltaTime);
  
private:
  static int s_CurrentID;
  static int s_CurrentColorID;
  
  // It should be fine to store these by value (copy only occurs on register)
  static std::vector<Animation> s_Animations;
  static std::vector<ColorAnimation> s_ColorAnimations;
};

}
