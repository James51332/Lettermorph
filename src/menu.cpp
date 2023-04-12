#include "menu.h"
#include "ui.h"
#include "renderer.h"
#include "style.h"
#include "animation.h"
#include "input.h"

namespace ltrm
{

MenuScene::MenuScene()
{
  Animation titleAnim;
  titleAnim.Min = 100;
  titleAnim.Max = Renderer::GetWidth() - 100;
  titleAnim.Duration = 5;
  titleAnim.Type = AnimationType::Lerp;
  titleAnim.ResetOnInactive = true;
  titleAnim.ResetOnComplete = false;
  titleAnim.Active = false;
  m_TitleAnimID = Animator::RegisterAnimation(titleAnim);
}

MenuScene::~MenuScene()
{
  
}

void MenuScene::Load()
{
  
}

void MenuScene::Unload()
{
  
}

void MenuScene::Update()
{
  Renderer::Clear(Color::Accent);
  
  {
    Animator::SetAnimationActive(m_TitleAnimID, Input::KeyDown(SDLK_a));
    if (Input::KeyDown(SDLK_SPACE)) Animator::ResetAnimation(m_TitleAnimID);
    
    float x = Animator::QueryAnimation(m_TitleAnimID).Value;
    UI::Text("Lettermorph", x, 200, 1.0f);
  }
  
  if (UI::Button("Play", Renderer::GetWidth() / 2, Renderer::GetHeight() / 2, true))
  {
    SceneManager::ChangeScene("level");
  }
}

void MenuScene::KeyDown(SDL_Keycode key)
{
  
}

}
