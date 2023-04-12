#include "animation.h"


namespace ltrm
{

int Animator::s_CurrentID = 0;
int Animator::s_CurrentColorID = 0;

std::vector<Animation> Animator::s_Animations;
std::vector<ColorAnimation> Animator::s_ColorAnimations;

void Animator::Init()
{
  
}

void Animator::Shutdown()
{
  
}

int Animator::RegisterAnimation(const Animation& animation)
{
  s_Animations.push_back(animation);
  return s_CurrentID++;
}

int Animator::RegisterColorAnimation(const ColorAnimation &animation)
{
  s_ColorAnimations.push_back(animation);
  return s_CurrentColorID++;
}

void Animator::SetAnimationActive(int ID, bool active)
{
  s_Animations[ID].Active = active;
}

void Animator::SetColorAnimationActive(int ID, bool active)
{
  s_ColorAnimations[ID].Active = active;
}

void Animator::ResetAnimation(int ID)
{
  Animation& anim = s_Animations[ID];
  
  switch (anim.Type)
  {
    case AnimationType::Lerp:
    {
      anim.Progress = 0;
      anim.Value = anim.Min;
      break;
    }
    case AnimationType::Wave:
    {
      // TODO
      break;
    }
  }
}

void Animator::ResetColorAnimation(int ID)
{
  ColorAnimation& anim = s_ColorAnimations[ID];
  
  anim.Progress = 0;
  anim.Value = anim.Start;
}

const Animation& Animator::QueryAnimation(int ID)
{
  return s_Animations[ID];
}

const ColorAnimation& Animator::QueryColorAnimation(int ID)
{
  return s_ColorAnimations[ID];
}

static void lerpUpdate(Animation& animation, float timestep)
{
  if (!animation.Active)
  {
    if (!animation.ResetOnInactive && animation.Progress < 1) return;
    if (!animation.ResetOnComplete && animation.Progress == 1) return;
    
    animation.Value = animation.Min;
    animation.Progress = 0;
  } else
  {
    animation.Progress += timestep / animation.Duration;
    animation.Value = animation.Min + animation.Progress * (animation.Max - animation.Min);
    
    if (animation.Value >= animation.Max)
    {
      animation.Value = animation.Max; // We'll still keep it at the end until the next frame
      
      if (!animation.Loop)
      {
        animation.Active = false;
        animation.Progress = 1;
      } else
      {
        animation.Progress = 0;
      }
    }
  }
}

static void waveUpdate(Animation& animation, float timestep)
{
  if (!animation.Active)
  {
    if (!animation.ResetOnInactive && animation.Progress < 1) return;
    if (!animation.ResetOnComplete && animation.Progress == 1) return;
    
    animation.Value = (animation.Min + animation.Max) / 2;
    animation.Progress = 0;
  } else
  {
    animation.Progress += timestep / animation.Duration;
    animation.Value = (animation.Min + animation.Max + SDL_sinf(animation.Progress * 3.1415f * 2) * (animation.Max - animation.Min)) / 2;
    
    if (animation.Progress >= 1)
    {
      animation.Value = (animation.Max + animation.Min) / 2;
      if (!animation.Loop)
      {
        animation.Active = false;
        animation.Progress = 0;
      } else
      {
        animation.Progress = 0;
      }
    }
  }
}

static void colorUpdate(ColorAnimation& animation, float timestep)
{
  // TODO
}

void Animator::Update(float timestep)
{
  for (auto& animation : s_Animations)
  {
    switch (animation.Type)
    {
      case AnimationType::Lerp:
      {
        lerpUpdate(animation, timestep);
        break;
      }
      case AnimationType::Wave:
      {
        waveUpdate(animation, timestep);
        break;
      }
      default: break;
    }
  }
  
  for (auto& animation : s_ColorAnimations)
  {
    colorUpdate(animation, timestep);
  }
}

}
