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
  titleAnim.Min = 300;
  titleAnim.Max = Renderer::GetWidth() - 300;
  titleAnim.Duration = 1;
  titleAnim.Type = AnimationType::Wave;
  titleAnim.ResetOnComplete = true;
  titleAnim.Loop = false;
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
  	if (Input::KeyPress(SDLK_a)) Animator::SetAnimationActive(m_TitleAnimID);
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
